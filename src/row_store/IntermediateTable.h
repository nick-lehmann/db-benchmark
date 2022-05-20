#pragma once
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

namespace RowStore {
template <typename T> class IntermediateTable {
private:
  std::vector<T *> data;
  T **tableOutput = nullptr;
  uint64_t tableOutputSize = 0;
  uint32_t tupleWidth = 0;

public:
  /// Initializes the intermediateTable with a given tuple width.
  /// \param tupleWidth number of attributes per tuple
  IntermediateTable(uint32_t tupleWidth) : tupleWidth(tupleWidth) {}

  /// Initializes the intermediateTable with a given tuple width and initial data. The content of the inital data vector
  /// is copies to the table's data storage
  /// \param tupleWidth number of attributes per tuple
  /// \param initData data that is copied into the table
  IntermediateTable(uint32_t tupleWidth, std::vector<T *> &initData) : tupleWidth(tupleWidth) {
    for (uint64_t i = 0; i < initData.size(); ++i) {
      addRowCopy(initData[i]);
    }
  }

  /// Destructor
  ~IntermediateTable() {
    freeTableOutput();
    for (uint64_t i = 0; i < data.size(); ++i) {
      free(data[i]);
    }
  }

  /// Appends a pre-filled tuple to the end of the table
  /// \param row tuple that is appended
  void addRow(T *row) { data.push_back(row); }

  /// Appends an empty tuple to the end of the table and copies the content from row
  /// \param row tuple that's content is copied
  void addRowCopy(T *row) {
    data.push_back(static_cast<T *>(malloc(sizeof(T) * tupleWidth)));
    std::memcpy(data.back(), row, sizeof(T) * tupleWidth);
  }

  /// Deletes a tuple from the table and frees the associated memory
  /// \param index position of the tuple in the data vector
  void removeRow(uint64_t index) {
    T *tmp = data[index];
    data.erase(index);
    free(tmp);
  }

  /// Returns the number of tuples that are contaied in this table
  uint64_t count() {
    return data.size(); // TODO Is this sufficient to avoid the compiler from removing the data?
  }

  /// Returns the number of values in a tuple, aka number of attributes of the table.
  uint32_t getTupleWidth() { return tupleWidth; }

  /// Returns the data of the table.
  std::vector<T *> *getData() { return &data; }

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
  std::tuple<T **, unsigned> table(unsigned outputSize) {
    freeTableOutput();

    tableOutput = static_cast<T **>(malloc(sizeof(T *) * data.size()));
    tableOutputSize = data.size();
    for (uint64_t i = 0; i < data.size(); ++i) {
      tableOutput[i] = static_cast<T *>(malloc(sizeof(T) * tupleWidth));
      std::memcpy(tableOutput[i], data[i], sizeof(T) * tupleWidth);
    }

    outputSize = (unsigned)tableOutputSize;

    return std::make_tuple(tableOutput, outputSize);
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
} // namespace RowStore
