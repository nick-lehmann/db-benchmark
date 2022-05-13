#include <array>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "Table.h"
#include "Helper.h"
#include "Filters.h"
#include "BaseTable.h"
#include "IntermediateTable.h"
#include "Filter.h"
#include "Projection.h"

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

  std::cout << "Print Test-Query: \n" << std::endl;
  std::vector<unsigned> projectionAttributes = {0, 2, 3};
  std::vector<Filter<int> *> filters = { new GreaterThan<int>(1, 6), new LessThan<int>(2, 9) };
  unsigned numRow = 0, numCol = 0;
  baseTable.query_table(projectionAttributes, filters, numRow, numCol);

  return 0;
}
