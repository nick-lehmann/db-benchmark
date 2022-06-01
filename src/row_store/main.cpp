#include <array>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "BaseTable.h"
//#include "Benchmark.h"
#include "Filter.h"
#include "Filters.h"
#include "Helper.h"
#include "ITable.h"
#include "IntermediateTable.h"
#include "Projection.h"

/// Run a demo of the Row-Store database.
/// Creates a small example BaseTable and applies a simple query on it. Afterwards run a benchmark with the same query on the same table.
void demo() {
    // define data type of table data
    using Type = int32_t;

    std::cout << "Row-Store Code" << std::endl;

    // generate example table and print
    const Type **initialData = TableHelper::generateRandomData<int>(5, 20, 1, 10);
    RowStore::BaseTable<Type> baseTable(5, 20, initialData);
    std::cout << "Print Test-BaseTable: \n" << std::endl;
    baseTable.print();

    // perform a query on base table and print result
    std::cout << "Print Test-Query: \n" << std::endl;
    std::vector<uint64_t> projectionAttributes = {0, 2, 3};
    std::vector<Filters::Scalar::Filter<Type> *> filters = {new Filters::Scalar::GreaterThan<Type>(1, 6),
                                                            new Filters::Scalar::LessThan<Type>(2, 9)};
    unsigned numRow = 0, numCol = 0;
    auto [queryResult, resultRowCount, resultColumnCount] = baseTable.queryTable(projectionAttributes, filters);
    RowStore::IntermediateTable<Type>::printTableOutput(queryResult, resultRowCount, resultColumnCount);

    // delete result table and free memory
    RowStore::IntermediateTable<Type>::deleteDetachedTableOutput(queryResult, resultRowCount);

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
    demo();
    // benchmark();

    return 0;
}
