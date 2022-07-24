#pragma once

#include <array>
#include <cstdint>
#include <cstdlib>
#include <vector>

#include "Filters/Base.h"
#include "SIMD.h"

namespace Tables {
/// Unified interface for all basic database tables.
/// \tparam T the type of stored data
template <typename T>
class ITable {
    using Dimension = uint64_t;

   public:
    const Dimension numberOfAttributes;
    const Dimension numberOfRows;
    const T **initialData;

    /// Initializes the table with a given size and initial data
    /// \param numberOfAttributes number of columns
    /// \param numberOfRows number of rows
    /// \param data two-dimensional C-array containing the constant data of the table
    ITable(Dimension numberOfAttributes, Dimension numberOfRows, const T **initialData)
        : numberOfAttributes(numberOfAttributes), numberOfRows(numberOfRows), initialData(initialData) {}

    virtual ~ITable() = default;

    /// Returns a table row as one-dimensional C-array of length numberOfAttributes.
    /// \param rowIndex index of the row
    virtual T *getRow(Dimension rowIndex) = 0;

    T *operator[](Dimension index) { return this->getRow(index); }

    /// Queries the table and returns a tuple of a 2-dimensional C-array representing the table and its number of rows and columns.
    /// \param projection column indices to project
    /// \param filters filters to apply
    virtual std::tuple<T **, Dimension, Dimension> queryTable(std::vector<Dimension> &projection,
                                                              std::vector<Filters::Filter<T, SIMD::None> *> &filters) = 0;
    virtual std::tuple<T **, Dimension, Dimension> queryTable(std::vector<Dimension> &projection,
                                                              std::vector<Filters::Filter<T, SIMD::AVX512> *> &filters) = 0;
    virtual std::tuple<T **, Dimension, Dimension> queryTable(std::vector<Dimension> &projection,
                                                              std::vector<Filters::Filter<T, SIMD::AVX512_Strided> *> &filters) = 0;

    /// Queries the table and returns the amount of rows in the result.
    /// \param projection column indices to project
    /// \param filters filters to apply
    virtual Dimension queryCount(std::vector<Dimension> &projection, std::vector<Filters::Filter<T, SIMD::None> *> &filters) = 0;
    virtual Dimension queryCount(std::vector<Dimension> &projection, std::vector<Filters::Filter<T, SIMD::AVX512> *> &filters) = 0;
    virtual Dimension queryCount(std::vector<Dimension> &projection, std::vector<Filters::Filter<T, SIMD::AVX512_Strided> *> &filters) = 0;

    /// Prints the entire table.
    void print() {
        for (Dimension rowIndex = 0; rowIndex < this->numberOfRows; ++rowIndex) {
            auto row = this->getRow(rowIndex);
            for (Dimension columnIndex = 0; columnIndex < numberOfAttributes; ++columnIndex) {
                std::cout << row[columnIndex];

                // append separator (except for last attribute)
                if (columnIndex < (numberOfAttributes - 1)) std::cout << " | ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};
}  // namespace Tables
