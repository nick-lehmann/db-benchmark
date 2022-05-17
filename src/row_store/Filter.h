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
  int i = 0;
row_iterator: // create label to be able to use goto
  for (; i < table.count(); ++i) {
    T *row = (*data)[i];
    // Iterate over filters and match
    for (int j = 0; j < filters.size(); ++j) {
      if (!filters[j]->match(row[filters[j]->index])) {
        ++i;               // increment i for next iteration of row_iterator loop
        goto row_iterator; // continues outer loop
      }
    }
    // this is only executed, when all fillters match; otherwise the loop is continued to the next iteration
    result->addRowCopy(row);
  }
  return result;
}
} // namespace RowStore
