#pragma once
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <type_traits>

#include "Constants.h"
#include "SIMD.h"
#include "ScalarIterator.h"
#include "VectorIterator.h"

namespace RowStore {

/// Helper to calculate the required memory depending on the SIMD variant that is used
/// \tparam T base type
/// \tparam V SIMD variant (None, AVX512, AVX512_Strided)
/// \tparam A alignment of the data and stridesize (shold be a multiple of sizeof(T))
template <typename T, SIMD V, int A>
class AllocationHelper {};

template <typename T, int A>
class AllocationHelper<T, SIMD::None, A> {
   public:
    /// Method of the helper that returns the required memory for the specified SIMD variant
    /// \param tupleWidth width of the tuples in the IntermediateTable
    /// \param tupleCapacity maximal number of tuples in the IntermediateTable
    static size_t const getByteSize(uint32_t tupleWidth, size_t tupleCapacity) {
        size_t pageCapacity = PAGE_SIZE / (sizeof(T) * tupleWidth);
        //               number of complete pages         add 1 if the tuple capacity does not fill complete pages
        size_t requiredPages = (tupleCapacity / pageCapacity) + ((tupleCapacity % pageCapacity) != 0);

        return PAGE_SIZE * requiredPages;
    }
};

template <typename T, int A>
class AllocationHelper<T, SIMD::AVX512, A> {
   public:
    static size_t const getByteSize(uint32_t tupleWidth, size_t tupleCapacity) {
        size_t pageCapacity = PAGE_SIZE / (sizeof(T) * tupleWidth);
        //               number of complete pages         add 1 if the tuple capacity does not fill complete pages
        size_t requiredPages = (tupleCapacity / pageCapacity) + ((tupleCapacity % pageCapacity) != 0);

        return PAGE_SIZE * requiredPages;
    }
};

template <typename T, int A>
class AllocationHelper<T, SIMD::AVX512_Strided, A> {
   public:
    static size_t const getByteSize(uint32_t tupleWidth, size_t tupleCapacity) {
        size_t requiredMemory = tupleWidth * tupleCapacity * sizeof(T);

        size_t strideCapacity = A / (tupleWidth * sizeof(T));
        size_t requiredStrides = tupleCapacity / strideCapacity + ((tupleCapacity % strideCapacity) != 0);
        size_t laneCount = VECTOR_BYTE_WIDTH / sizeof(T);
        size_t strideCount = requiredStrides + (laneCount - (requiredStrides % laneCount));

        return strideCount * A;
    }
};

//##########################################################################################################################################

/// This Class is used for data storage and provides Interators for data access
/// \tparam T base type
/// \tparam V simd variant, decides the internal data layout of the table
/// \tparam A alignment of the data and stridesize (should be a multiple of sizeof(T))
template <typename T, SIMD V, int A>
class IntermediateTable {
   public:
    using Type = T;
    static const SIMD Variant = V;
    static const int Alignment = A;

   private:
    size_t tupleWidth;
    size_t capacity;

    T *data;
    ScalarIterator<Type, Variant, Alignment> *writeIterator;

   public:
    void debugPrintData() {
        size_t itemCount = AllocationHelper<T, V, A>::getByteSize(tupleWidth, capacity) / sizeof(T);

        uint32_t column = 0;
        for (uint64_t i = 0; i < itemCount; ++i) {
            if constexpr (static_cast<uint8_t>(V) == static_cast<uint8_t>(SIMD::AVX512_Strided)) {
                if ((i % (A / sizeof(T))) == 0) {
                    std::cout << "\n" << std::endl;
                    column = 0;
                    getchar();
                }
            }
            std::cout << data[i] << " ";
            column++;
            if (column == tupleWidth) {
                column = 0;
                std::cout << std::endl;
            }
        }
    }

    /// Constructor
    IntermediateTable(uint32_t tupleWidth, size_t capacity) : tupleWidth(tupleWidth), capacity(capacity) {
        size_t requiredMemory = tupleWidth * capacity * sizeof(Type);
        size_t allocationSize = AllocationHelper<Type, Variant, Alignment>::getByteSize(tupleWidth, capacity);
        data = static_cast<T *>(std::aligned_alloc(Alignment, allocationSize));

        writeIterator = new ScalarIterator<Type, Variant, Alignment>(data, tupleWidth);
    }

    /// Constructor
    IntermediateTable(uint32_t tupleWidth, std::vector<T *> &initData) : IntermediateTable(tupleWidth, initData.size()) {
        for (T *row : initData) {
            // increment iterator and get address of old iterator position (in memcpy)
            addRow(row);
        }
    }

    /// Destructor, delete write iterator and free allocated memory
    ~IntermediateTable() {
        delete writeIterator;
        free(data);
    }

    /// Returns iterator pointer pointing to the first address of the allocated memory
    IntermediateIterator<Type, Variant, Alignment> *begin() { return new ScalarIterator<Type, Variant, Alignment>(data, tupleWidth); }
    /// Returns vector iterator pointer pointing to the first address of the allocated memory. A vector iterator is used to iterate through
    /// the table and prepare vector registers on demand.
    VectorIterator<Type, Variant, Alignment> *vectorBegin() { return new VectorIterator<Type, Variant, Alignment>(data, tupleWidth); }

    /// Returns iterator pointer pointing to the address one past the last tuple of the table
    IntermediateIterator<Type, Variant, Alignment> *end() {
        return new ScalarIterator<Type, Variant, Alignment>(data, tupleWidth, writeIterator->getPos());
    }
    /// Returns vector iterator pointer pointing to the address one past the last tuple of the table. A vector iterator is used to iterate
    /// through the table and prepare vector registers on demand.
    VectorIterator<Type, Variant, Alignment> *vectorEnd() {
        return new VectorIterator<Type, Variant, Alignment>(data, tupleWidth, writeIterator->getPos() / 8);
    }

    /// Appends a row to the table and increments the writeIterator
    void addRow(T *src) {
        std::memcpy(writeIterator->getAddress(), src, tupleWidth * sizeof(T));
        ++(*writeIterator);
    }

    /// Returns the number of tuples that are contaied in this table
    uint64_t count() {
        // TODO do something fancy counting stuff
        return writeIterator->getPos();
    }

    /// Creates a table data structure of type T** and assignes the number of tuples to outputSize
    std::tuple<T **, unsigned> table() {
        auto iter = begin();
        auto &iterEnd = *writeIterator;

        T **output = static_cast<T **>(malloc(sizeof(T *) * iterEnd.getPos()));
        while (iter != iterEnd) {
            output[iter.getPos()] = static_cast<T *>(malloc(sizeof(T) * tupleWidth));
            std::memcpy(output[iter.getPos()], iter.getAddress(), sizeof(T) * tupleWidth);
            ++iter;
        }

        return std::make_tuple(output, iterEnd.getPos());
    }

    /// Returns the number of values in a tuple, aka number of attributes of the table.
    uint32_t getTupleWidth() { return tupleWidth; }
};
}  // namespace RowStore