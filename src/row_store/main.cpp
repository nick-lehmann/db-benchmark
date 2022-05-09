#include <iostream>
#include <vector>
#include <array>
#include <cstdlib>

#include "Table.h"
#include "Projection.h"
#include "Filter.h"

int main(int argc, char ** argv) {
    using Type = uint64_t;

    std::cout << "Row-Store Code" << std::endl;
    auto baseTable = createSortedTestTable<Type>(5, 100, 50);

    baseTable->print();

    std::vector<int> projectionAttributes = {0, 2, 3};
    std::vector<Filter<Type> *> filters;
    filters.push_back(new LessThan<Type>(2, 42));

    auto result = baseTable->query(projectionAttributes, filters);

    result->print();

    //destruct tables -> free allocated memory
    delete baseTable;
    delete result;


	return 0;
}
