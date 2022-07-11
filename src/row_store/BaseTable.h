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
#include "Filters/All.h"
#include "ITable.h"
#include "intermediateRepresentation/IntermediateTable.h"
//#include "IntermediateTable_AVX.h"
#include "Projection.h"
#include "SIMD.h"

namespace RowStore {

template <typename T, SIMD Variant, int Alignment>
class BaseTable : public Tables::ITable<T, Variant> {
   public:
    std::vector<T *> data;

   public:
    /// Constructor for BaseTable
    /// \param numAttributes number of attributes (columns)
    /// \param numRows number of tuples (rows)
    /// \param initialData data to fill the table with
    BaseTable(unsigned numAttributes, unsigned numRows, const T **initialData)
        : Tables::ITable<T, Variant>(numAttributes, numRows, initialData) {
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
    std::tuple<T **, uint64_t, uint64_t> queryTable(std::vector<uint64_t> &projectionAttributes,
                                                    std::vector<Filters::Filter<T, Variant> *> &filters) override {
        // convert BaseTable to IntermediateTable for querying
        IntermediateTable<T, Variant, Alignment> interTableUnified(this->numberOfAttributes, data);

        // apply filter and projection to intermediate Table (make sure to use the new IntermediateTable-Implementetion)
        auto projectedResult = projection_unified(interTableUnified, projectionAttributes);
        auto filteredResult = apply_filters_unified((*projectedResult), filters);

        // get result as (T **) and create tuple containing the result with the result tables row count and tuple width
        uint64_t numberOfColumns = filteredResult->getTupleWidth();
        auto [table, numberOfRows] = filteredResult->table();

        // free memory
        delete projectedResult;
        delete filteredResult;

        // return result
        return std::make_tuple(table, numberOfRows, numberOfColumns);
    }

    /// Perform a query on the table and return the number of rows of the queried table.
    /// \param projectionAttributes column indices used for projection
    /// \param filters vector of filters used for the query
    uint64_t queryCount(std::vector<uint64_t> &projectionAttributes, std::vector<Filters::Filter<T, Variant> *> &filters) override {
        // convert BaseTable to IntermediateTable for querying
        IntermediateTable<T, Variant, Alignment> interTableUnified(this->numberOfAttributes, data);

        // apply filter and projection to intermediate Table (make sure to use the new IntermediateTable-Implementetion)
        auto projectedResult = projection_unified(interTableUnified, projectionAttributes);
        auto filteredResult = apply_filters_unified((*projectedResult), filters);

        // get result tables row count
        auto resultRowCount = filteredResult->count();

        // free memory
        delete projectedResult;
        delete filteredResult;

        // return result
        return resultRowCount;
    }
};
}  // namespace RowStore
