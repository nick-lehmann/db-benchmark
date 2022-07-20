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
#include "Filters/Base.h"
#include "Types.h"

using namespace std;

/**
 * @brief PAX Page that supports fixed width integers.
 *
 * No presence bits implemented as we do not store nullable values right now.
 * No v-minitables are supported because all T types we care about are fixed-sized.
 *
 * |numberOfAttributes|attributeSize|minipageOffsets|numberOfRecords|freespace|data....|
 *
 * Example
 *
 * 3 attributes/columns with a size of 8bit/1byte each. and 10 saved records.
 *
 * | 3 | 8 | 8 | 8 | 18 | 1377 | 2736 | 10 | 3838
 *
 * 3 = number of attributes
 * 8x3 = 3 times the size of the attributes
 * 18, 1377, 2736 = offsets inside the minipage for each column
 * 10 = number of records in the page
 * 3838 = free space in the page
 *
 *
 * @tparam T type of data
 */
template <typename T, typename idxT = T>
class PaxPage {
   private:
    unsigned short *start;
    long int pagesize;

    Header *numberOfAttributes;  // Fixed
    Header *attributeSizes;      // Variable, dependent on the amount of attributes
    Header *minipageOffsets;     // Variable, dependent on the amount of attributes
    Header *numberOfRecords;     // Fixed
    Header *freeSpace;           // Fixed

    T *getMinipageForAttribute(unsigned index) {
        const Header offset = *(this->minipageOffsets + index);
        return (T *)((char *)this->start + offset);
    }

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

        this->numberOfAttributes = start;
        *this->numberOfAttributes = attributes;

        initHeaderLocations();

        for (int i = 0; i < attributes; i++) {
            *this->attributeSizes++ = sizeof(T);
        }

        const unsigned short headerLength = getHeaderSize(attributes);
        const unsigned short freeBytes = pagesize - headerLength;
        const unsigned short recordSize = *numberOfAttributes * sizeof(T);
        const unsigned short minipageSize = freeBytes / attributes;

        for (int i = 0; i < attributes; i++) {
            const unsigned short offset = headerLength + (i * minipageSize);
            *(this->minipageOffsets + i) = offset;
        }

        *this->numberOfRecords = 0;
        *this->freeSpace = freeBytes;
    }

    void initHeaderLocations() {
        // All internal class variables
        attributeSizes = start + 1;
        minipageOffsets = attributeSizes + *numberOfAttributes;
        numberOfRecords = minipageOffsets + *numberOfAttributes;
        freeSpace = numberOfRecords + 1;
    }

    /**
     * @brief Get the Header Size object
     *
     * @param numberOfAttributes
     * @return unsigned short
     */
    static unsigned short getHeaderSize(Header numberOfAttributes) {
        // 3 header fields, plus 2 (attribute size + minipage offset) for every attribute
        return (3 + 2 * numberOfAttributes) * sizeof(Header);
    }

    /**
     * @brief Get the maximum number of records that can be stored in one page
     *
     * @param pagesize
     * @param numberOfAttributes
     * @return int
     */
    static int getMaximumRows(long pagesize, Header numberOfAttributes) {
        auto headerSize = getHeaderSize(numberOfAttributes);
        return (pagesize - headerSize) / (numberOfAttributes * sizeof(T));
    }

    /**
     * @brief prints the page table. 2 bytes at a time. (the size of the header type)
     *
     */
    void print() {
        const int lineWidth = 8;
        for (int i = 0; i < this->pagesize / sizeof(unsigned short); i++) {
            if (i % lineWidth == 0) cout << endl << &start[i] << "  ";
            cout << std::setw(8) << start[i] << " ";
        }
        cout << endl;
    }

    T *readRow(RowIndex index) {
        T *returnRow = new T[*(this->numberOfAttributes)];
        for (unsigned i = 0; i < *(this->numberOfAttributes); i++) {
            const auto minipage = this->getMinipageForAttribute(i);
            const T *location = minipage + index;
            returnRow[i] = *location;
        }
        return returnRow;
    }

    T *projectRow(RowIndex index, std::vector<uint64_t> &projection) {
        T *returnRow = new T[projection.size()];
        for (unsigned i = 0; i < projection.size(); i++) {
            const auto minipage = this->getMinipageForAttribute(projection[i]);
            const T *location = minipage + index;
            returnRow[i] = *location;
        }
        return returnRow;
    }

    void writeRecord(const T *record) {
        const unsigned short recordSize = *numberOfAttributes * sizeof(T);
        if (*this->freeSpace < recordSize) {
            cout << "Page is full" << endl;
            throw;
        }

        for (int i = 0; i < *this->numberOfAttributes; i++) {
            const auto minipage = this->getMinipageForAttribute(i);
            T *location = minipage + *numberOfRecords;
            *(location) = record[i];
        };
        *this->numberOfRecords += 1;
        *this->freeSpace -= recordSize;
    }

    vector<unsigned> query(std::vector<Filters::Filter<T, SIMD::None> *> &filters) {
        vector<unsigned> positions;
        bool firstRun = true;

        for (auto &filter : filters) {
            // Find the minipage for the attribute to which the filter applies.
            const auto minipage = this->getMinipageForAttribute(filter->index);

            if (firstRun) {
                // On the first run, we need to go through all records.
                for (RowIndex index = 0; index < *numberOfRecords; index++) {
                    const T cell = *(minipage + index);
                    if (filter->match(cell)) positions.push_back(index);
                }
                firstRun = false;
            } else {
                vector<unsigned> positionsFiltered;
                // On subsequent runs, we only need to go through the positions where
                // all previous filters already matched.
                for (auto &position : positions) {
                    const T cell = *(minipage + position);
                    if (filter->match(cell)) positionsFiltered.push_back(position);
                }
                positions = positionsFiltered;
            }
        }

        return positions;
    }

    // Note: This function currently only works when T is set to uint64_t. Otherwise, the `ColumStore::Helper::store` function does not
    // work.
    // TODO: Allow for uint32_t values
    std::tuple<idxT *, uint64_t> queryAVX(std::vector<Filters::Filter<T, SIMD::AVX512> *> &filters) {
        // Array of all row indices which have passed all filters that were already applied. At the beginning, all rows all still present
        // because no filter has been applied yet. However, for performance reasons, we do not populate the array with all indicies but
        // rather compute the indices on the fly when the first filter is applied (firstRun=true).
        // Please note that, for performance reasons, we do not recreate the positions array after each iteration. Instead, we overwrite the
        // elements starting from the beginning of each iteration and keep track of all still valid positions using the `positionsCounter`.
        // All values with an index greater than `positionsCounter` are outdated intermediate results.
        auto positions = new idxT[*numberOfRecords];
        bool firstRun = true;

        // Count of all rows which have passed all filters.
        uint64_t positionsCounter = *numberOfRecords;

        // Batch size. Equals the number of cells that fit in one AVX512 register (512bites = 64bytes).
        auto rowsPerRegister = (unsigned)64 / sizeof(T);

        for (auto &filter : filters) {
            // Find the minipage for the attribute to which the filter applies.
            // Header offset = *(minipageOffsets + filter->index);
            // T *minipage = (T *)((char *)start + offset);
            const auto minipage = this->getMinipageForAttribute(filter->index);

            auto *positionsTail = positions;

            // Counter of all rows which have also passed the current filter.
            uint64_t newPositionsCounter = 0;

            for (idxT rowIndex = 0; rowIndex < positionsCounter; rowIndex += rowsPerRegister) {
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
