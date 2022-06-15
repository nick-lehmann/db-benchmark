#pragma once
#include <cstdint>
#include <stdexcept>

#include "Filters.h"
#include "IntermediateTable.h"

namespace RowStore {

/// Selects these tuples of table that match all Filter-objects in filters and returns the address of the newly created
/// result table that contains exactly the selected tuples
/// \param table intermediateTable that is filtered
/// \param filters vector of filters that are applied conjunctive
template <typename T>
IntermediateTable<T> *apply_filters(IntermediateTable<T> &table, std::vector<Filters::Scalar::Filter<T> *> &filters) {
    // Validate filters
    for (int i = 0; i < filters.size(); ++i) {
        if (filters[i]->index < 0 || filters[i]->index >= table.getTupleWidth()) {
            throw std::invalid_argument("Error! Invalid attribute index!");
        }
    }

    // Create empty intermediate table
    auto result = new IntermediateTable<T>(table.getTupleWidth(), table.getRowCount());

    // Iterate over table rows
    int i = 0;
row_iterator:  // create label to be able to use goto
    for (; i < table.count(); ++i) {
        T *row = table[i];
        // Iterate over filters and match
        for (int j = 0; j < filters.size(); ++j) {
            if (!filters[j]->match(row[filters[j]->index])) {
                ++i;                // increment i for next iteration of row_iterator loop
                goto row_iterator;  // continues outer loop
            }
        }
        // this is only executed, when all fillters match; otherwise the loop is continued to the next iteration
        result->addRow(row);
    }
    return result;
}
}  // namespace RowStore
