#pragma once
#include "Table.h"

#include <functional>
#include <vector>

#include "memory.cpp"
#include "page.cpp"
#include "types.h"

template <typename T>
class PaxTable : Table<T> {
 private:
  vector<reference_wrapper<PaxPage<T>>> pages;
  unsigned int rowsPerPage;
  int pageIndex(const int rowIndex) { return rowIndex / rowsPerPage; }

 public:
  PaxTable(int numberOfAttributes, int numberOfRows, const T **initialData)
      : Table<T>(numberOfAttributes, numberOfRows, initialData) {
    unsigned long pagesize = getPagesize();
    rowsPerPage = PaxPage<T>::getMaximumRows(numberOfAttributes, pagesize);

    // Resize vector to contain all needed pages
    unsigned int neededPages = numberOfRows / rowsPerPage;
    this->pages.reserve(neededPages);

    // Initialize all pages
    for (int pageIndex = 0; pageIndex < neededPages; pageIndex++) {
      Buffer buffer = allocateAlignedBuffer(pagesize);
      const PaxPage<T> &page =
          PaxPage<T>((Header *)buffer, pagesize, numberOfAttributes);
      cout << "Allocate page " << pageIndex << endl;

      // pages.push_back(page);
    }
  }

  virtual ~PaxTable() { cout << "Destroy pax table" << endl; }

  T *getRow(unsigned rowIndex) override {
    PaxPage<T> &page = pages[pageIndex(rowIndex)];
    vector<T> row = page.readRow(rowIndex % rowsPerPage);
    return &(row[0]);
  }

  T *getCell(const int rowIndex, const int columnIndex) {
    T *row = this->getRow(rowIndex);
    return row + columnIndex;
  }

  T **query_table(std::vector<unsigned> &projection,
                  std::vector<Filter<T>> &filters) override {}

  uint64_t query_count(std::vector<unsigned> &projection,
                       std::vector<Filter<T>> &filters) override {}
};