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

#include "../include/Filters.h"
#include "types.h"

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

  Header *numberOfAttributes;
  Header *attributeSizes;
  Header *minipageOffsets;
  Header *numberOfRecords;
  Header *freeSpace;

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
    clear();

    numberOfAttributes = start;
    *(numberOfAttributes) = attributes;
    initHeaderLocations();

    cout << "Writing new page" << endl;
    this->start = start;

    *numberOfAttributes = attributes;

    for (int i = 0; i < attributes; i++) *(attributeSizes++) = sizeof(T);

    unsigned short headerLength = (3 + 2 * attributes) * sizeof(Header);
    unsigned short freeBytes = pagesize - headerLength;
    unsigned short recordSize = *numberOfAttributes * sizeof(T);
    cout << "Freespace: " << freeBytes << " (" << freeBytes / recordSize
         << " records)" << endl;

    unsigned short minipageSize = freeBytes / attributes;
    cout << "Minipage size: " << minipageSize << endl;

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

  static unsigned int getMaximumRows(unsigned long pagesize,
                                     Header numberOfAttributes) {
    unsigned short headerLength = (3 + 2 * numberOfAttributes) * sizeof(Header);

    return (pagesize - headerLength) / numberOfAttributes * sizeof(T);
  }

  void print() {
    int lineWidth = 8;
    for (int i = 0; i < pagesize / sizeof(unsigned short); i++) {
      if (i % lineWidth == 0) cout << endl << &start[i] << "  ";
      cout << std::setw(4) << start[i] << " ";
    }
    cout << endl;
  }

  void clear() {
    for (int i = 0; i < pagesize / sizeof(unsigned short); i++) {
      *(start + i) = 0;
    }
  }

  vector<T> readRow(RowIndex index) {
    vector<T> result;

    for (int i = 0; i < *numberOfAttributes; i++) {
      Header offset = *(minipageOffsets + i);

      T *minipage = (T *)(start + offset / 2);
      T *location = minipage + index;

      result.push_back(*location);
    }

    return result;
  }

  void writeRecord(T *record) {
    unsigned short recordSize = *numberOfAttributes * sizeof(T);
    if (*freeSpace < recordSize) {
      cout << "Page is full" << endl;
      return;
    }

    for (int i = 0; i < *numberOfAttributes; i++) {
      Header offset = *(minipageOffsets + i);

      T *minipage = (T *)(start + offset / 2);
      T *location = minipage + *numberOfRecords;
      cout << "Writing attribute " << i << " to offset " << offset << " at "
           << location << endl;
      *(location) = record[i];
    };
    (*numberOfRecords)++;
    (*freeSpace) -= recordSize;
  }

  void process(vector<Filter<T> *> filters) {
    vector<vector<T>> results = {};
    for (RowIndex index = 0; index < *numberOfRecords; index++) {
      vector<T> record = readRow(index);

      bool fits = true;
      for (auto &filter : filters) {
        T cell = record.at(filter.index);
        if (!filter.match(&cell)) {
          fits = false;
          break;
        }
      }
      if (fits) {
        // cout << "Row " << index << " fits" << endl;
        results.push_back(record);
      }
    }

    cout << "Found " << results.size() << " results:" << endl;
    for (auto &record : results) {
      for (auto &cell : record) {
        cout << cell << ", ";
      }
      cout << endl;
    }
  }
};
