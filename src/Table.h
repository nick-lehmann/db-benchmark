#pragma once
#include <array>
#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "filters.h"

template <typename T>
class Table {
 public:
  // We only work on static data for which the dimensions (number of rows and
  // columns) is known at compile time.
  int numberOfAttributes;
  const int numberOfRows;

  Table(const int numberOfAttributes, const int numberOfRows)
      : numberOfAttributes(numberOfAttributes), numberOfRows(numberOfRows) {}
  ~Table() = 0;

  virtual void addRow(T *row) = 0;
  virtual T *getRow(const int rowIndex) = 0;
  virtual T *getCell(const int rowIndex, const int columnIndex) = 0;

  T *operator[](const int index) { return this->getRow(index);}

  virtual Table<T>* query(std::vector<int> &projection, std::vector<Filter<T> *> &filters) = 0;

  // Print the whole table
  // TODO: Add padding to make all cells align
  void print() {
    for (int rowIndex = 0; rowIndex < this->numberOfRows; ++rowIndex) {
      T *row = this->getRow(rowIndex);
      for (int columnIndex = 0; columnIndex < numberOfAttributes;
           ++columnIndex) {
        std::cout << row[columnIndex];

        // append separator (except for last attribute)
        if (columnIndex < (numberOfAttributes - 1)) std::cout << " | ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
};
