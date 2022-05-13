#pragma once

#include "IntermediateTable.h"
#include "RowStoreTable.h"
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace RowStore {
bool columnIndicesValid(std::vector<int> columnIndices, int tupleSize) {
  // check for each column index if it is in range of 0 and tupleSize
  for (uint64_t i = 0; i < columnIndices.size(); i++) {
    if (columnIndices[i] < 0 || columnIndices[i] >= tupleSize) {
      return false;
    }
  }
  return true;
}

/*
template <typename T>
RowStoreTable<T> *projection(RowStoreTable<T> &table, std::vector<int> &projection) {
    if (!columnIndicesValid(projection, table.numberOfAttributes)) {
        throw std::invalid_argument("Error! Invalid column indices!");
    }

    // create empty table
    auto result = new Table<T>(projection.size(), true);

    // iterate over given table tuples
    for (int i = 0; i < table.size(); i++) {
        // create empty (temporary) tuple
        auto tuple = result->getTuple(result->createEmptyTuple());

        // fill tuple with data from every row of table
        for (int j = 0; j < projection.size(); j++) {
            tuple[j] = table[i][projection[j]];
        }
    }

    return result;
}*/

template <typename T>
IntermediateTable<T> *projection(IntermediateTable<T> &table, std::vector<int> &projectionParameters) {
  // check for valid projection parameters
  if (!columnIndicesValid(projection, table.numberOfAttributes)) {
    throw std::invalid_argument("Error! Invalid column indices!");
  }

  // create empty intermediate table
  auto result = new IntermediateTable<T>(projectionParameters.size());

  // iterate over given table tuples
  for (int i = 0; i < table.data.size(); i++) {
    // create empty (temporary) tuple
    auto tuple = (T *)calloc(projectionParameters.size(), sizeof(T));

    // fill tuple with data from every row of table
    for (int j = 0; j < projectionParameters.size(); j++) {
      tuple[j] = table[i][projectionParameters[j]];
    }

    // add tuple to result table
    result->addRow(tuple);
  }

  return result;
}
} // namespace RowStore
