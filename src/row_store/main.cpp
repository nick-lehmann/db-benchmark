#include <array>
#include <cstdlib>
#include <iostream>
#include <vector>

//#include "Benchmark.h"

#include "BaseTable.h"
#include "Filters/All.h"
#include "Helper.h"
#include "ITable.h"
#include "intermediateRepresentation/IntermediateTable.h"
//#include "IntermediateTable.h"
//#include "Filter.h"
//#include "Projection.h"
// #include "IntermediateTable_AVX.h"
//#include "IntermediateTable_AVX.h"
#include "SIMD.h"

/// Run a demo of the Row-Store database.
/// Creates a small example BaseTable and applies a simple query on it. Afterwards run a benchmark with the same query on the same table.
// TODO: Probably only works for `Variant = SIMD::AVX512` as the AVX variant of the intermediate table is used
template <typename T, SIMD Variant, int Alignment>
void demo() {
    std::cout << "Row-Store Code" << std::endl;

    int width = 10, height = 32;

    // generate example table and print


    const T **initialData = TableHelper::generateRandomData<T>(width, height, 1, 10);
    RowStore::BaseTable<T, Alignment> baseTable(width, height, initialData);
    TableHelper::freeTable(const_cast<T **>(initialData), height);

    std::cout << "Print Test-BaseTable: \n" << std::endl;
    baseTable.print();

    // perform a query on base table and print result
    std::cout << "Print Test-Query: \n" << std::endl;
    std::vector<uint64_t> projectionAttributes = {0, 2, 3};
    std::vector<Filters::Filter<T, Variant> *> filters = {new Filters::GreaterThan<T, Variant>(1, 6),
                                                          new Filters::LessThan<T, Variant>(2, 9)};

    auto [queryResult, resultRowCount, resultColumnCount] = baseTable.queryTable(projectionAttributes, filters);

    TableHelper::printTable(queryResult, resultColumnCount, resultRowCount);

    // delete result table and free memory
    TableHelper::freeTable(queryResult, resultRowCount);
}

int main(int argc, char **argv) {

    demo<uint32_t, SIMD::None, 4096>();
    demo<uint32_t, SIMD::AVX512, 4096>();
    demo<uint32_t, SIMD::AVX512_Strided, 4096>();


    // benchmark();
    return 0;
}
