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

  /**
   * @brief Get index of the page
   *
   * @param rowIndex
   * @return int
   */
  int pageIndex(const int rowIndex) { return rowIndex / rowsPerPage; }

 public:
  PaxTable(int numberOfAttributes, int numberOfRows, const T **initialData)
      : Table<T>(numberOfAttributes, numberOfRows, initialData) {
    auto pagesize = getPagesize();
    this->rowsPerPage =
        PaxPage<T>::getMaximumRows(pagesize, numberOfAttributes);

    // Resize vector to contain all needed pages
    unsigned int neededPages =
        (numberOfRows + (rowsPerPage - 1)) / rowsPerPage;  // Rounding up
    cout << "Needed pages: " << neededPages << endl;
    this->pages.reserve(neededPages);

    // Initialize all pages
    for (int pageIndex = 0; pageIndex < neededPages; pageIndex++) {
      Buffer buffer = allocateAlignedBuffer(pagesize);
      const PaxPage<T> page =
          PaxPage<T>((Header *)buffer, pagesize, numberOfAttributes);
      cout << "Allocate page " << pageIndex << endl;
      auto page_ref(page);
      this->pages.push_back(page_ref);
    }
  }

  virtual ~PaxTable() { cout << "Destroy pax table" << endl; }

  T *getRow(unsigned rowIndex) override {
    PaxPage<T> &page = pages[pageIndex(rowIndex)];
    vector<T> row = page.readRow(rowIndex % this->rowsPerPage);
    return &(row[0]);
  }

  T *getCell(const int rowIndex, const int columnIndex) {
    T *row = this->getRow(rowIndex);
    return row + columnIndex;
  }

  T **query_table(std::vector<unsigned> &projection,
                  std::vector<Filter<T> *> &filters, unsigned &numberOfRows,
                  unsigned &numberOfColumns) override {
    T **data = (T **)malloc(numberOfRows * sizeof(T *));
    // for (unsigned row = 0; row < numberOfRows; row++) {
    //   T *rowData = (T *)malloc(numberOfColumns * sizeof(T));
    //   data[row] = rowData;
    // }
    return data;
  };

  uint64_t query_count(std::vector<unsigned> &projection,
                       std::vector<Filter<T> *> &filters) override {
    return 0;
  }

  void print() {
    cout << "Pax Table" << endl;
    for (const auto &page : this->pages) {
      cout << "Page" << endl;
    }
  }
};
