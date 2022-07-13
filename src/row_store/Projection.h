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
template <typename T, SIMD Variant, int Alignment>
IntermediateTable<T, Variant, Alignment> *projection_unified(IntermediateTable<T, Variant, Alignment> &table,
                                                             std::vector<uint64_t> &projectionParameters) {
    // Check for valid projection parameters
    if (!columnIndicesValid(projectionParameters, table.getTupleWidth())) {
        throw std::invalid_argument("Error! Invalid column indices!");
    }

    // Create empty intermediate table
    auto result = new IntermediateTable<T, Variant, Alignment>(projectionParameters.size(), table.end()->getPos());

    // Iterate over given table tuples
    auto scalarIterBegin = table.begin();
    auto scalarIterEnd = table.end();
    auto scalarResultIter = result->end();

    while (*scalarIterBegin != *scalarIterEnd) {
        // Fill tuple with data from every row of table
        for (int j = 0; j < projectionParameters.size(); j++) {
            scalarResultIter->getAddress()[j] = scalarIterBegin->getAddress()[projectionParameters[j]];
        }
        ++(*scalarIterBegin);
        ++(*scalarResultIter);
        result->incrementWriteIterator();
    }
    return result;
}
}  // namespace RowStore
