#pragma once
#include <errno.h>
#include <malloc.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include <iostream>
#include <iterator>
#include <vector>

#include "types.h"

using namespace std;

// PAX Page that supports fixed width integers.
//
// No presence bits implemented as we do not store nullable values right now
// Question: Can we skip the presence bit if the attribute is not nullable?
// Question: If the presence bit for a value is not set, will we not write the
// attribute in the minipage?
//
// No v-minitables are supported because all data types we care about are
// fixed-sized.
class Page {
  unsigned short *start;
  long int pagesize;

  Header *numberOfAttributes;
  Header *attributeSizes;
  Header *minipageOffsets;
  Header *numberOfRecords;
  Header *freeSpace;

 public:
  // Read page from memory
  Page(Header *start, long int pagesize) {
    this->start = start;
    this->pagesize = pagesize;
    numberOfAttributes = start;
  }

  // Write page to memory
  Page(Header *start, long int pagesize, Header attributes) {
    this->start = start;
    this->pagesize = pagesize;
    clear();

    numberOfAttributes = start;
    *(numberOfAttributes) = attributes;
    initHeaderLocations();

    cout << "Writing new page" << endl;
    this->start = start;

    *numberOfAttributes = attributes;

    for (int i = 0; i < attributes; i++) {
      *(attributeSizes++) = sizeof(Data);
    }

    unsigned short headerLength = (3 + 2 * attributes) * sizeof(unsigned short);
    unsigned short freeBytes = pagesize - headerLength;
    unsigned short recordSize = *numberOfAttributes * sizeof(Data);
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

  vector<Data> readRecord(RowIndex index) {
    vector<Data> result;

    for (int i = 0; i < *numberOfAttributes; i++) {
      Header offset = *(minipageOffsets + i);

      Data *minipage = (Data *)(start + offset / 2);
      Data *location = minipage + index;

      result.push_back(*location);
    }

    return result;
  }

  void writeRecord(Data *record) {
    unsigned short recordSize = *numberOfAttributes * sizeof(Data);
    if (*freeSpace < recordSize) {
      cout << "Page is full" << endl;
      return;
    }

    for (int i = 0; i < *numberOfAttributes; i++) {
      Header offset = *(minipageOffsets + i);

      Data *minipage = (Data *)(start + offset / 2);
      Data *location = minipage + *numberOfRecords;
      cout << "Writing attribute " << i << " to offset " << offset << " at "
           << location << endl;
      *(location) = record[i];
    };
    (*numberOfRecords)++;
    (*freeSpace) -= recordSize;
  }

  void process(vector<Equal> filters) {
    vector<vector<Data>> results = {};
    for (RowIndex index = 0; index < *numberOfRecords; index++) {
      vector<Data> record = readRecord(index);

      bool fits = true;
      for (auto &filter : filters) {
        Data cell = record.at(filter.index);
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
