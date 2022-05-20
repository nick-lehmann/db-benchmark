#include <iostream>

#include "ColumnStoreTable.h"
#include "Helper.h"
#include "Benchmark.h"

#define COLUMNS 5
#define ROWS 100000000

void debugTest() {
    const int** initialData = TableHelper::generateRandomData<int>(COLUMNS, ROWS, 1, 10);
    ColumnStore::ColumnStoreTable<int> testTable(COLUMNS, ROWS, initialData);

    std::cout << "Test table: \n" << std::endl;

    testTable.print();

    auto equalFilter = new Equal<int>(0, 4);
    auto greaterFilter = new GreaterThan<int>(4, 4);

    std::vector<Filter<int>*> filters {equalFilter, greaterFilter};
    std::vector<unsigned> projection {0, 2, 4};

    auto [queried, rows, columns] = testTable.query_table(projection, filters);

    std::cout << "Filters (0, =4) and (4, >4) as well as projection on (0, 2, 4): \n" << std::endl;

    TableHelper::printTable(queried, columns, rows);

    std::cout << "Number of rows: " << testTable.query_count(projection, filters) << std::endl;
}

void benchmark() {
    const uint8_t** initialData = TableHelper::generateRandomData<uint8_t>(COLUMNS, ROWS, 1, 10);
    ColumnStore::ColumnStoreTable<uint8_t> testTable(COLUMNS, ROWS, initialData);

    auto equalFilter = new Equal<uint8_t>(0, 4);
    auto greaterFilter = new GreaterThan<uint8_t>(4, 4);

    std::vector<Filter<uint8_t>*> filters {equalFilter, greaterFilter};
    std::vector<unsigned> projection {0, 2, 4};

    Benchmark::measureTime(testTable, projection, filters);
}

int main(int argc, char ** argv) {
    // debugTest();
    benchmark();
    return 0;
}
