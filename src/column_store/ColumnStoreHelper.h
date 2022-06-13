#pragma once

#include <tuple>
#include <immintrin.h>

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
    template<typename T>
    std::pair<__m512i, __m512i> gather(uint64_t *iSA, T *columnStart);

    template<>
    std::pair<__m512i, __m512i> gather(uint64_t *iSA, uint32_t *columnStart) {
        auto indexRegister = _mm512_set_epi32(iSA[15], iSA[14], iSA[13], iSA[12], iSA[11], iSA[10], iSA[9], iSA[8],
                                              iSA[7], iSA[6], iSA[5], iSA[4], iSA[3], iSA[2], iSA[1], iSA[0]);
        return std::make_pair(_mm512_i32gather_epi32(indexRegister, columnStart, 4), indexRegister);
    }

    template<>
    std::pair<__m512i, __m512i> gather(uint64_t *iSA, uint64_t *columnStart) {
        auto indexRegister = _mm512_set_epi64(iSA[7], iSA[6], iSA[5], iSA[4], iSA[3], iSA[2], iSA[1], iSA[0]);
        return std::make_pair(_mm512_i64gather_epi64(indexRegister, columnStart, 8), indexRegister);
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
    template<typename T>
    std::pair<__m512i, __m512i> load(T *columnStart, uint64_t b);

    template<>
    std::pair<__m512i, __m512i> load(uint32_t *columnStart, uint64_t b) {
        auto indexRegister = _mm512_set_epi32(b + 15, b + 14, b + 13, b + 12, b + 11, b + 10, b + 9, b + 8, b + 7,
                                              b + 6, b + 5, b + 4, b + 3, b + 2, b + 1, b);
        return std::make_pair(_mm512_loadu_si512(columnStart), indexRegister);
    }

    template<>
    std::pair<__m512i, __m512i> load(uint64_t *columnStart, uint64_t b) {
        auto indexRegister = _mm512_set_epi64(b + 7, b + 6, b + 5, b + 4, b + 3, b + 2, b + 1, b);
        return std::make_pair(_mm512_loadu_si512(columnStart), indexRegister);
    }

    /// Stores data from a register in a C-array based on a mask and returns the amount of elements stored.
    /// \param indexRegister the indices to store
    /// \param mask whether an index should be stored or not
    /// \param baseAddr start of the C-array
    /// \return amount of stored elements (amount of 1-bits in the mask)
    uint64_t store(__m512i indexRegister, __mmask16 mask, uint64_t *baseAddr) {
        // FIXME
        // this method currently only works for 64-bit integers since for others since the amount of data would
        // otherwise exceed the amount of indices (indices are uint64_t, meaning a maximum of 8 in an AVX-512 register,
        // with a 32-bit data type in the table there would be 16 elements in a register)

        _mm512_mask_compressstoreu_epi64(baseAddr, mask, indexRegister);
        return _mm512_mask_reduce_add_epi64(mask, ONE_REGISTER_64);
    }
}