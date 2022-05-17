#pragma once
#include "Filters.h"
#include "IntermediateTable.h"
#include <cstdint>
#include <stdexcept>

namespace RowStore {
template <typename T>
IntermediateTable<T> *apply_filters(IntermediateTable<T> &table, std::vector<Filter<T> *> &filters) {
  // Validate filters
  for (int i = 0; i < filters.size(); ++i) {
    if (filters[i]->index < 0 || filters[i]->index >= table.getTupleWidth()) {
      throw std::invalid_argument("Error! Invalid attribute index!");
    }
  }
  // Get pointer to table data
  std::vector<T *> *data = table.getData();
  // Create empty intermediate table
  auto result = new IntermediateTable<T>(table.getTupleWidth());

  // Iterate over table rows
  for (int i = 0; i < table.count(); ++i) {
    bool match = true;
    // Iterate over filters and match
    for (int j = 0; j < filters.size(); ++j) {
      if (!filters[j]->match((*data)[i][filters[j]->index])) {
        match = false;
      }
    }
    if (match) {
      result->addRow((*data)[i]);
    }
  }

  return result;
}
} // namespace RowStore
