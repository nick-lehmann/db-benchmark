#pragma once

#include "IntermediateTable.h"
#include "RowStoreTable.h"
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace RowStore {
bool columnIndicesValid(std::vector<unsigned> columnIndices, uint32_t tupleSize) {
  // Check for each column index if it is in range of 0 and tupleSize
  for (uint64_t i = 0; i < columnIndices.size(); i++) {
    if (columnIndices[i] < 0 || columnIndices[i] >= tupleSize) {
      return false;
    }
  }
  return true;
}

template <typename T>
IntermediateTable<T> *projection(IntermediateTable<T> &table, std::vector<unsigned> &projectionParameters) {
  // Check for valid projection parameters
  if (!columnIndicesValid(projectionParameters, table.getTupleWidth())) {
    throw std::invalid_argument("Error! Invalid column indices!");
  }

  // Get pointer to table data
  auto data = table.getData();

  // Create empty intermediate table
  auto result = new IntermediateTable<T>(projectionParameters.size());

  // Iterate over given table tuples
  for (int i = 0; i < table.count(); i++) {
    // Create empty (temporary) tuple
    auto tuple = (T *)calloc(projectionParameters.size(), sizeof(T));

    // Fill tuple with data from every row of table
    for (int j = 0; j < projectionParameters.size(); j++) {
      tuple[j] = (*data)[i][projectionParameters[j]];
    }

    // Add tuple to result table
    result->addRow(tuple);
  }

  return result;
}
} // namespace RowStore
