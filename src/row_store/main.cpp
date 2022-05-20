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

/// Run a demo of the Row-Store database.
/// Creates a small example BaseTable and applies a simple query on it. Afterwards run a benchmark with the same query on the same table.
void demo() {
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
  auto [queryResult, resultRowCount, resultColumnCount] = baseTable.query_table(projectionAttributes, filters);
  RowStore::IntermediateTable<Type>::printTableOutput(queryResult, resultRowCount, resultColumnCount);

  // delete result table and free memory
  RowStore::IntermediateTable<Type>::deleteDetachedTableOutput(queryResult, resultRowCount);

  // run benchmark of same query
  std::cout << "Print benchmark: " << std::endl << std::endl;
  auto benchmarkResult = Benchmark::measureTime(baseTable, projectionAttributes, filters);
}

int main(int argc, char **argv) {
  demo();

  return 0;
}
