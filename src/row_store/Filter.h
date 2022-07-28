#pragma once
#include <cstdint>
#include <stdexcept>

#include "Filters/All.h"
#include "SIMD.h"
#include "intermediateRepresentation/IntermediateTable.h"

namespace RowStore {

template <typename T, int Alignment>
class TupleCopyHelperScalar {
   public:
    static void copyMaskedTupleN(IntermediateTable<T, SIMD::None, Alignment> *result,
                                 IntermediateIterator<T, SIMD::None, Alignment> &scalarIterBegin, bool mask) {
        if (mask) {
            result->addRow(scalarIterBegin.addressOf(scalarIterBegin.getPos()));
        }
    }
};

template <typename T, SIMD Variant, int Alignment>
class TupleCopyHelper;

template <SIMD Variant, int Alignment>
class TupleCopyHelper<uint32_t, Variant, Alignment> {
   public:
    static void copyMaskedTupleN(IntermediateTable<uint32_t, Variant, Alignment> *result,
                                 IntermediateIterator<uint32_t, Variant, Alignment> &scalarIterBegin, __mmask16 mask) {
        if (!mask) return;

        for (int i = 0; i < 16; ++i) {
            if ((mask >> (15 - i)) & 1) {
                result->addRow(scalarIterBegin.addressOf(scalarIterBegin.getPos() + i));
            }
        }
    }
};

template <SIMD Variant, int Alignment>
class TupleCopyHelper<uint64_t, Variant, Alignment> {
   public:
    static void copyMaskedTupleN(IntermediateTable<uint64_t, Variant, Alignment> *result,
                                 IntermediateIterator<uint64_t, Variant, Alignment> &scalarIterBegin, __mmask16 mask) {
        if (!mask) return;

        for (int i = 0; i < 8; ++i) {
            if ((mask >> (7 - i)) & 1) {
                result->addRow(scalarIterBegin.addressOf(scalarIterBegin.getPos() + i));
            }
        }
    }
};

/// Selects these tuples of table that match all Filter-objects in filters and returns the address of the newly created
/// result table that contains exactly the selected tuples
/// \param table intermediateTable that is filtered
/// \param filters vector of filters that are applied conjunctive
template <typename T, SIMD Variant, int Alignment>
IntermediateTable<T, Variant, Alignment> *apply_filters_unified(IntermediateTable<T, Variant, Alignment> &table,
                                                                std::vector<Filters::Filter<T, Variant> *> &filters) {
    // Validate filters
    for (int i = 0; i < filters.size(); ++i) {
        if (filters[i]->index < 0 || filters[i]->index >= table.getTupleWidth()) {
            throw std::invalid_argument("Error! Invalid attribute index!");
        }
    }

    // Create empty intermediate table
    auto result = new IntermediateTable<T, Variant, Alignment>(table.getTupleWidth(), table.end()->getPos());

    // Iterate over table rows
    auto vectorIterBegin = table.vectorBegin();
    auto vectorIterEnd = table.vectorEnd();
    auto vectorResultIter = result->vectorEnd();

    //
    while (*vectorIterBegin != *vectorIterEnd) {
        if constexpr (Variant == SIMD::None) {
            bool filteringMask = true;
            // Iterate over filters and match
            for (int j = 0; j < filters.size(); ++j) {
                auto value = vectorIterBegin->gather(filters[j]->index);
                if (!filters[j]->match(value)) {
                    filteringMask = false;
                    break;
                }
            }
            // copy tuples to result
            auto scalarIter = vectorIterBegin->getScalarIterator();
            TupleCopyHelperScalar<T, Alignment>::copyMaskedTupleN(result, *scalarIter, filteringMask);
            delete scalarIter;
        } else {
            __mmask16 filteringMask = ONE_MASK;
            // Iterate over filters and match
            for (int j = 0; j < filters.size(); ++j) {
                auto vectorReg = vectorIterBegin->gather(filters[j]->index);
                filteringMask = filters[j]->match(vectorReg, filteringMask);
                if (!filteringMask) {
                    break;
                }
            }
            // copy tuples to result
            auto scalarIter = vectorIterBegin->getScalarIterator();
            TupleCopyHelper<T, Variant, Alignment>::copyMaskedTupleN(result, *scalarIter, filteringMask);
            delete scalarIter;
        }

        ++(*vectorIterBegin);
        ++(*vectorResultIter);
    }

    // process the left few elements, that do not fill a entire vector (the tables alloc enougth memory to avoid a segementation fault)
    if constexpr (Variant != SIMD::None) {
        uint32_t laneMultiplier = VectorIterator<T, Variant, Alignment>::LaneMultiplier;
        uint32_t leftTuple = table.end()->getPos() % laneMultiplier;
        if (leftTuple != 0) {
            __mmask16 filteringMask = _mm512_int2mask(0b1111111111111111 << (laneMultiplier - leftTuple));
            // Iterate over filters and match
            for (int j = 0; j < filters.size(); ++j) {
                auto vectorReg = vectorIterBegin->gather(filters[j]->index);
                filteringMask = filters[j]->match(vectorReg, filteringMask);
            }
            // copy tuples to result
            auto scalarIter = vectorIterBegin->getScalarIterator();
            TupleCopyHelper<T, Variant, Alignment>::copyMaskedTupleN(result, *scalarIter, _mm512_mask2int(filteringMask));
            delete scalarIter;
        }
    }

    // free memory
    delete vectorIterBegin;
    delete vectorIterEnd;
    delete vectorResultIter;

    return result;
}
}  // namespace RowStore
