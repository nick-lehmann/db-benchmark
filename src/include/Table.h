#pragma once

#include <array>
#include <cstdint>
#include <cstdlib>
#include <vector>

#include "Filters.h"

/// Unified interface for all database tables.
/// \tparam T the type of stored data
template <typename T>
class Table {
public:
    /// Initializes the table with a given size and initial data
    /// \param numberOfAttributes number of columns
    /// \param numberOfRows number of rows
    /// \param data two-dimensional C-array containing the constant data of the table
    Table(unsigned numberOfAttributes, unsigned numberOfRows, const T **initialData)
            : numberOfAttributes(numberOfAttributes), numberOfRows(numberOfRows), initialData(initialData) {}

    virtual ~Table() = default;

    /// Returns a table row as one-dimensional C-array of length numberOfAttributes.
    /// \param rowIndex index of the row
    virtual T *getRow(unsigned rowIndex) = 0;

    T *operator[](unsigned index) { return this->getRow(index); }

    /// Queries the table and returns a 2-dimensional C-array representing the table.
    /// \param projection column indices to project
    /// \param filters filters to apply
    virtual T **query_table(std::vector<unsigned> &projection, std::vector<Filter<T>> &filters) = 0;

    /// Queries the table and returns the amount of rows in the result.
    /// \param projection column indices to project
    /// \param filters filters to apply
    virtual uint64_t query_count(std::vector<unsigned> &projection, std::vector<Filter<T>> &filters) = 0;

    /// Prints the entire table.
    void print() {
        for (unsigned rowIndex = 0; rowIndex < this->numberOfRows; ++rowIndex) {
            auto row = this->getRow(rowIndex);
            for (unsigned columnIndex = 0; columnIndex < numberOfAttributes; ++columnIndex) {
                std::cout << row[columnIndex];

                // append separator (except for last attribute)
                if (columnIndex < (numberOfAttributes - 1)) std::cout << " | ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

public:
    const unsigned numberOfAttributes;
    const unsigned numberOfRows;
    const T **initialData;
};
