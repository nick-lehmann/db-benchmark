#pragma once

#include <array>
#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <tuple>
#include <vector>

#include "Filter.h"
#include "Filter_AVX.h"
#include "Filters/All.h"
#include "ITable.h"
#include "IntermediateTable.h"
#include "IntermediateTable_AVX.h"
#include "Projection.h"
#include "SIMD.h"

namespace RowStore {

template <typename T>
class BaseTable : public Tables::ITable<T> {
   public:
    std::vector<T *> data;

   public:
    /// Constructor for BaseTable
    /// \param numAttributes number of attributes (columns)
    /// \param numRows number of tuples (rows)
    /// \param initialData data to fill the table with
    BaseTable(unsigned numAttributes, unsigned numRows, const T **initialData) : Tables::ITable<T>(numAttributes, numRows, initialData) {
        for (unsigned row = 0; row < numRows; row++) {
            data.push_back(static_cast<T *>(malloc(sizeof(T) * numAttributes)));
            for (unsigned column = 0; column < numAttributes; column++) {
                data.back()[column] = initialData[row][column];
            }
        }
    }

    /// Destructor for BaseTable
    ~BaseTable() override {
        // Delete the tuple of the base table
        for (uint64_t i = 0; i < data.size(); ++i) {
            free(data[i]);
        }
    }

    /// Get a certain tuple of the table
    /// \param rowIndex index of tuple in table
    T *getRow(uint64_t rowIndex) override { return data[rowIndex]; }

    /// Perform a query on the table and return a tuple containing the data, the number of rows and the number of columns.
    /// \param projectionAttributes column indices used for projection
    /// \param filters vector of filters used for the query
    /// \param numberOfRows number of rows of the current table
    /// \param numberOfColumns number of columns of the current table
    std::tuple<T **, uint64_t, uint64_t> queryTable(std::vector<uint64_t> &projectionAttributes,
                                                    std::vector<Filters::Filter<T, SIMD::None> *> &filters) override {
        // convert BaseTable to InterMediateTable for query
        IntermediateTable<T> interTable(this->numberOfAttributes, data);
        // apply query
        auto projectedResult = projection(interTable, projectionAttributes);
        auto filteredResult = apply_filters((*projectedResult), filters);
        delete projectedResult;

        // get result and free memory
        unsigned numberOfColumns = filteredResult->getTupleWidth();
        auto [table, numberOfRows] = filteredResult->table();
        filteredResult->detachTableOutput();  // ignore output
        delete filteredResult;

        return std::make_tuple(table, numberOfRows, numberOfColumns);
    }

    /// Perform a query on the table and return a tuple containing the data, the number of rows and the number of columns.
    /// \param projectionAttributes column indices used for projection
    /// \param filters vector of filters used for the query
    /// \param numberOfRows number of rows of the current table
    /// \param numberOfColumns number of columns of the current table
    std::tuple<T **, uint64_t, uint64_t> queryTable(std::vector<uint64_t> &projectionAttributes,
                                                    std::vector<Filters::Filter<T, SIMD::AVX512> *> &filters) override {
        // convert BaseTable to InterMediateTable for query
        IntermediateTable_AVX<T> interTableAVX(this->numberOfAttributes, data);
        // apply query
        auto projectedResult = projection_AVX(interTableAVX, projectionAttributes);
        auto filteredResult = apply_filters_AVX((*projectedResult), filters);
        delete projectedResult;

        // get result and free memory
        unsigned numberOfColumns = filteredResult->getTupleWidth();
        auto [table, numberOfRows] = filteredResult->table();
        filteredResult->detachTableOutput();  // ignore output
        delete filteredResult;

        return std::make_tuple(table, numberOfRows, numberOfColumns);
    }

    /// Perform a query on the table and return the number of rows of the queried table.
    /// \param projectionAttributes column indices used for projection
    /// \param filters vector of filters used for the query
    uint64_t queryCount(std::vector<uint64_t> &projectionAttributes, std::vector<Filters::Filter<T, SIMD::None> *> &filters) override {
        // convert BaseTable to InterMediateTable for query
        IntermediateTable<T> interTable(this->numberOfAttributes, data);
        // apply query
        auto projectedResult = projection(interTable, projectionAttributes);
        auto filteredResult = apply_filters((*projectedResult), filters);

        // get row count and free memory
        auto resultRowCount = filteredResult->count();
        delete projectedResult;
        delete filteredResult;

        return resultRowCount;
    }

    /// Perform a query on the table and return the number of rows of the queried table.
    /// \param projectionAttributes column indices used for projection
    /// \param filters vector of filters used for the query
    uint64_t queryCount(std::vector<uint64_t> &projectionAttributes, std::vector<Filters::Filter<T, SIMD::AVX512> *> &filters) override {
        // convert BaseTable to InterMediateTable for query
        IntermediateTable_AVX<T> interTableAVX(this->numberOfAttributes, data);
        // apply query
        auto projectedResult = projection_AVX(interTableAVX, projectionAttributes);
        auto filteredResult = apply_filters_AVX((*projectedResult), filters);

        // get row count and free memory
        auto resultRowCount = filteredResult->count();
        delete projectedResult;
        delete filteredResult;

        return resultRowCount;
    }
};
}  // namespace RowStore
