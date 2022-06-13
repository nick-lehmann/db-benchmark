#pragma once
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include <iostream>
#include <iterator>
#include <memory>
#include <vector>

#include "../column_store/ColumnStoreHelper.h"
#include "Constants.h"
#include "Filters.h"
#include "Filters_AVX.h"
#include "Types.h"

using namespace std;

// PAX Page that supports fixed width integers.
//
// No presence bits implemented as we do not store nullable values right now
// No v-minitables are supported because all T types we care about are
// fixed-sized.
template <typename T>
class PaxPage {
    unsigned short *start;
    long int pagesize;

    Header *numberOfAttributes;  // Fixed
    Header *attributeSizes;      // Variable
    Header *minipageOffsets;     // Variable
    Header *numberOfRecords;     // Fixed
    Header *freeSpace;           // Fixed

   public:
    // Read page from memory
    PaxPage(Header *start, long int pagesize) {
        this->start = start;
        this->pagesize = pagesize;
        numberOfAttributes = start;
    }

    // Write page to memory
    PaxPage(Header *start, long int pagesize, Header attributes) {
        this->start = start;
        this->pagesize = pagesize;

        numberOfAttributes = start;
        *(numberOfAttributes) = attributes;
        initHeaderLocations();

        this->start = start;
        *numberOfAttributes = attributes;

        for (int i = 0; i < attributes; i++) *(attributeSizes++) = sizeof(T);

        unsigned short headerLength = (3 + 2 * attributes) * sizeof(Header);
        unsigned short freeBytes = pagesize - headerLength;
        unsigned short recordSize = *numberOfAttributes * sizeof(T);
        unsigned short minipageSize = freeBytes / attributes;

        for (int i = 0; i < attributes; i++) {
            unsigned short offset = headerLength + (i * minipageSize);
            // cout << "Page " << i << " has offset " << offset << endl;
            *(minipageOffsets + i) = offset;
        }

        *numberOfRecords = 0;
        *freeSpace = freeBytes;
    }

    void initHeaderLocations() {
        attributeSizes = start + 1;
        minipageOffsets = attributeSizes + *numberOfAttributes;
        numberOfRecords = minipageOffsets + *numberOfAttributes;
        freeSpace = numberOfRecords + 1;
    }

    /**
     * @brief Get the maximum number of records that can be stored in one page
     *
     * @param pagesize
     * @param numberOfAttributes
     * @return int
     */
    static int getMaximumRows(long pagesize, Header numberOfAttributes) {
        // 3 header fields, plus 2 for every attribute
        unsigned short headerLength = (3 + 2 * numberOfAttributes) * sizeof(Header);
        return (pagesize - headerLength) / (numberOfAttributes * sizeof(T));
    }

    void print() {
        int lineWidth = 8;
        for (int i = 0; i < pagesize / sizeof(unsigned short); i++) {
            if (i % lineWidth == 0) cout << endl << &start[i] << "  ";
            cout << std::setw(8) << start[i] << " ";
        }
        cout << endl;
    }

    T *readRow(RowIndex index) {
        T *returnRow = new T[*(this->numberOfAttributes)];
        for (unsigned i = 0; i < *(this->numberOfAttributes); i++) {
            Header offset = *(minipageOffsets + i);

            T *minipage = (T *)(start + offset / 2);
            T *location = minipage + index;
            returnRow[i] = *location;
        }
        return returnRow;
    }

    T *projectRow(RowIndex index, std::vector<uint64_t> &projection) {
        T *returnRow = new T[projection.size()];
        for (unsigned i = 0; i < projection.size(); i++) {
            Header offset = *(minipageOffsets + projection[i]);

            T *minipage = (T *)(start + offset / 2);
            T *location = minipage + index;
            returnRow[i] = *location;
        }
        return returnRow;
    }

    void writeRecord(const T *record) {
        unsigned short recordSize = *numberOfAttributes * sizeof(T);
        if (*freeSpace < recordSize) {
            cout << "Page is full" << endl;
            throw;
        }

        // cout << "Write: ";
        for (int i = 0; i < *numberOfAttributes; i++) {
            Header offset = *(minipageOffsets + i);

            T *minipage = (T *)(start + offset / 2);
            T *location = minipage + *numberOfRecords;
            // cout << record[i] << " (" << location << ") ";
            *(location) = record[i];
        };
        // cout << endl;
        (*numberOfRecords)++;
        (*freeSpace) -= recordSize;
    }

    vector<unsigned> query(std::vector<Filters::Scalar::Filter<T> *> &filters) {
        vector<unsigned> positions;
        bool firstRun = true;

        for (auto &filter : filters) {
            // Find the minipage for the attribute to which the filter applies.
            Header offset = *(minipageOffsets + filter->index);
            T *minipage = (T *)((char *)start + offset);

            if (firstRun) {
                // On the first run, we need to go through all records.
                for (RowIndex index = 0; index < *numberOfRecords; index++) {
                    T cell = *(minipage + index);
                    if (filter->match(cell)) positions.push_back(index);
                }
                firstRun = false;
            } else {
                // On subsequent runs, we only need to go through the positions where
                // all previous filters already matched.
                for (auto &position : positions) {
                    T cell = *(minipage + position);
                    if (filter->match(cell)) positions.push_back(position);
                }
            }
        }

        return positions;
    }

    // Note: This function currently only works when T is set to uint64_t. Otherwise, the `ColumStore::Helper::store` function does not
    // work.
    // TODO: Allow for uint32_t values
    std::tuple<uint64_t *, uint64_t> queryAVX(std::vector<Filters::AVX::Filter<T> *> &filters) {
        // Array of all row indices which have passed all filters that were already applied. At the beginning, all rows all still present
        // because no filter has been applied yet. However, for performance reasons, we do not populate the array with all indicies but
        // rather compute the indices on the fly when the first filter is applied (firstRun=true).
        // Please note that, for performance reasons, we do not recreate the positions array after each iteration. Instead, we overwrite the
        // elements starting from the beginning of each iteration and keep track of all still valid positions using the `positionsCounter`.
        // All values with an index greater than `positionsCounter` are outdated intermediate results.
        auto positions = new uint64_t[*numberOfRecords];
        bool firstRun = true;

        // Count of all rows which have passed all filters.
        uint64_t positionsCounter = *numberOfRecords;

        // Batch size. Equals the number of cells that fit in one AVX512 register (512bites = 64bytes).
        auto rowsPerRegister = (unsigned)64 / sizeof(T);

        for (auto &filter : filters) {
            // Find the minipage for the attribute to which the filter applies.
            Header offset = *(minipageOffsets + filter->index);
            T *minipage = (T *)((char *)start + offset);

            auto *positionsTail = positions;

            // Counter of all rows which have also passed the current filter.
            uint64_t newPositionsCounter = 0;

            for (RowIndex rowIndex = 0; rowIndex < positionsCounter; rowIndex += rowsPerRegister) {
                // Check bounds for next load into AVX register.
                __mmask16 mask;
                if (rowIndex + rowsPerRegister <= positionsCounter) {
                    // We are inside the bounds. There is enough data still in the row to fill a whole AVX512 register.
                    mask = ONE_MASK;
                } else {
                    // We are close to the end of the row and there is not enough data left to fill a whole AVX512 register. Still, we will
                    // load as much data as needed to fill the whole register, but we tell the filter to ignore the random data that pads
                    // the register.
                    auto remaining = positionsCounter % rowsPerRegister;
                    mask = _mm512_int2mask(~(0xffffffff << remaining));
                }

                // Load the indices of the rows
                uint64_t *vindex;
                std::pair<__m512i, __m512i> loadResult;
                if (firstRun) {
                    // On the first run, the positions array contains only zeros. Since all rows are still valid, we can simply load the
                    // next batch of rows starting from the current `rowIndex`.
                    loadResult = ColumnStore::Helper::load(minipage, rowIndex);
                } else {
                    loadResult = ColumnStore::Helper::gather(positions + rowIndex, minipage);
                }

                // Load the cells of a row into the data register. `vindex` is the absolute index of the row and we, therefore, gather from
                // the start of the minipage.
                auto [dataRegister, indicesRegister] = loadResult;
                auto filterResult = filter->match(dataRegister, mask);

                // Store the successfully matched at the current end of the positions array.
                auto addedElements = ColumnStore::Helper::store(indicesRegister, filterResult, positionsTail);
                positionsTail += addedElements;
                newPositionsCounter += addedElements;
            }
            firstRun = false;
            positionsCounter = newPositionsCounter;
        }

        return std::make_tuple(positions, positionsCounter);
    }
};
