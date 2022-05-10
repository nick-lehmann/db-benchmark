#pragma once

#include <array>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <cstdarg>
#include <iostream>
#include <stdexcept>

#include "Table.h"
#include "Filter.h"
#include "Projection.h"


namespace RowStore {
    template<typename T>
    class BaseTable : public Table<T> {

    public:
        std::vector<T*> data;

    public:
        // constructor for BaseTable
        BaseTable(unsigned numAttributes, unsigned numRows, const T** initialData)
            : Table<T>(numAttributes, numRows, initialData) {
            
            data = new std::vector<T>[numAttributes];
            for (unsigned row = 0; row < numRows; row++) {
                for (unsigned column = 0; column < numAttributes; column++) {
                    data[column].push_back(initialData[row][column]);
                }
            }
        }


        // destructor for BaseTable
        ~BaseTable() override {
            //delete the tuple of the base table
            for (uint64_t i = 0; i < data.size(); ++i) {
                free(data[i]);
            }
        }

        void addRow(T* row) {
            data.push_back(row);
            this->numberOfRows = data.size();
        }

        T* getRow(unsigned rowIndex) override {
            return data[rowIndex];
        }

        T** query_table(std::vector<unsigned>& projection, std::vector<Filter<T>>& filters) override {
            // TODO
            return nullptr;
        }

        uint64_t query_count(std::vector<unsigned>& projection, std::vector<Filter<T>>& filters) override {
            // TODO
            return -1;
        }

        bool columnIndicesValid(std::vector<int> columnIndices, int tupleSize) {
            // check for each column index if it is in range of 0 and tupleSize
            for (uint64_t i = 0; i < columnIndices.size(); i++) {
                if (columnIndices[i] < 0 || columnIndices[i] >= tupleSize) {
                    return false;
                }
            }
            return true;
        }
        /*
        BaseTable<T>* projection(BaseTable<T>& table, std::vector<int>& projection) {
            if (!columnIndicesValid(projection, table.numberOfAttributes)) {
                throw std::invalid_argument("Error! Invalid column indices!");
            }

            // create empty table
            //TODO result is if type intermediate result, which we need to implement first
            auto result = new BaseTable<T>(projection.size(), true);

            // iterate over given table tuples
            for (int i = 0; i < table.data.size(); i++) {
                // create empty (temporary) tuple
                auto tuple = (T*)calloc(projection.size(), sizeof(T));

                // fill tuple with data from every row of table
                for (int j = 0; j < projection.size(); j++) {
                    tuple[j] = table[i][projection[j]];
                }
                result->addRow(tuple);
            }

            return result;
        }



        BaseTable<T>* filter_basic(BaseTable<T>& table, Filter<T>* predicate) {
            if (predicate->index < 0 || predicate->index >= table.numberOfAttributes) {
                throw std::invalid_argument("Error! Invalid attribute index!");
            }

            //TODO needs to be intermediate table (see above)
            BaseTable<T>* result = new BaseTable<T>(table.numberOfAttributes, 0);
            for (int i = 0; i < table.data.size(); ++i) {
                if (predicate->match(table[i][predicate->index])) {
                    result->addRow(table[i]);
                }
            }
            return result;
        }

        Table<T>* query(std::vector<int>& projectionColumns, std::vector<Filter<T>*>& filters) {
            //TODO: for highest performance do projections as early as possible
            //(AQP WiSe19/20, Lecture 8 Query Processing, Slide 23)

            BaseTable<T>* filterResult = this;
            // apply all filters on the table
            for (int i = 0; i < filters.size(); ++i) {
                auto oldResult = filterResult;
                filterResult = filter_basic(*filterResult, filters[i]);

                if (oldResult != this) {
                    delete oldResult;
                }
            }
            // project to the desired columns
            BaseTable<T>* projectionResult = projection(*filterResult, projectionColumns);
            return projectionResult;
        }*/
    };
}
    /*
template<typename T>
BaseTable<T>* createSortedBaseTable(unsigned numAttributes, unsigned numRows, const T** initialData) {
    BaseTable<T>* table = new BaseTable<T>(numAttributes, numRows, initialData);

    for (int i = 0; i < numRows; ++i) {
        //create empty tuple
        T* tuple = (T*)malloc(numAttributes * sizeof(T));
        // fill tuple with data
        for (int j = 0; j < numAttributes; ++j) {
            tuple[j] = i * numAttributes + j;
        }
        // append tuple to table
        table->addRow(tuple);
    }
    return table;
}
*/