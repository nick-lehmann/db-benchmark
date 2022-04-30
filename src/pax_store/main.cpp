#include <errno.h>
#include <malloc.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <iterator>
#include <vector>

#include "filters.cpp"
#include "memory.cpp"
#include "pax.cpp"
#include "types.h"

using namespace std;

int main() {
  Page page = createPage();
  fillPage(&page);

  page.print();

  // cout << "Reading vector 4" << endl;
  // vector<Data> record = page.readRecord(4);
  // for (const auto& x : record) {
  //   cout << x << "\n";
  // }

  vector<Equal> filters;
  Equal f1(2, 69);
  filters.push_back(f1);

  page.process(filters);
}
