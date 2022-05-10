#pragma once

#include <cstdlib>

namespace TableHelper {

    /// Generates a two-dimensional C-array of random numbers. The array is in row first format.
    /// \tparam T type of the data
    /// \param numberOfColumns number of columns
    /// \param numberOfRows number of rows
    /// \param lowerBound lower bound for the random values
    /// \param upperBound upper bound for the random values
    /// \param seed seed of the random generator, default = 0
    template<typename T>
    const T** generateRandomData(unsigned numberOfColumns, unsigned numberOfRows, T lowerBound, T upperBound,
                           unsigned seed = 0) {
        std::srand(seed);
        T** data = (T**) malloc(numberOfRows * sizeof(T*));

        for (unsigned row = 0; row < numberOfRows; row++) {
            T* rowData = (T*) malloc(numberOfColumns * sizeof(T));
            data[row] = rowData;
            for (unsigned column = 0; column < numberOfColumns; column++) {
                data[row][column] = std::rand() % (upperBound - lowerBound + 1) + lowerBound;
            }
        }

        return const_cast<const T**>(data);
    }

    /// Prints a two-dimensional C-array as a table.
    /// \tparam T type of the data
    /// \param table table to print
    /// \param numberOfColumns number of columns
    /// \param numberOfRows number of rows
    template<typename T>
    void printTable(T** table, unsigned numberOfColumns, unsigned numberOfRows) {
        for (unsigned row = 0; row < numberOfRows; row++) {
            for (unsigned column = 0; column < numberOfColumns; column++) {
                std::cout << table[row][column];

                if (column < (numberOfColumns - 1)) std::cout << " | ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}