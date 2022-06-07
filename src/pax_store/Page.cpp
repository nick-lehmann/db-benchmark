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

#include "Filters.h"
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
            cout << "Page " << i << " has offset " << offset << endl;
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

        cout << "Write: ";
        for (int i = 0; i < *numberOfAttributes; i++) {
            Header offset = *(minipageOffsets + i);

            T *minipage = (T *)(start + offset / 2);
            T *location = minipage + *numberOfRecords;
            cout << record[i] << " (" << location << ") ";
            *(location) = record[i];
        };
        cout << endl;
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
};
