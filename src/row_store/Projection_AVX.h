#pragma once

#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <vector>

#include "IntermediateTable.h"

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
IntermediateTable_AVX<T> *projection_AVX(IntermediateTable_AVX<T> &table, std::vector<uint64_t> &projectionParameters) {
    
    // TODO : Add Code for projection_AVX() (Copy from Projection.h and adjust to AVX interface)

    // TODO : Change return value back to "result" of type IntermediateTable_AVX
    return nullptr;
}
}  // namespace RowStore
