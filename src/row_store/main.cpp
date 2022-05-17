#include <array>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "BaseTable.h"
#include "Filter.h"
#include "Filters.h"
#include "Helper.h"
#include "IntermediateTable.h"
#include "Projection.h"
#include "Table.h"

int main(int argc, char **argv) {
  using Type = int32_t;

  std::cout << "Row-Store Code" << std::endl;
  const Type **initialData = TableHelper::generateRandomData<int>(5, 20, 1, 10);
  RowStore::BaseTable<Type> baseTable(5, 20, initialData);

  std::cout << "Print Test-BaseTable: \n" << std::endl;
  baseTable.print();

  /*std::cout << "Print Test-IntermediateTable: \n" << std::endl;
  RowStore::IntermediateTable<Type> interTable(5, baseTable.data);
  uint64_t size = 0;
  interTable.printTableOutput();*/

  std::cout << "Print Test-Query: \n" << std::endl;
  std::vector<unsigned> projectionAttributes = {0, 2, 3};
  std::vector<Filter<Type> *> filters = {new GreaterThan<Type>(1, 6), new LessThan<Type>(2, 9)};
  unsigned numRow = 0, numCol = 0;
  Type **tableOut = baseTable.query_table(projectionAttributes, filters, numRow, numCol);
  RowStore::IntermediateTable<Type>::printTableOutput(tableOut, numRow, numCol);
  RowStore::IntermediateTable<Type>::deleteDetachedTableOutput(tableOut, numRow);

  return 0;
}
