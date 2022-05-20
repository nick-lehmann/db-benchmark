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

        std::tuple<T **, unsigned, unsigned>
        query_table(std::vector<unsigned> &projection, std::vector<Filter<T> *> &filters) override {
            std::vector<unsigned> *filter_indices = nullptr;
            for (const auto &filter: filters) {
                filter_indices = filter_basic(filter, filter_indices);
            }

            return std::make_tuple(reconstruct_table(projection, filter_indices),
                                   (unsigned) filter_indices->size(), (unsigned) projection.size());
        }

        uint64_t query_count(std::vector<unsigned> &projection, std::vector<Filter<T> *> &filters) override {
            // the first list of filtered indices is empty
            std::vector<unsigned> *filter_indices = nullptr;

            // apply each filter, (hopefully) reducing the index vector each iteration
            for (const auto &filter: filters) {
                filter_indices = filter_basic(filter, filter_indices);
            }

            // accessing a portion of the memory (if possible) to avoid compiler optimizations
            auto final_result = reconstruct_table(projection, filter_indices);
            if (!filter_indices->empty()) {
                auto access = final_result[filter_indices->size() - 1][projection.size() - 1];
            }

            return (unsigned) filter_indices->size();
        }

    private:
        /// Applies a single filter on the stored data based on a vector of input indices from previous filtering.
        /// \param filter the filter to apply
        /// \param input_indices the vector of indices that are still in the filtering. This can be a nullptr in which case the entire set of data is accessed.
        /// \return a (reduced) vector of indices for rows that match this filter
        std::vector<unsigned> *filter_basic(const Filter<T> *filter, std::vector<unsigned> *input_indices = nullptr) {
            auto output_indices = new std::vector<unsigned>();
            auto filter_col = data[filter->index];

            if (input_indices) { // input_indices is not empty
                for (const auto &row: *input_indices) {
                    if (filter->match(filter_col.at(row)))
                        output_indices->push_back(row);
                }
            } else { // input_indices is empty, iterate over the entire set of data
                for (unsigned row = 0; row < this->numberOfRows; row++) {
                    if (filter->match(filter_col.at(row)))
                        output_indices->push_back(row);
                }
            }

            // free the old input_indices memory
            free(input_indices);
            return output_indices;
        }

        /// Constructs and returns a table (2-dimensional C-array) based on prior filtering and using projection.
        /// \param projection the columns to pack into the table
        /// \param row_indices the rows to pack into the table
        T **reconstruct_table(std::vector<unsigned> &projection, std::vector<unsigned> *row_indices) {
            T **to_return = (T **) malloc(row_indices->size() * sizeof(T *));

            for (unsigned row = 0; row < row_indices->size(); row++) {
                T *rowData = (T *) malloc(projection.size() * sizeof(T));
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