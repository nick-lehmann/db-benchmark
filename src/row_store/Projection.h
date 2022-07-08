#pragma once

#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <vector>

#include "intermediateRepresentation/IntermediateTable.h"

namespace RowStore {
/// Cecks whether all column indices are vallid for the given tupleSize
/// \param columnIndices column indices that are checked for validity
/// \param tupleSize tuple width of the table that should be projected
bool columnIndicesValid(std::vector<uint64_t> columnIndices, uint32_t tupleSize) {
    // Check for each column index if it is in range of 0 and tupleSize
    for (uint64_t i = 0; i < columnIndices.size(); i++) {
        if (columnIndices[i] < 0 || columnIndices[i] >= tupleSize) {
            return false;
        }
    }
    return true;
}

/// Copies the content of the columns that are referred in projectionParameters to a new intermediateTable-Objects and
/// returns its address projected
/// \param table table data of the original table
/// \param projectionParameters vector of column indices that are contained in the result
template <typename T>
IntermediateTable<T> *projection(IntermediateTable<T> &table, std::vector<uint64_t> &projectionParameters) {
    // Check for valid projection parameters
    if (!columnIndicesValid(projectionParameters, table.getTupleWidth())) {
        throw std::invalid_argument("Error! Invalid column indices!");
    }

    // Create empty intermediate table
    auto result = new IntermediateTable<T>(projectionParameters.size(), table.getRowCount());

    // Iterate over given table tuples
    for (int i = 0; i < table.count(); i++) {
        // Create empty (temporary) tuple
        auto tuple = (T *)malloc(projectionParameters.size() * sizeof(T));

        // Fill tuple with data from every row of table
        for (int j = 0; j < projectionParameters.size(); j++) {
            tuple[j] = table[i][projectionParameters[j]];
        }

        // Add tuple to result table
        result->addRow(tuple);
        free(tuple);
    }

    return result;
}

/// Copies the content of the columns that are referred in projectionParameters to a new intermediateTable-Objects and
/// returns its address projected
/// \param table table data of the original table
/// \param projectionParameters vector of column indices that are contained in the result
template <typename T>
IntermediateTable_AVX<T> *projection_AVX(IntermediateTable_AVX<T> &table, std::vector<uint64_t> &projectionParameters) {
    // Check for valid projection parameters
    if (!columnIndicesValid(projectionParameters, table.getTupleWidth())) {
        throw std::invalid_argument("Error! Invalid column indices!");
    }

    // Create empty intermediate table
    auto result = new IntermediateTable_AVX<T>(projectionParameters.size(), table.getRowCount());

    // Iterate over given table tuples
    for (int i = 0; i < table.count(); i++) {
        // Create empty (temporary) tuple
        auto tuple = (T *)malloc(projectionParameters.size() * sizeof(T));

        // Fill tuple with data from every row of table
        for (int j = 0; j < projectionParameters.size(); j++) {
            tuple[j] = table[i][projectionParameters[j]];
        }

        // Add tuple to result table
        result->addRow(tuple);
        free(tuple);
    }

    return result;
}

}  // namespace RowStore
