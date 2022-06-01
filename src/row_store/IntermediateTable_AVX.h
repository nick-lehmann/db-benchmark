#pragma once
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#include "Constants.h"

namespace RowStore {

template <typename T>
class IntermediateTable_AVX {
   private:
    size_t capacity;
    size_t rowCount = 0;
    size_t allocatedPages;
    T *data;

    T **tableOutput = nullptr;
    unsigned tableOutputSize = 0;
    uint32_t tupleWidth = 0;

   public:
    /// Same functional idea as IntermediateTable but adapted to the AVX interface
    /// Initializes the IntermediateTable_AVX with a given tuple width.
    /// \param tupleWidth number of attributes per tuple
    IntermediateTable_AVX(uint32_t tupleWidth, size_t tupleCapacity) : tupleWidth(tupleWidth), capacity(tupleCapacity) {
        size_t pageCapacity = PAGE_SIZE / (sizeof(T) * tupleCapacity);
        //               number of complete pages         add 1 if the tuple capacity does not fill complete pages
        allocatedPages = (tupleCapacity / pageCapacity) + ((tupleCapacity % pageCapacity) != 0);

        // allloc memory that is aligned at the page boundaries
        data = static_cast<T *>(aligned_alloc(PAGE_SIZE, allocatedPages * PAGE_SIZE));
    }

    /// Initializes the IntermediateTable_AVX with a given tuple width and initial data. The content of the inital data vector
    /// is copied to the table's data storage
    /// \param tupleWidth number of attributes per tuple
    /// \param initData data that is copied into the table
    IntermediateTable_AVX(uint32_t tupleWidth, std::vector<T *> &initData) : IntermediateTable_AVX(tupleWidth, initData.size()) {
        // the constructor above handles all the stroage allocation
        for (uint64_t i = 0; i < initData.size(); ++i) {
            addRow(initData[i]);
        }
    }

    /// Destructor
    ~IntermediateTable_AVX() {
        freeTableOutput();
        free(data);
    }

    /// Copies the content from row to the allocated storage
    /// \param row tuple that's content is copied
    void addRow(T *row) { std::memcpy(getTuple(rowCount++), row, sizeof(T) * tupleWidth); }

    /// Copies the content from multiple rows laying end by end to the allocated storage
    /// \param rows tuples that's content is copied
    void addRows(T *rows, size_t numberOfRows) {
        std::memcpy(getTuple(rowCount), rows, sizeof(T) * tupleWidth * numberOfRows);
        rowCount += numberOfRows;
    }

    /// Returns the number of tuples that are contaied in this table
    uint64_t count() {
        uint64_t count = 0;
        for (uint64_t i = 0; i < rowCount; ++i) {
            T tmp = data[i];
            ++count;
        }
        return count;
    }

    /// Calculates the address of a tuple specified by its position in the table
    /// \param index position of a tuple in the table
    /// \return Address of the tuple at the index
    T *operator[](uint64_t index) { return data + (index * tupleWidth); }

    /// Calculates the address of a tuple specified by its position in the table
    /// \param index position of a tuple in the table
    /// \return Address of the tuple at the index
    T *getTuple(uint64_t index) { return data + (index * tupleWidth); }

    /// Returns the number of values in a tuple, aka number of attributes of the table.
    uint32_t getTupleWidth() { return tupleWidth; }

    /// Returns the number of rows of the table.
    size_t getRowCount() { return rowCount; }

    /// Frees tableOutput if it already exists and resets tableOutputSize
    void freeTableOutput() {
        if (tableOutput) {
            for (uint64_t i = 0; i < tableOutputSize; ++i) {
                free(tableOutput[i]);
            }
            free(tableOutput);
            tableOutput = nullptr;
            tableOutputSize = 0;
        }
    }

    /// Creates a table data structure of type T** and assignes the number of tuples to outputSize
    /// \param outputSize number of tuple in the output
    std::tuple<T **, unsigned> table() {
        freeTableOutput();

        tableOutput = static_cast<T **>(malloc(sizeof(T *) * rowCount));
        tableOutputSize = rowCount;
        for (uint64_t i = 0; i < rowCount; ++i) {
            tableOutput[i] = static_cast<T *>(malloc(sizeof(T) * tupleWidth));
            std::memcpy(tableOutput[i], getTuple(i), sizeof(T) * tupleWidth);
        }

        return std::make_tuple(tableOutput, tableOutputSize);
    }

    /// Detaches the table data structure of type T** from this IntermediateTable-Object and returns its address.
    /// That means that this datastructure is not freed if the table is deleted.
    /// \param outputSize number of tuple in the output
    std::tuple<T **, unsigned> detachTableOutput() {
        T **tmp = tableOutput;
        tableOutput = nullptr;
        unsigned tmpSize = tableOutputSize;
        tableOutputSize = 0;
        return std::make_tuple(tmp, tmpSize);
    }

    /// Frees the memory that is accociated to tableOutput.
    /// \param tableOutput data structure that is freed
    /// \param outputSize number of tuple in tableOutput
    static void deleteDetachedTableOutput(T **tableOutput, unsigned outputSize) {
        if (tableOutput) {
            for (unsigned i = 0; i < outputSize; ++i) {
                free(tableOutput[i]);
            }
            free(tableOutput);
        }
    }

    /// Prints the intermediateTable to stdout
    void printTableOutput() {
        for (uint64_t row = 0; row < rowCount; ++row) {
            for (uint32_t col = 0; col < tupleWidth; ++col) {
                // Prepend a column separator except for the first column
                if (col) {
                    std::cout << " | ";
                }
                std::cout << getTuple(row)[col];
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    /// Prints a table data structure of type T** to stdout
    /// \param tableOutput data structure that printed
    /// \param outputSize number of tuple in the output
    static void printTableOutput(T **data, unsigned numberOfRows, unsigned numberOfColumns) {
        for (unsigned row = 0; row < numberOfRows; ++row) {
            for (unsigned col = 0; col < numberOfColumns; ++col) {
                // Prepend a column separator except for the first column
                if (col) {
                    std::cout << " | ";
                }
                std::cout << data[row][col];
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};

}  // namespace RowStore
