#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <iterator>
#include <vector>

#include "Filters.h"
#include "Helper.h"
#include "memory.cpp"
#include "page.cpp"
#include "table.cpp"
#include "types.h"

using namespace std;

template <typename T>
const T** getData(const int numberOfRows) {
  T** data = (T**)malloc(numberOfRows * sizeof(T*));

  for (unsigned row = 0; row < numberOfRows; row++) {
    T* rowData = (T*)malloc(3 * sizeof(T));
    data[row] = rowData;
    data[row][0] = row;
    data[row][1] = 42 + row;
    data[row][2] = 200 + row;
  }

  return const_cast<const T**>(data);
}

template <typename T>
void testGetData() {
  const int numberOfRows = 10;
  const T** data = getData<T>(numberOfRows);
  cout << data << endl;
}

/**
 * Inspect the memory layout of a single PAX page.
 */
template <typename T>
void testPagePageManual() {
  unsigned numberOfRows = 300;
  unsigned numberOfAttributes = 3;

  const T** data = getData<T>(numberOfRows);

  auto pagesize = getPagesize();
  Buffer buffer = allocateAlignedBuffer(pagesize);

  PaxPage<T> page = PaxPage<T>((Header*)buffer, pagesize, numberOfAttributes);

  for (unsigned row = 0; row < numberOfRows; row++) page.writeRecord(data[row]);

  page.print();
}

/**
 * Inspect the memory layout of a table with a
 * single page.
 */
template <typename T>
void testPaxTableSinglePageMemory() {
  unsigned numberOfRows = 10;
  unsigned numberOfAttributes = 3;

  const T** data = getData<T>(numberOfRows);

  PaxTable<T> table(numberOfAttributes, numberOfRows, data);

  table.pages[0].print();
}

/**
 * Inspect the memory layout of a table with a two
 * pages.
 */
template <typename T>
void testPaxTableTwoPagesMemory() {
  unsigned numberOfRows = 1000;
  unsigned numberOfAttributes = 3;

  const T** data = getData<T>(numberOfRows);

  PaxTable<T> table(numberOfAttributes, numberOfRows, data);

  table.pages[0].print();
  table.pages[1].print();
}

/**
 * Print a sample table to console.
 */
template <typename T>
void testPaxTablePrint() {
  unsigned numberOfRows = 1000;
  unsigned numberOfAttributes = 3;

  const T** data = getData<T>(numberOfRows);

  PaxTable<T> table(numberOfAttributes, numberOfRows, data);

  table.pages[0].print();
  table.print();
}

/**
 * Test a single filter on a table with a single
 * page.
 */
template <typename T>
void testBasicFilter() {
  unsigned numberOfRows = 10;
  unsigned numberOfAttributes = 3;

  const T** data = getData<T>(numberOfRows);

  PaxTable<T> table(numberOfAttributes, numberOfRows, data);

  unsigned returnedRows = 0;
  unsigned returnedColumns = 0;
  std::vector<unsigned> projection = {1, 2, 3};
  std::vector<Filter<T>*> filters = {new Equal<T>(0, (T)2)};

  T** result =
      table.query_table(projection, filters, returnedRows, returnedColumns);

  for (unsigned row = 0; row < returnedRows; row++) {
    for (unsigned column = 0; column < returnedColumns; column++) {
      cout << result[row][column] << " ";
    }
    cout << endl;
  }

  cout << "Count: " << table.query_count(projection, filters) << endl;
}

int main() {
  testBasicFilter<uint16_t>();
  return 0;
}
