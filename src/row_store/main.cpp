#include <array>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "BaseTable.h"
#include "Benchmark.h"
#include "Filter.h"
#include "Filters.h"
#include "Helper.h"
#include "IntermediateTable.h"
#include "Projection.h"
#include "Table.h"

int main(int argc, char **argv) {
  // define data type of table data
  using Type = int32_t;

  std::cout << "Row-Store Code" << std::endl;

  // generate example table and print
  const Type **initialData = TableHelper::generateRandomData<int>(5, 20, 1, 10);
  RowStore::BaseTable<Type> baseTable(5, 20, initialData);
  std::cout << "Print Test-BaseTable: \n" << std::endl;
  baseTable.print();

  // perform a query on base table and print result
  std::cout << "Print Test-Query: \n" << std::endl;
  std::vector<unsigned> projectionAttributes = {0, 2, 3};
  std::vector<Filter<Type> *> filters = {new GreaterThan<Type>(1, 6), new LessThan<Type>(2, 9)};
  unsigned numRow = 0, numCol = 0;
  auto [queryResult, resultRowCount, resultColumnCount] = baseTable.query_table(projectionAttributes, filters, numRow, numCol);
  RowStore::IntermediateTable<Type>::printTableOutput(queryResult, resultRowCount, resultColumnCount);

  // delete result table and free memory
  RowStore::IntermediateTable<Type>::deleteDetachedTableOutput(queryResult, resultRowCount);

  std::cout << "Print benchmark: " << std::endl << std::endl;
  auto benchmarkResult = Benchmark::measureTime(baseTable, projectionAttributes, filters);

  return 0;
}
