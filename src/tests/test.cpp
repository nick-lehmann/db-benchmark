#include <cassert>
#include <iostream>

#include "../column_store/ColumnStoreTable.h"
#include "../pax_store/PaxTable.h"
#include "../row_store/BaseTable.h"
#include "./TestUtils.h"
#include "Filters/All.h"
#include "ITable.h"
#include "SIMD.h"

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

/**
 * Test a single filter on a table with a single page.
 */
template <typename T, SIMD Variant>
int testBasicFilters() {
    unsigned numberOfRows = 10;
    unsigned numberOfAttributes = 3;
    const T** data = getData<T>(numberOfRows);

    PaxTable<T> paxTable(numberOfAttributes, numberOfRows, data);
    ColumnStore::Table<T> columnTable(numberOfAttributes, numberOfRows, data);
    RowStore::BaseTable<T> rowTable(numberOfAttributes, numberOfRows, data);

    // Tables::ITable<T>* tables[] = {&paxTable};
    // Tables::ITable<T>* tables[] = {&columnTable};
    Tables::ITable<T>* tables[] = {&rowTable};

    std::vector<uint64_t> projection = {0, 1, 2};
    std::vector<Filters::Filter<T, Variant>*> filters = {
        new Filters::LessEqual<T, Variant>(0, 2),
    };

    const uint64_t expectedRows = 2, expectedColumns = 3;
    T expectedData[2][3] = {{0, 42, 200}, {1, 43, 201}};

    for (unsigned i = 0; i < 1; i++) {
        std::cout << "Testing table " << i << std::endl;
        Tables::ITable<T>* table = tables[i];

        auto [data, rows, columns] = table->queryTable(projection, filters);

        std::cout << "Rows x Columns: (" << rows << "," << columns << ")" << std::endl;

        if (rows != expectedRows || columns != expectedColumns) {
            std::cout << "Expected table with dimensions (" << expectedRows << "," << expectedColumns << ") but got one with (" << rows
                      << "," << columns << ")" << std::endl
                      << std::endl;

            std::cout << "Got: " << std::endl;
            printTable<T**>(data, rows, columns);

            std::cout << "Expected: " << std::endl;
            printTable<T[][3]>(expectedData, expectedRows, expectedColumns);

            return 1;
        }
    }

    return 0;
}

int main() {
    std::cout << "Test basic filters" << std::endl;
    return testBasicFilters<uint64_t, SIMD::AVX512>();
}