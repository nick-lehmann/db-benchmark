#pragma once

#include "IntermediateTable.h"
#include "RowStoreTable.h"
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace RowStore {
bool columnIndicesValid(std::vector<unsigned> columnIndices, uint32_t tupleSize) {
  // check for each column index if it is in range of 0 and tupleSize
  for (uint64_t i = 0; i < columnIndices.size(); i++) {
    if (columnIndices[i] < 0 || columnIndices[i] >= tupleSize) {
      return false;
    }
  }
  return true;
}

template <typename T>
IntermediateTable<T> *projection(IntermediateTable<T> &table, std::vector<unsigned> &projectionParameters) {
  // check for valid projection parameters
  if (!columnIndicesValid(projectionParameters, table.getTupleWidth())) {
    throw std::invalid_argument("Error! Invalid column indices!");
  }

  // create empty intermediate table
  auto result = new IntermediateTable<T>(projectionParameters.size());

  // iterate over given table tuples
  for (int i = 0; i < table.count(); i++) {
    // create empty (temporary) tuple
    auto tuple = (T *)calloc(projectionParameters.size(), sizeof(T));

    // fill tuple with data from every row of table
    for (int j = 0; j < projectionParameters.size(); j++) {
      tuple[j] = table.getData()[i][projectionParameters[j]];
    }

    // add tuple to result table
    result->addRow(tuple);
  }

  return result;
}
} // namespace RowStore
