#include <array>
#include <cstdlib>
#include <iostream>
#include <vector>

//#include "Benchmark.h"

#include "BaseTable.h"
#include "Filters/All.h"
#include "Helper.h"
#include "ITable.h"
#include "StridedTable.h"
//#include "IntermediateTable.h"
//#include "Filter.h"
//#include "Projection.h"
// #include "IntermediateTable_AVX.h"
#include "IntermediateTable_AVX.h"
#include "SIMD.h"

template <typename T, SIMD Variant>
void stridedDemo() {
    unsigned width = 10;
    unsigned height = 20;

    const T **initialData =
        TableHelper::generateFunctionData<T>(width, height, [&width](unsigned column, unsigned row) { return row * width + column; });

    RowStore::BaseTable<T> baseTable(width, height, initialData);
    std::cout << "Print Test-BaseTable: \n" << std::endl;
    baseTable.print();

    RowStore::StridedTable<T, 4096> stridedTable(width, baseTable.data);
    stridedTable.addRow(baseTable.data[1]);

    auto iter = stridedTable.begin();
    while (iter != stridedTable.end()) {
        std::cout << *iter++ << std::endl;
    }
}

/// Run a demo of the Row-Store database.
/// Creates a small example BaseTable and applies a simple query on it. Afterwards run a benchmark with the same query on the same table.
// TODO: Probably only works for `Variant = SIMD::AVX512` as the AVX variant of the intermediate table is used
template <typename T, SIMD Variant>
void demo() {
    std::cout << "Row-Store Code" << std::endl;

    // generate example table and print
    const T **initialData = TableHelper::generateRandomData<T>(5, 32, 1, 10);
    RowStore::BaseTable<T> baseTable(5, 32, initialData);
    std::cout << "Print Test-BaseTable: \n" << std::endl;
    baseTable.print();

    // perform a query on base table and print result
    std::cout << "Print Test-Query: \n" << std::endl;
    std::vector<uint64_t> projectionAttributes = {0, 2, 3};
    std::vector<Filters::Filter<T, Variant> *> filters = {new Filters::GreaterThan<T, Variant>(1, 6),
                                                          new Filters::LessThan<T, Variant>(2, 9)};

    unsigned numRow = 0, numCol = 0;
    auto [queryResult, resultRowCount, resultColumnCount] = baseTable.queryTable(projectionAttributes, filters);
    RowStore::IntermediateTable_AVX<T>::printTableOutput(queryResult, resultRowCount, resultColumnCount);

    // delete result table and free memory
    RowStore::IntermediateTable_AVX<T>::deleteDetachedTableOutput(queryResult, resultRowCount);

    // run benchmark of same query
    // std::cout << "Print benchmark: " << std::endl << std::endl;
    // auto benchmarkResult = Benchmark::measureTime(baseTable, projectionAttributes, filters);
}

/// Run an example benchmark of the Row-Store database.
/*void benchmark() {
    using Type = int32_t;

    // define projection attributes
    std::vector<unsigned> projectionAttributes = {0, 2, 3};
    // define filters
    std::vector<Filter<Type> *> filters = {new GreaterThan<Type>(1, 400), new LessThan<Type>(2, 600)};

    // run benchmarks
    Benchmark::benchmarkRows(0, projectionAttributes, filters);
}*/

int main(int argc, char **argv) {
    stridedDemo<uint32_t, SIMD::AVX512>();

    // benchmark();
    return 0;
}
