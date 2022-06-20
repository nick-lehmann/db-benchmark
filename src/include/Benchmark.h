#include <chrono>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include "../column_store/ColumnStoreTable_AVX.h"
#include "../pax_store/PaxTable_AVX.h"
#include "../row_store/BaseTable_AVX.h"
#include "BenchmarkResult.h"
#include "Helper.h"
#include "Filters/Base.h"
#include "ITable_AVX.h"
//#include "Filters/LessThan.h"

namespace Benchmark {
/// Apply a query using given projection attributes and filters on a table and measure the time.
/// Print the used cpu time and real time in ms, as well as the number of rows of the result table, and returns those
/// results as well.
/// \param table original table to apply the query on
/// \param projection projection attributes for the query
/// \param filters filters to apply for the query
template <typename T>
std::tuple<uint64_t, uint64_t, double> measureTime(Tables::AVX::ITable<T> &table, std::vector<unsigned> &projection,
                                                   std::vector<Filter::Filter<T, SIMD::AVX512> *> &filters, bool enablePrint = true) {
    // measure both cpu time and real time
    auto clockStartTime = std::clock();
    auto realStartTime = std::chrono::steady_clock::now();

    // call query count function
    uint64_t count = table.queryCount(projection, filters);

    auto clockEndTime = std::clock();
    auto realEndTime = std::chrono::steady_clock::now();

    // store results
    uint64_t clockDuration = clockEndTime - clockStartTime;
    double realDuration = std::chrono::duration<double, std::milli>(realEndTime - realStartTime).count();

    // print result
    if (enablePrint) {
        std::cout << std::fixed << std::setprecision(8) << "CPU cycles: " << clockEndTime - clockStartTime << std::endl
                  << "Real time: " << std::chrono::duration<double, std::milli>(realEndTime - realStartTime).count() << " ms" << std::endl
                  << "Row Count: " << count << std::endl;
    }

    return std::make_tuple(count, clockDuration, realDuration);
}

/// Increment the given value by growth factor (linear or exponential growth). Used to increment current row-/columnCount.
/// \param value current value
/// \param exponentialGrowth if true the number of rows/columns grows exponentially, if false linear growth is applied
/// \param growthFactor determines how much value grows. if exponentialGrowth = true growthFactor multiplies current number
/// of rows/columns, else it is added
unsigned incrementValue(unsigned value, bool exponentialGrowth, unsigned growthFactor) {
    if (exponentialGrowth) {
        return value * growthFactor;
    } else {
        return value + growthFactor;
    }
}

/// Run a benchmark for specified table implementation and parameters.
/// \param tableStoreId ID determines which kind of store is used. 0 -> Row-Store, 1-> Column-Store, 2-> PAX-Store
/// \param tableData data to fill table with
/// \param projection projection attributes for the query
/// \param filters filters to apply for the query
/// \param rowCount initial number of rows
/// \param columnCount initial number of columns
/// \param lowerBound lower bound of values stored in table cells
/// \param upperBound upper bound of values stored in table cells
/// \param seed used for data generation
template <typename T>
std::tuple<uint64_t, uint64_t, double> benchmarkTableImplementation(int tableStoreId, std::vector<unsigned> &projectionAttributes,
                                                                    std::vector<Filter::Filter<T, SIMD::AVX512> *> &filters, unsigned rowCount,
                                                                    unsigned columnCount, T lowerBound, T upperBound, unsigned seed) {
    // create data
    const T **tableData = TableHelper::generateRandomData<T>(columnCount, rowCount, lowerBound, upperBound, seed);

    // create table
    switch (tableStoreId) {
        case 0: {
            // row store
            RowStore::BaseTable_AVX<T> table(columnCount, rowCount, tableData);

            // run benchmark and return
            return Benchmark::measureTime(table, projectionAttributes, filters, false);
        }
        case 1: {
            // column store
            ColumnStore::AVX::Table table(columnCount, rowCount, tableData);

            // run benchmark and return
            return Benchmark::measureTime(table, projectionAttributes, filters, false);
        }
        case 2: {
            // pax store
            PaxTableAVX<T> table(columnCount, rowCount, tableData);

            // run benchmark and return
            return Benchmark::measureTime(table, projectionAttributes, filters, false);
        }
        default: {
            throw std::invalid_argument("Invalid table store ID used!");
        }
    }
}

/// Run a benchmark performing multiple time measurements on table with different parameters.
/// \param tableStoreId ID determines which kind of store is used. 0 -> Row-Store, 1-> Column-Store, 2-> PAX-Store
/// \param projection projection attributes for the query
/// \param filters filters to apply for the query
/// \param rowCount initial number of rows
/// \param columnCount initial number of columns
/// \param exponentialGrowth if true the number of rows grows exponentially, if false linear growth is applied
/// \param growthFactor determines how much number of rows grows. if exponentialGrowth = true growthFactor multiplies current number
/// of rows/columns, else it is added
/// \param iterations number of measurements performed. each measurement increments rowCount
/// \param lowerBound lower bound of values stored in table cells
/// \param upperBound upper bound of values stored in table cells
/// \param seed used for data generation
/// \param filepath if specified export results to filepath. filepath should include filename and ending (e.g. .csv)
template <typename T>
void benchmarkRows(int tableStoreId, std::vector<unsigned> &projectionAttributes, std::vector<Filter::Filter<T, SIMD::AVX512> *> &filters, unsigned rowCount = 10,
                   unsigned columnCount = 10, bool exponentialGrowth = false, unsigned growthFactor = 50, unsigned iterations = 300,
                   T lowerBound = 0, T upperBound = 1000, unsigned seed = 42, const std::string &filepath = "benchmark.csv") {
    // initialize store for result
    BenchmarkResult<T> result;

    for (int iterationCounter = 0; iterationCounter < iterations; iterationCounter++) {
        std::cout << "Processing iteration " << iterationCounter + 1 << "/" << iterations << std::endl;

        // run benchmark
        auto [resultRowCount, resultCpuTime, resultRealTime] = Benchmark::benchmarkTableImplementation(
            tableStoreId, projectionAttributes, filters, rowCount, columnCount, lowerBound, upperBound, seed);

        // store benchmark in result
        result.addBenchmark(tableStoreId, rowCount, columnCount, lowerBound, upperBound, resultRowCount, resultCpuTime, resultRealTime);

        // increment row-count
        rowCount = incrementValue(rowCount, exponentialGrowth, growthFactor);
    }

    // print result
    result.printBenchmarkResult();

    // if filepath specified export result
    if (filepath != "") {
        result.exportBenchmarkResult(filepath);
    }
}

};  // namespace Benchmark