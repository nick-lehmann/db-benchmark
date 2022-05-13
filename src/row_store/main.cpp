#include <array>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "BaseTable.h"
#include "Filter.h"
#include "Helper.h"
#include "IntermediateTable.h"
#include "Projection.h"
#include "Table.h"

int main(int argc, char **argv) {
  using Type = uint64_t;

  std::cout << "Row-Store Code" << std::endl;
  const int **initialData = TableHelper::generateRandomData<int>(5, 20, 1, 10);
  RowStore::BaseTable<int> baseTable(5, 20, initialData);

  std::cout << "Print Test-BaseTable: \n" << std::endl;
  baseTable.print();

  std::cout << "Print Test-IntermediateTable: \n" << std::endl;
  RowStore::IntermediateTable<int> interTable(5, baseTable.data);
  uint64_t size = 0;
  interTable.printTableOutput();

  std::vector<unsigned> projectionAttributes = {0, 2, 3};

  std::vector<Filter<int> *> filters;
  unsigned numRow = 0, numCol = 0;
  baseTable.query_table(projectionAttributes, filters, numRow, numCol);

  /*RowStore::IntermediateTable<int> result = RowStore::projection(interTable, projectionAttributes);
  RowStore::printTableOutput(projectionAttributes.size(), size, result.table(size));*/

  /*std::vector<int> projectionAttributes = {0, 2, 3};
  std::vector<Filter<Type> *> filters;
  filters.push_back(new LessThan<Type>(2, 42));*/

  /* commented out until implementation of baseTable & IntermediateTable is complete and working
  auto result = baseTable->query(projectionAttributes, filters);

  result->print();
  */

  return 0;
}
