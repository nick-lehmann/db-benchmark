#pragma once

#include <array>
#include <cstdint>
#include <cstdlib>
#include <vector>

#include "Filters.h"

namespace Tables::Scalar {
    /// Unified interface for all basic database tables.
    /// \tparam T the type of stored data
    template<typename T>
    class ITable {
    public:
        const uint64_t numberOfAttributes;
        const uint64_t numberOfRows;
        const T **initialData;

        /// Initializes the table with a given size and initial data
        /// \param numberOfAttributes number of columns
        /// \param numberOfRows number of rows
        /// \param data two-dimensional C-array containing the constant data of the table
        ITable(uint64_t numberOfAttributes, uint64_t numberOfRows, const T **initialData)
                : numberOfAttributes(numberOfAttributes), numberOfRows(numberOfRows), initialData(initialData) {}

        virtual ~ITable() = default;

        /// Returns a table row as one-dimensional C-array of length numberOfAttributes.
        /// \param rowIndex index of the row
        virtual T *getRow(uint64_t rowIndex) = 0;

        T *operator[](uint64_t index) { return this->getRow(index); }

        /// Queries the table and returns a tuple of a 2-dimensional C-array representing the table and its number of rows and columns.
        /// \param projection column indices to project
        /// \param filters filters to apply
        virtual std::tuple<T **, uint64_t, uint64_t>
        queryTable(std::vector<uint64_t> &projection, std::vector<Filters::Scalar::Filter<T> *> &filters) = 0;

        /// Queries the table and returns the amount of rows in the result.
        /// \param projection column indices to project
        /// \param filters filters to apply
        virtual uint64_t
        queryCount(std::vector<uint64_t> &projection, std::vector<Filters::Scalar::Filter<T> *> &filters) = 0;

        /// Prints the entire table.
        void print() {
            for (uint64_t rowIndex = 0; rowIndex < this->numberOfRows; ++rowIndex) {
                auto row = this->getRow(rowIndex);
                for (uint64_t columnIndex = 0; columnIndex < numberOfAttributes; ++columnIndex) {
                    std::cout << row[columnIndex];

                    // append separator (except for last attribute)
                    if (columnIndex < (numberOfAttributes - 1))
                        std::cout << " | ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
    };
}
