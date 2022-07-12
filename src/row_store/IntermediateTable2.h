#pragma once
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#include "Constants.h"

namespace RowStore {

// TODO this is meant to implement an more complex (lane)-pages, round-robin, tuple allocation algortithm that enables better
// gather-performance
template <typename T>
class IntermediateTable2Iterator {
   private:
    T const *baseAddress;
    const size_t tupleSize;
    const size_t pageCapacity;

    int32_t skipPageFootCounter;  // counts down the number of tuples till the
    const size_t pageFootSize;

   public:
    IntermediateTableIterator(T *baseAddress, size_t tupleSize, size_t pageCapacity)
        : baseAddress(baseAddress), tupleSize(tupleSize), pageCapacity(pageCapacity) {
        skipPageFootCounter =
    }
};

template <typename T>
class IntermediateTable2 {
   private:
    size_t capacity;
    size_t pageCapacity;
    size_t rowCount = 0;
    size_t allocatedPages;
    T *const data;

    T **tableOutput = nullptr;
    unsigned tableOutputSize = 0;
    uint32_t tupleWidth = 0;

   public:
    /// Initializes the intermediateTable with a given tuple width.
    /// \param tupleWidth number of attributes per tuple
    IntermediateTable(uint32_t tupleWidth, size_t tupleCapacity) : tupleWidth(tupleWidth), capacity(tupleCapacity) {
        pageCapacity = PAGE_SIZE / (sizeof(T) * tupleWidth);
        //---------------number of complete pages-------add 1 if the tuple capacity does not fill complete pages
        allocatedPages = (tupleCapacity / pageCapacity) + ((tupleCapacity % pageCapacity) != 0);

        // allloc memory that is aligned at the page boundaries
        data = alligned_alloc(PAGE_SIZE, allocatedPages * PAGE_SIZE);
    }

    /// Initializes the intermediateTable with a given tuple width and initial data. The content of the inital data vector
    /// is copies to the table's data storage
    /// \param tupleWidth number of attributes per tuple
    /// \param initData data that is copied into the table
    /*IntermediateTable(uint32_t tupleWidth, std::vector<T *> &initData) : tupleWidth(tupleWidth) {
        for (uint64_t i = 0; i < initData.size(); ++i) {
            addRowCopy(initData[i]);
        }
    }*/

    /// Destructor
    ~IntermediateTable() {
        freeTableOutput();
        free(data);
    }

    /// Calculates the first free address in the allocated memory of the IntermediateTable
    /// \return Free address that can be used to write data at
    T *getFreeAddress() {
        return getAddress(rowCount);  // get address of tuple one past last wrtitten tuple
    }

    /// Calculates the address of a tuple specified by its tuple
    /// \param index position of a tuple in the table
    /// \return Address of the tuple at the index
    T *getAddress(uint64_t index) {
        return data + ((index / pageCapacity) * PAGE_SIZE) +  // skip pages that dont contain the index
               (index % pageCapacity);                        // offset inside the page that contins the index
    }

    /// Copies the content from row to the allocated storage
    /// \param row tuple that's content is copied
    void addRow(T *row) {
        std::memcpy(getFreeAddress(), row, sizeof(T) * tupleWidth);
        ++rowCount;
    }

    /// Copies the content from multiple rows laying end by end to the allocated storage
    /// \param rows tuples that's content is copied
    void addRows(T *rows, size_t numberOfRows) {
        size_t leftPageCapacity = pageCapacity - (rowCount % pageCapacity);

        while (numberOfRows > 0) {
            std::memcpy(getFreeAddress(), row, sizeof(T) * tupleWidth * leftPageCapacity);
            rowCount += leftPageCapacity;
            numberOfRows -= leftPageCapacity;
            leftPageCapacity = pageCapacity;
        }
    }

    /// Returns the number of tuples that are contaied in this table
    uint64_t count() {
        // TODO do something fancy counting stuff
    }

    /// Returns the number of values in a tuple, aka number of attributes of the table.
    uint32_t getTupleWidth() { return tupleWidth; }

    /// Frees tableOutput if it already exists and resets tableOutputSize
    void freeTableOutput() {
        if (tableOutput) {
            for (uint64_t i = 0; i < data.size(); ++i) {
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

        tableOutput = static_cast<T **>(malloc(sizeof(T *) * data.size()));
        tableOutputSize = data.size();
        for (uint64_t i = 0; i < data.size(); ++i) {
            tableOutput[i] = static_cast<T *>(malloc(sizeof(T) * tupleWidth));
            std::memcpy(tableOutput[i], data[i], sizeof(T) * tupleWidth);
        }

        return std::make_tuple(tableOutput, tableOutputSize);
    }

    /// Detaches the table data structure of type T** from this IntermediateTable-Object and returns its address.
    /// That means that this datastructure is not freed if the table is deleted.
    /// \param outputSize number of tuple in the output
    T **detachTableOutput(unsigned &outputSize) {
        T **tmp = tableOutput;
        tableOutput = nullptr;
        outputSize = tableOutputSize;
        tableOutputSize = 0;
        return tmp;
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
        for (uint64_t row = 0; row < data.size(); ++row) {
            for (uint32_t col = 0; col < tupleWidth; ++col) {
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
