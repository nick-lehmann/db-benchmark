#pragma once

#include <algorithm>
#include <vector>

#include "Table.h"

namespace ColumnStore {
    template<typename T>
    class ColumnStoreTable : public Table<T> {
    public:
        ColumnStoreTable(unsigned numAttributes, unsigned numRows, const T **initialData)
            : Table<T>(numAttributes, numRows, initialData) {
            data = new std::vector<T>[numAttributes];
            for (unsigned row = 0; row < numRows; row++) {
                for (unsigned column = 0; column < numAttributes; column++) {
                    data[column].push_back(initialData[row][column]);
                }
            }
        }

        ~ColumnStoreTable() override = default;

        T *getRow(unsigned rowIndex) override {
            auto returnRow = new T[this->numberOfAttributes];
            for (unsigned i = 0; i < this->numberOfAttributes; i++) {
                returnRow[i] = data[i][rowIndex];
            }
            return returnRow;
        }

        T **query_table(std::vector<unsigned> &projection, std::vector<Filter<T>> &filters) override {
            // TODO
            return nullptr;
        }

        uint64_t query_count(std::vector<unsigned> &projection, std::vector<Filter<T>> &filters) override {
            // TODO
            return -1;
        }

    public:
        std::vector<T> *data;
    };
}