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

        T **query_table(std::vector<unsigned> &projection, std::vector<Filter<T>*> &filters) override {
            std::vector<unsigned>* filter_indices = nullptr;
            for (const auto& filter : filters) {
                filter_indices = filter_basic(filter, filter_indices);
            }

            return reconstruct_table(filter_indices);

            // TODO add projection code
        }

        uint64_t query_count(std::vector<unsigned> &projection, std::vector<Filter<T>*> &filters) override {
            // TODO
            return -1;
        }

    private:
        std::vector<unsigned>* filter_basic(const Filter<T>* filter, std::vector<unsigned>* input_indices = nullptr) {
            auto output_indices = new std::vector<unsigned>();
            auto filter_col = data[filter->index];

            if (input_indices) {
                for (const auto& row : *input_indices) {
                    if(filter->match(filter_col.at(row)))
                        output_indices->push_back(row);
                }
            } else {
                for (unsigned row = 0; row < this->numberOfRows; row++) {
                    if(filter->match(filter_col.at(row)))
                        output_indices->push_back(row);
                }
            }

            free(input_indices);
            return output_indices;
        }

        std::vector<unsigned>* projection_basic() {
            return nullptr;
        }

        T** reconstruct_table(std::vector<unsigned>* row_indices /* TODO projection (column indices) */) {
            T** to_return = (T**) malloc(row_indices->size() * sizeof(T*));

            for (unsigned row = 0; row < row_indices->size(); row++) {
                // TODO add projection from here
                T* rowData = (T*) malloc(this->numberOfAttributes * sizeof(T));
                to_return[row] = rowData;
                for (unsigned column = 0; column < this->numberOfAttributes; column++) {
                    to_return[row][column] = data[column][row];
                }
            }

            return to_return;
        }

    public:
        std::vector<T> *data;
    };
}