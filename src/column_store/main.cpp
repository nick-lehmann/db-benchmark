#include <iostream>

#include "ColumnStoreTable.h"
#include "Helper.h"

void debugTest() {
    const int** initialData = TableHelper::generateRandomData<int>(5, 20, 1, 10);
    TableHelper::printTable(initialData, 5, 20);

    std::cout << "\n\n" << std::endl;

    ColumnStore::ColumnStoreTable<int> testTable(5, 20, initialData);

    testTable.print();
}

int main(int argc, char ** argv) {
    debugTest();
    return 0;
}
