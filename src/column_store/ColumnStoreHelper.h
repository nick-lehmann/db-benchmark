#pragma once

#include <immintrin.h>

#include <tuple>
#include <utility>

#include "Constants.h"

// deactivate certain compiler warnings
#pragma GCC diagnostic ignored "-Wignored-attributes"

namespace ColumnStore::Helper {

/// Gathers certain elements from a c-array into a register and returns it together with the indices.
/// \param iSA the C-array of indices to use for the gather operation
/// \param columnStart C-array to select the values from
/// \return
/// pair[0]: loaded values register <br>
/// pair[1]: indices register
template <typename T, typename idxT = T, typename M>
std::pair<__m512i, __m512i> gather(idxT *iSA, T *columnStart, M mask);

template <>
std::pair<__m512i, __m512i> gather(uint32_t *iSA, uint32_t *columnStart, __mmask16 mask) {
    const auto indexRegister = _mm512_set_epi32(iSA[15], iSA[14], iSA[13], iSA[12], iSA[11], iSA[10], iSA[9], iSA[8], iSA[7], iSA[6],
                                                iSA[5], iSA[4], iSA[3], iSA[2], iSA[1], iSA[0]);
    const auto data = _mm512_mask_i32gather_epi32(ONE_REGISTER_32, mask, indexRegister, columnStart, 4);
    return std::make_pair(data, indexRegister);
}

template <>
std::pair<__m512i, __m512i> gather(uint64_t *iSA, uint64_t *columnStart, __mmask16 mask) {
    const auto indexRegister = _mm512_set_epi64(iSA[7], iSA[6], iSA[5], iSA[4], iSA[3], iSA[2], iSA[1], iSA[0]);
    const auto data = _mm512_mask_i64gather_epi64(ONE_REGISTER_64, mask, indexRegister, columnStart, 8);
    return std::make_pair(data, indexRegister);
}

/// Loads a contiguous part of memory into a register and returns it together with a register containing the
/// corresponding indices.
/// \param columnStart the starting address to load into a register
/// \param b
/// index of the first element, increments of this value will be loaded into the register <br>
/// <b>example:</b> b = 0 -> register: | 0 | 1 | 2 | ...
/// \return
/// pair[0]: loaded values register <br>
/// pair[1]: indices register
template <typename T, typename idxT = T>
std::pair<__m512i, __m512i> load(T *columnStart, idxT b);

template <>
std::pair<__m512i, __m512i> load(uint32_t *columnStart, uint32_t b) {
    auto indexRegister =
        _mm512_set_epi32(b + 15, b + 14, b + 13, b + 12, b + 11, b + 10, b + 9, b + 8, b + 7, b + 6, b + 5, b + 4, b + 3, b + 2, b + 1, b);
    return std::make_pair(_mm512_loadu_si512(columnStart), indexRegister);
}

template <>
std::pair<__m512i, __m512i> load(uint64_t *columnStart, uint64_t b) {
    auto indexRegister = _mm512_set_epi64(b + 7, b + 6, b + 5, b + 4, b + 3, b + 2, b + 1, b);
    return std::make_pair(_mm512_loadu_si512(columnStart), indexRegister);
}

/// Stores data from a register in a C-array based on a mask and returns the amount of elements stored.
/// \param indexRegister the indices to store
/// \param mask whether an index should be stored or not
/// \param baseAddr start of the C-array (important: this is an array of index positions)
/// \return amount of stored elements (amount of 1-bits in the mask)
template <typename idxT, typename M>
uint64_t store(__m512i indexRegister, M mask, idxT *baseAddr);

template <>
uint64_t store(__m512i indexRegister, __mmask8 mask, uint64_t *baseAddr) {
    _mm512_mask_compressstoreu_epi64(baseAddr, mask, indexRegister);
    return _mm512_mask_reduce_add_epi64(mask, ONE_REGISTER_64);
}

template <>
uint64_t store(__m512i indexRegister, __mmask16 mask, uint32_t *baseAddr) {
    _mm512_mask_compressstoreu_epi32(baseAddr, mask, indexRegister);
    return _mm512_mask_reduce_add_epi32(mask, ONE_REGISTER_32);
}
}  // namespace ColumnStore::Helper
