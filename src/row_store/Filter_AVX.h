#pragma once
#include <cstdint>
#include <stdexcept>

#include "Constants.h"
#include "Filters/All.h"
#include "IntermediateTable_AVX.h"
#include "SIMD.h"
#include "copy_tuple_codegen/copyMaskedTuple.h"

namespace RowStore {

/// Filter_AVXHelper is a helper class that is responsible for gathering data from an IntermediateTable_AVX to AVX registers.
/// Once initialized it holds a register containing the virtual indices sufficent for gathering.
/// \tparam T base type of the Intermediate_AVX table this class should operate on.
template <typename T>
class Filter_AVXHelper {};

template <>
class Filter_AVXHelper<uint64_t> {
   private:
    /// Virtual index (relative to base address) used for gathering
    __m512i vindexReg;
    const unsigned tupleWidth;

   public:
    Filter_AVXHelper(unsigned tupleWidth) : tupleWidth(tupleWidth) {
        vindexReg = _mm512_set_epi64(0 * tupleWidth, 1 * tupleWidth, 2 * tupleWidth, 3 * tupleWidth, 4 * tupleWidth, 5 * tupleWidth,
                                     6 * tupleWidth, 7 * tupleWidth);
    }

    /// Gathers 64bit-integer from 8 consecutive row in the baseAddress storage. Their exact position within the baseAddress storage is
    /// determined by rowIndex and columnIndex.
    /// \param baseAddress base address of the data storage
    /// \param rowIndex index of the first row (of the 8 consecutive rows)
    /// \param columnIndex index of the column that contains the integers
    __m512i gather(uint64_t *baseAddress, uint64_t rowIndex, uint32_t columnIndex) {
        return _mm512_i64gather_epi64(vindexReg, baseAddress + (rowIndex * tupleWidth) + columnIndex, 8);
    }
};

template <>
class Filter_AVXHelper<uint32_t> {
   private:
    __m512i vindexReg;
    const unsigned tupleWidth;

   public:
    Filter_AVXHelper(unsigned tupleWidth) : tupleWidth(tupleWidth) {
        vindexReg = _mm512_set_epi32(0 * tupleWidth, 1 * tupleWidth, 2 * tupleWidth, 3 * tupleWidth, 4 * tupleWidth, 5 * tupleWidth,
                                     6 * tupleWidth, 7 * tupleWidth, 8 * tupleWidth, 9 * tupleWidth, 10 * tupleWidth, 11 * tupleWidth,
                                     12 * tupleWidth, 13 * tupleWidth, 14 * tupleWidth, 15 * tupleWidth);
    }

    /// Gathers 32bit-integer from 16 consecutive rows in the baseAddress storage. Their exact position within the baseAddress storage is
    /// determined by rowIndex and columnIndex.
    /// \param baseAddress base address of the data storage
    /// \param rowIndex index of the first row (of the 16 consecutive rows)
    /// \param columnIndex index of the column that contains the integers
    __m512i gather(uint32_t *baseAddress, uint64_t rowIndex, uint32_t columnIndex) {
        return _mm512_i32gather_epi32(vindexReg, baseAddress + (rowIndex * tupleWidth) + columnIndex, 4);
    }
};

/// Selects those tuples of table that match all Filter-objects in filters and returns the address of the newly created
/// result table that contains exactly the selected tuples
/// \param table intermediateTable that is filtered
/// \param filters vector of filters that are applied in conjunction
template <typename T>
IntermediateTable_AVX<T> *apply_filters_AVX(IntermediateTable_AVX<T> &table, std::vector<Filters::Filter<T, SIMD::AVX512> *> &filters) {
    // Validate filters
    for (int i = 0; i < filters.size(); ++i) {
        if (filters[i]->index < 0 || filters[i]->index >= table.getTupleWidth()) {
            throw std::invalid_argument("Error! Invalid attribute index!");
        }
    }

    // Create empty intermediate table
    auto result = new IntermediateTable_AVX<T>(table.getTupleWidth(), table.getRowCount());
    Filter_AVXHelper<T> helper(table.getTupleWidth());

    // skip as many integers as fit in one vector
    for (int row = 0; row < table.count(); row += VECTOR_BYTE_WIDTH / sizeof(T)) {
        __mmask16 filteringMask = ONE_MASK;
        for (int filterIdx = 0; filterIdx < filters.size(); ++filterIdx) {
            // TODO maybe it is more efficient to add if (only load data if column changed)
            __m512i reg = helper.gather(table.getData(), row, filters[filterIdx]->index);
            // Match with AVX-Filter while applying mask from previous filter application
            filteringMask = filters[filterIdx]->match(reg, filteringMask);
        }

        result->manipulateRowCount(copyMaskedTuple(result->getDataEnd(), table.getData() + (row * table.getTupleWidth()),
                                                   table.getTupleWidth(), _mm512_mask2int(filteringMask)));
    }

    return result;
}

}  // namespace RowStore
