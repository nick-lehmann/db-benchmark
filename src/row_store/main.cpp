#include <iostream>
#include <vector>
#include <array>
#include <cstdlib>

#include "Table.h"
#include "Projection.h"
#include "Filter.h"
#include "BaseTable.h"
#include "Helper.h"

int main(int argc, char ** argv) {
    using Type = uint64_t;

    std::cout << "Row-Store Code" << std::endl;
    const int** initialData = TableHelper::generateRandomData<int>(5, 20, 1, 10);
    RowStore::BaseTable<int> baseTable(5, 20, initialData);

    std::cout << "Print Test-BaseTable: \n" << std::endl;
    baseTable.print();

    std::vector<int> projectionAttributes = {0, 2, 3};
    std::vector<Filter<Type> *> filters;
    filters.push_back(new LessThan<Type>(2, 42));

    /* commented out until implementation of baseTable & IntermediateTable is complete and working
    auto result = baseTable->query(projectionAttributes, filters);

    result->print();
    */


    //destruct tables -> free allocated memory
    delete baseTable;
    
    /* commented out until implementation of baseTable & IntermediateTable is complete and working
    delete result;
    */

	return 0;
}
