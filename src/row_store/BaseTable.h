#pragma once

#include <array>
#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "Filter.h"
#include "IntermediateTable.h"
#include "Projection.h"
#include "Table.h"

namespace RowStore {
template <typename T> class BaseTable : public Table<T> {

public:
  std::vector<T *> data;

public:
  // constructor for BaseTable
  BaseTable(unsigned numAttributes, unsigned numRows, const T **initialData)
      : Table<T>(numAttributes, numRows, initialData) {

    // data = new std::vector<T>[numAttributes];
    for (unsigned row = 0; row < numRows; row++) {
      data.push_back(static_cast<T *>(malloc(sizeof(T) * numAttributes)));
      for (unsigned column = 0; column < numAttributes; column++) {
        data.back()[column] = initialData[row][column];
      }
    }
  }

  // destructor for BaseTable
  ~BaseTable() override {
    // delete the tuple of the base table
    for (uint64_t i = 0; i < data.size(); ++i) {
      free(data[i]);
    }
  }

  T *getRow(unsigned rowIndex) override { return data[rowIndex]; }

  T **query_table(std::vector<unsigned> &projection, std::vector<Filter<T> *> &filters, unsigned &numberOfRows,
                  unsigned &numberOfColumns) override {
    RowStore::IntermediateTable<T> result(this->numberOfAttributes, data);

    //  TODO

    result.printTableOutput();

    numberOfColumns = result.tupleWidth;
    return result.table(numberOfRows);
  }

  uint64_t query_count(std::vector<unsigned> &projection, std::vector<Filter<T> *> &filters) override {
    RowStore::IntermediateTable<T> result(this->numberOfAttributes, data);
    //  TODO
    return result.count();
  }

  bool columnIndicesValid(std::vector<int> columnIndices, int tupleSize) {
    // check for each column index if it is in range of 0 and tupleSize
    for (uint64_t i = 0; i < columnIndices.size(); i++) {
      if (columnIndices[i] < 0 || columnIndices[i] >= tupleSize) {
        return false;
      }
    }
    return true;
  }
};
} // namespace RowStore
