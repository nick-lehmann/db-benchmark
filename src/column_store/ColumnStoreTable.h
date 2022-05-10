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

        T **query_table(std::vector<unsigned> &projection, std::vector<Filter<T>*> &filters, unsigned& rows,
                        unsigned& columns) override {
            std::vector<unsigned>* filter_indices = nullptr;
            for (const auto& filter : filters) {
                filter_indices = filter_basic(filter, filter_indices);
            }

            columns = (unsigned) projection.size();
            rows = (unsigned) filter_indices->size();

            return reconstruct_table(projection,filter_indices);
        }

        uint64_t query_count(std::vector<unsigned> &projection, std::vector<Filter<T>*> &filters) override {
            std::vector<unsigned>* filter_indices = nullptr;
            for (const auto& filter : filters) {
                filter_indices = filter_basic(filter, filter_indices);
            }

            return (unsigned) filter_indices->size();
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

        T** reconstruct_table(std::vector<unsigned> &projection, std::vector<unsigned>* row_indices) {
            T** to_return = (T**) malloc(row_indices->size() * sizeof(T*));

            for (unsigned row = 0; row < row_indices->size(); row++) {
                T* rowData = (T*) malloc(projection.size() * sizeof(T));
                to_return[row] = rowData;
                for (unsigned projection_index = 0; projection_index < projection.size(); projection_index++) {
                    to_return[row][projection_index] = data[projection.at(projection_index)][row_indices->at(row)];
                }
            }

            return to_return;
        }

    public:
        std::vector<T> *data;
    };
}