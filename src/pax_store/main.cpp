#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <iterator>
#include <vector>

#include "Helper.h"
#include "memory.cpp"
#include "page.cpp"
#include "table.cpp"
#include "types.h"

using namespace std;

int main() {
  uint16_t lowerBound = 0;
  uint16_t upperBound = 100;
  const uint16_t **data =
      TableHelper::generateRandomData<uint16_t>(3, 3, lowerBound, upperBound);

  PaxTable<uint16_t> table(3, 3, data);

  unsigned numberOfRows = 0;
  unsigned numberOfColumns = 0;
  std::vector<unsigned> projection;
  std::vector<Filter<uint16_t> *> filters;
  auto result =
      table.query_table(projection, filters, numberOfRows, numberOfColumns);

  // Page page = createPage();
  // fillPage(&page);

  table.print();
  // page.print();

  // cout << "Reading vector 4" << endl;
  // vector<Data> record = page.readRecord(4);
  // for (const auto& x : record) {
  //   cout << x << "\n";
  // }

  // vector<Equal> filters;
  // Equal f1(2, 69);
  // filters.push_back(f1);

  // page.process(filters);
}
