#pragma once
#include <cstdint>

namespace RowStore {

/// This Class is used as SuperClass fot the IntermediateTable Iterators.
/// \tparam T base type
/// \tparam V simd variant, decides the internal data layout of the table
/// \tparam A alignment of the data and stridesize (shold be a multiple of sizeof(T))
template <typename T, SIMD V, int A>
class IntermediateIterator {
   protected:
    /// starting address of the IntermediateTables data
    T const *baseAddress;
    /// memory address the iterator is currently pointing to
    T *currentAddress;

    const uint32_t tupleWidth;
    /// tuple index the iterator is pointing to
    uint64_t pos = 0;

   public:
    /// retuns the memory address of the index-th tuple of the table
    /// \param index index of the tuple whose address is requested
    virtual T *addressOf(uint64_t index) = 0;

   public:
    /// Constructor
    IntermediateIterator(T *baseAddress, uint32_t tupleWidth) : baseAddress(baseAddress), tupleWidth(tupleWidth) {}

    /// Increment operator, iterator points to the next tuple or tuple set (in case of teh vector iterator)
    virtual IntermediateIterator<T, V, A> *operator++() = 0;

    /// Value-at operator, returns a reference to the first column of the index-th tuple
    /// \param index index of the tuple whose first elements value is requested
    T &operator[](uint64_t index) { return *addressOf(index); }
    /// Value operator, returns a reference to the first column of the current tuple
    T &operator*() { return *currentAddress; }

    /// Equal operator, returns if two iterator are currently referencing the same index or not
    bool operator==(const IntermediateIterator<T, V, A> &other) { return pos == other.pos; }
    /// Not Equal operator, returns if two iterator are currently referencing different indices or not
    bool operator!=(const IntermediateIterator<T, V, A> &other) { return pos != other.pos; }

    /// Retuns address that is currently referenced by the iterator
    T *getAddress() { return currentAddress; }
    /// Returns the value in the specified column of the index-th tuple
    /// \param index index of the tuple
    /// \param column whose value is requested
    T &getValueAt(uint64_t index, uint32_t column) { return *(addressOf(index) + column); }
    /// Returns the value in the specified column of the currently referenced tuple
    /// \param column whose value is requested
    T &getValue(uint32_t column) { return *(currentAddress + column); }
    /// Returns the index of the currently refernced tuple
    uint64_t getPos() { return pos; }
};
}  // namespace RowStore