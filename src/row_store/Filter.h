#pragma once
#include <cstdint>
#include <stdexcept>

#include "Filters/All.h"
#include "SIMD.h"
#include "intermediateRepresentation/IntermediateTable.h"

namespace RowStore {

template <typename T, SIMD Variant, int Alignment>
class TupleCopyHelper;

template <typename T, int Alignment>
class TupleCopyHelperScalar {
   public:
    static void copyMaskedTupleN(IntermediateTable<T, SIMD::None, Alignment> *result,
                                 IntermediateIterator<T, SIMD::None, Alignment> &scalarIterBegin, bool mask) {
        if (!mask) {
            return;
        } else {
            result->addRow(scalarIterBegin.addressOf(scalarIterBegin.getPos()));
        }
    }
};

template <SIMD Variant, int Alignment>
class TupleCopyHelper<uint32_t, Variant, Alignment> {
   public:
    static void copyMaskedTupleN(IntermediateTable<uint32_t, Variant, Alignment> *result,
                                 IntermediateIterator<uint32_t, Variant, Alignment> &scalarIterBegin, uint16_t mask) {
        if (!mask) return;

        if (mask & 0b1000000000000000) {
            result->addRow(scalarIterBegin.addressOf(scalarIterBegin.getPos() + 0));
        }
        if (mask & 0b0100000000000000) {
            result->addRow(scalarIterBegin.addressOf(scalarIterBegin.getPos() + 1));
        }
        if (mask & 0b0010000000000000) {
            result->addRow(scalarIterBegin.addressOf(scalarIterBegin.getPos() + 2));
        }
        if (mask & 0b0001000000000000) {
            result->addRow(scalarIterBegin.addressOf(scalarIterBegin.getPos() + 3));
        }
        if (mask & 0b0000100000000000) {
            result->addRow(scalarIterBegin.addressOf(scalarIterBegin.getPos() + 4));
        }
        if (mask & 0b0000010000000000) {
            result->addRow(scalarIterBegin.addressOf(scalarIterBegin.getPos() + 5));
        }
        if (mask & 0b0000001000000000) {
            result->addRow(scalarIterBegin.addressOf(scalarIterBegin.getPos() + 6));
        }
        if (mask & 0b0000000100000000) {
            result->addRow(scalarIterBegin.addressOf(scalarIterBegin.getPos() + 7));
        }
        if (mask & 0b0000000010000000) {
            result->addRow(scalarIterBegin.addressOf(scalarIterBegin.getPos() + 8));
        }
        if (mask & 0b0000000001000000) {
            result->addRow(scalarIterBegin.addressOf(scalarIterBegin.getPos() + 9));
        }
        if (mask & 0b0000000000100000) {
            result->addRow(scalarIterBegin.addressOf(scalarIterBegin.getPos() + 10));
        }
        if (mask & 0b0000000000010000) {
            result->addRow(scalarIterBegin.addressOf(scalarIterBegin.getPos() + 11));
        }
        if (mask & 0b0000000000001000) {
            result->addRow(scalarIterBegin.addressOf(scalarIterBegin.getPos() + 12));
        }
        if (mask & 0b0000000000000100) {
            result->addRow(scalarIterBegin.addressOf(scalarIterBegin.getPos() + 13));
        }
        if (mask & 0b0000000000000010) {
            result->addRow(scalarIterBegin.addressOf(scalarIterBegin.getPos() + 14));
        }
        if (mask & 0b0000000000000001) {
            result->addRow(scalarIterBegin.addressOf(scalarIterBegin.getPos() + 15));
        }
    }
};

template <SIMD Variant, int Alignment>
class TupleCopyHelper<uint64_t, Variant, Alignment> {
   public:
    static void copyMaskedTupleN(IntermediateTable<uint64_t, Variant, Alignment> *result,
                                 IntermediateIterator<uint64_t, Variant, Alignment> &scalarIterBegin, uint16_t mask) {
        if (!mask) return;

        if (mask & 0b10000000) {
            result->addRow(scalarIterBegin.addressOf(scalarIterBegin.getPos() + 0));
        }
        if (mask & 0b01000000) {
            result->addRow(scalarIterBegin.addressOf(scalarIterBegin.getPos() + 1));
        }
        if (mask & 0b00100000) {
            result->addRow(scalarIterBegin.addressOf(scalarIterBegin.getPos() + 2));
        }
        if (mask & 0b00010000) {
            result->addRow(scalarIterBegin.addressOf(scalarIterBegin.getPos() + 3));
        }
        if (mask & 0b00001000) {
            result->addRow(scalarIterBegin.addressOf(scalarIterBegin.getPos() + 4));
        }
        if (mask & 0b00000100) {
            result->addRow(scalarIterBegin.addressOf(scalarIterBegin.getPos() + 5));
        }
        if (mask & 0b00000010) {
            result->addRow(scalarIterBegin.addressOf(scalarIterBegin.getPos() + 6));
        }
        if (mask & 0b00000001) {
            result->addRow(scalarIterBegin.addressOf(scalarIterBegin.getPos() + 7));
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
                filteringMask = filters[j]->match(value) && filteringMask;
            }
            // copy tuples to result
            auto scalarIter = vectorIterBegin->getScalarIterator();
            TupleCopyHelperScalar<T, Alignment>::copyMaskedTupleN(result, *scalarIter, filteringMask);
        } else {
            __mmask16 filteringMask = ONE_MASK;
            // Iterate over filters and match
            for (int j = 0; j < filters.size(); ++j) {
                auto vectorReg = vectorIterBegin->gather(filters[j]->index);
                filteringMask = filters[j]->match(vectorReg, filteringMask);
            }
            // copy tuples to result
            auto scalarIter = vectorIterBegin->getScalarIterator();
            TupleCopyHelper<T, Variant, Alignment>::copyMaskedTupleN(result, *scalarIter, _mm512_mask2int(filteringMask));
        }

        ++(*vectorIterBegin);
        ++(*vectorResultIter);
    }
    return result;
}
}  // namespace RowStore