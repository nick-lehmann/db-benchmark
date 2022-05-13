#pragma once
#include "Filters.h"
#include "IntermediateTable.h"
#include <cstdint>
#include <stdexcept>

namespace RowStore {
template <typename T> IntermediateTable<T> *apply_filter(IntermediateTable<T> &table, Filter<T> *filter) {
  if (filter->index < 0 || filter->index >= table.getTupleWidth()) {
    throw std::invalid_argument("Error! Invalid attribute index!");
  }

  // Get pointer to table data
  auto data = table.getData();

  // Create empty intermediate table
  auto result = new IntermediateTable<T>(table.getTupleWidth());

  // Iterate over table rows
  for (int i = 0; i < table.count(); ++i) {
    // Add matching rows to result table
    if (filter->match((*data)[i][filter->index])) {
      result->addRow((*data)[i]);
    }
  }

  return result;
}

template <typename T>
IntermediateTable<T> *apply_filters(IntermediateTable<T> &table, std::vector<Filter<T> *> &filters) {
  // Create empty intermediate table
  auto result = new IntermediateTable<T>(table.getTupleWidth(), (*table.getData()));

  for (int i = 0; i < filters.size(); i++) {
    result = apply_filter((*result), filters[i]);
  }

  return result;
}

} // namespace RowStore
