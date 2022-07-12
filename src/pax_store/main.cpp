#include <errno.h>
#include <immintrin.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <iterator>
#include <vector>

#include "Constants.h"
#include "Filters/All.h"
#include "Helper.h"
#include "Memory.h"
#include "Page.h"
#include "PaxTable.h"
#include "SIMD.h"
#include "Types.h"

using namespace std;

template <typename T>
const T** getData(const int numberOfRows) {
    T** data = (T**)malloc(numberOfRows * sizeof(T*));

    for (unsigned row = 0; row < numberOfRows; row++) {
        T* rowData = (T*)malloc(3 * sizeof(T));
        data[row] = rowData;
        data[row][0] = row;
        data[row][1] = 42 + row;
        data[row][2] = 200 + row;
    }

    return const_cast<const T**>(data);
}

template <typename T>
void testGetData() {
    const int numberOfRows = 10;
    const T** data = getData<T>(numberOfRows);
    cout << data << endl;
}

/**
 * Inspect the memory layout of a single PAX page.
 */
template <typename T>
void testPagePageManual() {
    unsigned numberOfRows = 300;
    unsigned numberOfAttributes = 3;

    const T** data = getData<T>(numberOfRows);

    auto pagesize = getPagesize();
    Buffer buffer = allocateAlignedBuffer(pagesize);

    PaxPage<T> page = PaxPage<T>((Header*)buffer, pagesize, numberOfAttributes);

    for (unsigned row = 0; row < numberOfRows; row++) page.writeRecord(data[row]);

    page.print();
}

/**
 * Inspect the memory layout of a table with a
 * single page.
 */
template <typename T>
void testPaxTableSinglePageMemory() {
    unsigned numberOfRows = 10;
    unsigned numberOfAttributes = 3;

    const T** data = getData<T>(numberOfRows);

    PaxTable<T> table(numberOfAttributes, numberOfRows, data);

    table.pages[0].print();
}

/**
 * Inspect the memory layout of a table with a two
 * pages.
 */
template <typename T>
void testPaxTableTwoPagesMemory() {
    unsigned numberOfRows = 1000;
    unsigned numberOfAttributes = 3;

    const T** data = getData<T>(numberOfRows);

    PaxTable<T> table(numberOfAttributes, numberOfRows, data);

    table.pages[0].print();
    table.pages[1].print();
}

/**
 * Print a sample table to console.
 */
template <typename T>
void testPaxTablePrint() {
    unsigned numberOfRows = 1000;
    unsigned numberOfAttributes = 3;

    const T** data = getData<T>(numberOfRows);

    PaxTable<T> table(numberOfAttributes, numberOfRows, data);

    table.pages[0].print();
    table.print();
}

/**
 * Test a single filter on a table with a single
 * page.
 */
// template <typename T>
// void testBasicScalarFilters() {
//     unsigned numberOfRows = 10;
//     unsigned numberOfAttributes = 3;

//     const T** data = getData<T>(numberOfRows);

//     PaxTable<T> table(numberOfAttributes, numberOfRows, data);

//     std::vector<uint64_t> projection = {0, 1, 2};
//     std::vector<Filters::Scalar::Filter<T>*> filters = {new Filters::Scalar::Equal<T>(0, (T)2)};

//     auto [result, rows, columns] = table.queryTable(projection, filters);

//     for (unsigned row = 0; row < rows; row++) {
//         for (unsigned column = 0; column < columns; column++) {
//             cout << result[row][column] << " ";
//         }
//         cout << endl;
//     }

//     cout << "Count: " << rows << endl;
// }

/**
 * Test a single filter on a table with a single
 * page.
 */
template <typename T>
void testBasicAVXFilters() {
    unsigned numberOfRows = 10;
    unsigned numberOfAttributes = 3;

    const T** data = getData<T>(numberOfRows);

    PaxTable<T> table(numberOfAttributes, numberOfRows, data);

    std::vector<uint64_t> projection = {0, 1, 2};
    std::vector<Filters::Filter<T, SIMD::AVX512>*> filters = {new Filters::LessEqual<T, SIMD::AVX512>(0, 3),
                                                              new Filters::GreaterEqual<T, SIMD::AVX512>(0, 1),
                                                              new Filters::Equal<T, SIMD::AVX512>(0, 2)};

    auto [result, rows, columns] = table.queryTable(projection, filters);

    for (unsigned row = 0; row < rows; row++) {
        for (unsigned column = 0; column < columns; column++) {
            cout << result[row][column] << " ";
        }
        cout << endl;
    }

    cout << "Count: " << rows << endl;
}

// template <typename T>
// void queryAVX(Filter::Filter<T, SIMD::AVX512>& filter) {
//     __m512i reg32bit = _mm512_set1_epi32(1);
//     auto mask = filter.match(reg32bit);
// }

// template <typename T>
// void queryNormal(Filter::Filter<T, SIMD::None>& filter) {
//     T value = 1;
//     auto result = filter.match(value);
//     cout << "Scalar operations says: " << result << endl;
// }

int main() {
    testBasicAVXFilters<uint64_t>();
    // Filter::Equal<uint32_t, SIMD::AVX512> equal32bitAVX(1, 1);
    // Filter::LessThan<uint32_t, SIMD::AVX512> lessThan32bitAVX(1, 1);
    // Filter::Equal<uint32_t, SIMD::None> equal32bitScalar(1, 1);

    // queryAVX(equal32bitAVX);
    // queryAVX(lessThan32bitAVX);
    // queryNormal(equal32bitScalar);

    return 0;
}
