#include <iostream>
#include <vector>
#include <array>
#include <cstdlib>

#include "Table.h"
#include "Projection.h"
#include "Filter.h"

int main(int argc, char ** argv) {
    /*std::cout << "Row-Store Code" << std::endl;
    auto baseTable = *createSortedTestTable<uint64_t>(5, 100, 50);

    baseTable.print();
    std::cout << "_____________________" << std::endl;

    int projectionAttributes[] = {0, 2, 3};
    auto result = *projection(baseTable, projectionAttributes, 3);

    result.print();

    auto result2 = *filter_basic(result, 1, 42);

    result2.print();


    //destruct tables -> free allocated memory
    delete &result;
    delete &result2;
    delete &baseTable;*/

	return 0;
}
