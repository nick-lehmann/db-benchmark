#pragma once

#include <chrono>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>
#include<type_traits>

#include "../column_store/ColumnStoreTable.h"
#include "../pax_store/PaxTable.h"
#include "../row_store/BaseTable.h"
#include "BenchmarkResult.h"
#include "Constants.h"
#include "Filters/Base.h"
#include "Helper.h"
#include "ITable.h"
#include "SIMD.h"
//#include "Filters/LessThan.h"

namespace Benchmark {
/// Apply a query using given projection attributes and filters on a table and measure the time.
/// Print the used cpu time and real time in ms, as well as the number of rows of the result table, and returns those
/// results as well.
/// \param table original table to apply the query on
/// \param projection projection attributes for the query
/// \param filters filters to apply for the query
template <typename T, SIMD Variant>
std::tuple<double, double, double> measureTime(Tables::ITable<T> &table, std::vector<uint64_t> &projection,
                                               std::vector<Filters::Filter<T, Variant> *> &filters, bool enablePrint = true) {
    std::vector<uint64_t> clockDurations;
    std::vector<double> realDurations;
    std::vector<uint64_t> counts;

    double realDuration;
    double count;
    double clockDuration;

    for (int i = 0; i < 10; i++) {
        // measure both cpu time and real time
        auto clockStartTime = std::clock();
        auto realStartTime = std::chrono::steady_clock::now();

        // call query function
        auto [result, rows, columns] = table.queryTable(projection, filters);
        auto countTMP = rows;

        auto clockEndTime = std::clock();
        auto realEndTime = std::chrono::steady_clock::now();

        // free the table
        TableHelper::freeTable(result, rows);

        // store results
        auto clockDurationTMP = clockEndTime - clockStartTime;
        realDuration = std::chrono::duration<double, std::milli>(realEndTime - realStartTime).count();

        clockDurations.push_back(clockDurationTMP);
        realDurations.push_back(realDuration);
        counts.push_back(countTMP);
    }

    auto max = std::max_element(realDurations.begin(), realDurations.end());
    auto argmax = std::distance(realDurations.begin(), max);

    clockDurations.erase(clockDurations.begin() + argmax);
    realDurations.erase(realDurations.begin() + argmax);
    counts.erase(counts.begin() + argmax);

    auto min = std::min_element(realDurations.begin(), realDurations.end());
    auto argmin = std::distance(realDurations.begin(), min);

    clockDurations.erase(clockDurations.begin() + argmin);
    realDurations.erase(realDurations.begin() + argmin);
    counts.erase(counts.begin() + argmin);

    clockDuration = std::accumulate(clockDurations.begin(), clockDurations.end(), 0.0) / (double)clockDurations.size();
    realDuration = std::accumulate(realDurations.begin(), realDurations.end(), 0.0) / (double)realDurations.size();
    count = std::accumulate(counts.begin(), counts.end(), 0.0) / (double)counts.size();

    // print result
    if (enablePrint) {
        std::cout << std::fixed << std::setprecision(8) << "CPU cycles: " << clockDuration << std::endl
                  << "Real time: " << realDuration << " ms" << std::endl
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
template <typename T, SIMD Variant>
std::tuple<double, double, double> benchmarkTableImplementation(int tableStoreId, std::vector<uint64_t> &projectionAttributes,
                                                                std::vector<Filters::Filter<T, Variant> *> &filters, unsigned rowCount,
                                                                unsigned columnCount, T lowerBound, T upperBound, unsigned seed) {
    // create data
    const T **tableData = TableHelper::generateRandomData<T>(columnCount, rowCount, lowerBound, upperBound, seed);

    // create table
    switch (tableStoreId) {
        case 0: {
            // row store
            if constexpr(std::is_same<T,uint64_t>::value==true) {
                RowStore::BaseTable<T, 4096> table(columnCount, rowCount, tableData);
                auto [resultRowCount, resultCpuTime, resultRealTime] = Benchmark::measureTime(table, projectionAttributes, filters, false);
                TableHelper::freeTable(const_cast<T **>(tableData), rowCount);
                return std::make_tuple(resultRowCount, resultCpuTime, resultRealTime);
            } else {
                RowStore::BaseTable<T, 2048> table(columnCount, rowCount, tableData);
                auto [resultRowCount, resultCpuTime, resultRealTime] = Benchmark::measureTime(table, projectionAttributes, filters, false);
                TableHelper::freeTable(const_cast<T **>(tableData), rowCount);
                return std::make_tuple(resultRowCount, resultCpuTime, resultRealTime);
            }
            // run benchmark and return

        }
        case 1: {
            // column store
            ColumnStore::Table<T, T> table(columnCount, rowCount, tableData);

            // run benchmark and return
            auto [resultRowCount, resultCpuTime, resultRealTime] = Benchmark::measureTime(table, projectionAttributes, filters, false);
            TableHelper::freeTable(const_cast<T **>(tableData), rowCount);
            return std::make_tuple(resultRowCount, resultCpuTime, resultRealTime);
        }
        case 2: {
            // pax store
            PaxTable<T> table(columnCount, rowCount, tableData);

            // run benchmark and return
            auto [resultRowCount, resultCpuTime, resultRealTime] = Benchmark::measureTime(table, projectionAttributes, filters, false);
            TableHelper::freeTable(const_cast<T **>(tableData), rowCount);
            return std::make_tuple(resultRowCount, resultCpuTime, resultRealTime);
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
template <typename T, SIMD Variant>
void benchmarkRows(int tableStoreId, std::vector<uint64_t> &projectionAttributes, std::vector<Filters::Filter<T, Variant> *> &filters,
                   unsigned rowCount = 10, unsigned columnCount = 10, bool exponentialGrowth = false, unsigned growthFactor = 50,
                   unsigned iterations = 300, T lowerBound = 0, T upperBound = 1000, unsigned seed = 42,
                   const std::string &filepath = "benchmark.csv") {
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