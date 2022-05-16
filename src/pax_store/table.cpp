#pragma once
#include "Table.h"

#include <functional>
#include <vector>

#include "memory.cpp"
#include "page.cpp"
#include "types.h"

template <typename T>
class PaxTable : public Table<T> {
 private:
  unsigned int rowsPerPage;
  unsigned int numberOfPages;

  /**
   * @brief Get index of the page
   *
   * @param rowIndex
   * @return int
   */
  int pageIndex(const int rowIndex) { return rowIndex / rowsPerPage; }

 public:
  PaxPage<T> *pages;

  PaxTable(int numberOfAttributes, int numberOfRows, const T **initialData)
      : Table<T>(numberOfAttributes, numberOfRows, initialData) {
    auto pagesize = getPagesize();
    this->rowsPerPage =
        PaxPage<T>::getMaximumRows(pagesize, numberOfAttributes);

    cout << "Rows per page: " << rowsPerPage << endl;

    // Resize vector to contain all needed pages
    this->numberOfPages =
        (numberOfRows + (rowsPerPage - 1)) / rowsPerPage;  // Rounding up
    cout << "Needed pages: " << this->numberOfPages << endl;
    this->pages =
        (PaxPage<T> *)malloc(this->numberOfPages * sizeof(PaxPage<T>));

    // Initialize all pages
    for (int pageIndex = 0; pageIndex < numberOfPages; pageIndex++) {
      Buffer buffer = allocateAlignedBuffer(pagesize);
      PaxPage<T> page =
          PaxPage<T>((Header *)buffer, pagesize, numberOfAttributes);

      // Write data to page
      for (int rowIndex = pageIndex * rowsPerPage;
           rowIndex < (pageIndex + 1) * rowsPerPage && rowIndex < numberOfRows;
           rowIndex++) {
        page.writeRecord(initialData[rowIndex]);
      }

      *(this->pages + pageIndex) = page;
    }
  }

  // TODO: Free memory
  virtual ~PaxTable() { cout << "Destroy pax table" << endl; }

  T *getRow(unsigned rowIndex) override {
    PaxPage<T> &page = pages[pageIndex(rowIndex)];
    return page.readRow(rowIndex % this->rowsPerPage);
  }

  T *projectRow(unsigned rowIndex, std::vector<unsigned> &projection) {
    PaxPage<T> &page = pages[pageIndex(rowIndex)];
    return page.projectRow(rowIndex % this->rowsPerPage, projection);
  }

  T *getCell(const int rowIndex, const int columnIndex) {
    T *row = this->getRow(rowIndex);
    return row + columnIndex;
  }

  T **query_table(std::vector<unsigned> &projection,
                  std::vector<Filter<T> *> &filters, unsigned &numberOfRows,
                  unsigned &numberOfColumns) override {
    vector<unsigned> positions;

    // Find positions of all rows that match the given filters.
    for (unsigned pageIndex = 0; pageIndex < this->numberOfPages; pageIndex++) {
      PaxPage<T> page = this->pages[pageIndex];
      vector<unsigned> newPositions = page.query(filters);
      positions.insert(positions.end(), newPositions.begin(),
                       newPositions.end());
    }

    numberOfColumns = projection.size();
    numberOfRows = positions.size();

    // Allocate enough memory for all rows.
    T **data = (T **)malloc(positions.size() * sizeof(T *));
    for (int i = 0; i < positions.size(); i++)
      data[i] = this->projectRow(positions[i], projection);

    return data;
  };

  uint64_t query_count(std::vector<unsigned> &projection,
                       std::vector<Filter<T> *> &filters) override {
    unsigned returnedRows, returnedColumns;
    query_table(projection, filters, returnedRows, returnedColumns);
    return returnedRows;
  }
};
