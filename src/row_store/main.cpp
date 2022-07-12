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

template <typename T, SIMD Variant, int Stride>
void demoUnified() {
    /*unsigned width = 10;
    unsigned height = 20;

    const T **initialData =
        TableHelper::generateFunctionData<T>(width, height, [&width](unsigned column, unsigned row) { return row * width + column; });

    std::vector<T *> data;
    for (unsigned row = 0; row < height; row++) {
        data.push_back(static_cast<T *>(malloc(sizeof(T) * width)));
        for (unsigned column = 0; column < width; column++) {
            data.back()[column] = initialData[row][column];
        }
    }

    RowStore::IntermediateTable<T, Variant, Stride> table(width, data);

    auto iter = table.vectorBegin();
    auto itere = table.vectorEnd();
    while (*iter != *itere) {
        auto vec = iter->gather(5);
        auto target = aligned_alloc(64, 64);
        _mm512_store_epi32(target, vec);

        for (int i = 0; i < 16; ++i) {
            std::cout << ((T *)target)[i] << " ";
        }
        std::cout << std::endl;
        free(target);
        ++(*iter);
    }
    std::cout << "\n\n" << std::endl;

    delete iter;
    delete itere;*/
}

template <typename T, SIMD Variant>
void stridedDemo() {
    /*unsigned width = 10;
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
    }*/
}

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
    std::cout << "Print Test-BaseTable: \n" << std::endl;
    baseTable.print();

    // perform a query on base table and print result
    std::cout << "Print Test-Query: \n" << std::endl;
    std::vector<uint64_t> projectionAttributes = {0, 2, 3};
    std::vector<Filters::Filter<T, Variant> *> filters = {new Filters::GreaterThan<T, Variant>(1, 6),
                                                          new Filters::LessThan<T, Variant>(2, 9)};

    auto [queryResult, resultRowCount, resultColumnCount] = baseTable.queryTable(projectionAttributes, filters);

    RowStore::IntermediateTable<T, Variant, Alignment>::printTableOutput(queryResult, resultRowCount, resultColumnCount);

    // delete result table and free memory
    RowStore::IntermediateTable<T, Variant, Alignment>::deleteTableOutput(queryResult, resultRowCount);

    // run benchmark of same query
    // std::cout << "Print benchmark: " << std::endl << std::endl;
    // auto benchmarkResult = Benchmark::measureTime(baseTable, projectionAttributes, filters);*/
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
    // demoUnified<uint32_t, SIMD::None, 4096>();
    // demoUnified<uint64_t, SIMD::AVX512, 4096>();
    // demoUnified<uint64_t, SIMD::AVX512_Strided, 4096>();
    demo<uint32_t, SIMD::None, 4096>();
    demo<uint32_t, SIMD::AVX512, 4096>();
    demo<uint32_t, SIMD::AVX512_Strided, 4096>();

    // benchmark();
    return 0;
}
