#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <iterator>
#include <vector>

#include "Filters.h"
#include "Helper.h"
#include "Memory.h"
#include "Page.h"
#include "PaxTable.h"
#include "PaxTable_AVX.h"
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
template <typename T>
void testBasicScalarFilters() {
    unsigned numberOfRows = 10;
    unsigned numberOfAttributes = 3;

    const T** data = getData<T>(numberOfRows);

    PaxTable<T> table(numberOfAttributes, numberOfRows, data);

    std::vector<uint64_t> projection = {0, 1, 2};
    std::vector<Filters::Scalar::Filter<T>*> filters = {new Filters::Scalar::Equal<T>(0, (T)2)};

    auto [result, rows, columns] = table.queryTable(projection, filters);

    for (unsigned row = 0; row < rows; row++) {
        for (unsigned column = 0; column < columns; column++) {
            cout << result[row][column] << " ";
        }
        cout << endl;
    }

    cout << "Count: " << rows << endl;
}

/**
 * Test a single filter on a table with a single
 * page.
 */
template <typename T>
void testBasicAVXFilters() {
    unsigned numberOfRows = 10;
    unsigned numberOfAttributes = 3;

    const T** data = getData<T>(numberOfRows);

    PaxTableAVX<T> table(numberOfAttributes, numberOfRows, data);

    std::vector<uint64_t> projection = {0, 1, 2};
    std::vector<Filters::AVX::Filter<T>*> filters = {new Filters::AVX::LessEqual<T>(0, 3), new Filters::AVX::GreaterEqual<T>(0, 1),
                                                     new Filters::AVX::Equal<T>(0, 2)};
    // std::vector<Filters::AVX::Filter<T>*> filters = {new Filters::AVX::LessEqual<T>(0, 3)};

    auto [result, rows, columns] = table.queryTable(projection, filters);

    for (unsigned row = 0; row < rows; row++) {
        for (unsigned column = 0; column < columns; column++) {
            cout << result[row][column] << " ";
        }
        cout << endl;
    }

    cout << "Count: " << rows << endl;
}

int main() {
    testBasicAVXFilters<uint64_t>();
    return 0;
}
