#pragma once
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#include "BaseTable.h"

namespace RowStore {
template <typename T> class IntermediateTable {
private:
  uint32_t tupleWidth = 0;
  std::vector<T *> data;

  T **tableOutput = nullptr;
  uint64_t tableOutputSize = 0;

public:
  // Constructor: creates an empty Intermediate Table
  IntermediateTable(uint32_t tupleWidth) : tupleWidth(tupleWidth) {}
  // Constructor: creates an Intermediate Table and copies all data from a BaseTable
  IntermediateTable(uint32_t tupleWidth, BaseTable<T> &table) : tupleWidth(tupleWidth) {
    for (uint64_t i = 0; i < table.data.size(); ++i) {
      addRow(table.getRow(i));
    }
  }

  // Destructor: frees tableOutput and frees all pointer created in data
  ~IntermediateTable() {
    freeTableOutput();
    for (uint64_t i = 0; i < data.size(); ++i) {
      free(data[i]);
    }
  }

  // append a row to the end of the table and copies the content from row
  void addRow(T *row) {
    data.push_back(static_cast<T *>(malloc(sizeof(T) * tupleWidth)));
    std::memcpy(data.back(), row, sizeof(T) * tupleWidth);
  }

  // deletes (and frees) a row from the table
  void removeRow(uint64_t index) {
    T *tmp = data[index];
    data.erase(index);
    free(tmp);
  }

  // returns the number of rows in this table
  uint64_t count() {
    return data.size(); // is this sufficient to avoid the compiler from removing the data?
  }

  // frees tableOutput if it already exists and resets tableOutputSize
  void freeTableOutput() {
    if (tableOutput) {
      for (uint64_t i = 0; i < data.size(); ++i) {
        free(tableOutput[i]);
      }
      free(tableOutput);
      tableOutput = nullptr;
      tableOutputSize = 0;
    }
  }

  // transforms data to T ** to output it
  T **table(uint64_t &outputSize) {
    freeTableOutput();

    tableOutput = static_cast<T **>(malloc(sizeof(T *) * data.size()));
    tableOutputSize = data.size();
    for (uint64_t i = 0; i < data.size(); ++i) {
      tableOutput[i] = static_cast<T *>(malloc(sizeof(T) * tupleWidth));
      std::memcpy(tableOutput[i], data[i], sizeof(T) * tupleWidth);
    }
    outputSize = tableOutputSize;

    return tableOutput;
  }
};

template <typename T> void printTableOutput(uint32_t tupleSize, uint64_t rowCount, T **tableData) {
  for (uint64_t row = 0; row < rowCount; ++row) {
    for (uint32_t col = 0; col < tupleSize; ++col) {
      // prepend a column separator except for the first column
      if (col) {
        std::cout << " | ";
      }
      std::cout << tableData[row][col];
    }
    std::cout << std::endl;
  }
}
} // namespace RowStore
