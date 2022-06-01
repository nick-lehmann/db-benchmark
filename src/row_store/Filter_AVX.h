#pragma once
#include <cstdint>
#include <stdexcept>

#include "Filters_AVX.h"
#include "IntermediateTable_AVX.h"

namespace RowStore {

/// Selects these tuples of table that match all Filter-objects in filters and returns the address of the newly created
/// result table that contains exactly the selected tuples
/// \param table intermediateTable that is filtered
/// \param filters vector of filters that are applied conjunctive
template <typename T>
IntermediateTable_AVX<T> *apply_filters_AVX(IntermediateTable_AVX<T> &table, std::vector<Filters::AVX::Filter<T> *> &filters) {
    
    // TODO : Add Code for apply_filters_AVX() (Copy from Filter.h and adjust to AVX interface)

    // TODO : Change return value back to "result" of type IntermediateTable_AVX
    return nullptr;
}
}  // namespace RowStore
