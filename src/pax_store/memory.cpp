#pragma once
#include <stdio.h>

#include <iostream>
#include <memory>

#include "page.cpp"
#include "types.h"

using namespace std;

long getPagesize() {
  long pagesize = sysconf(_SC_PAGE_SIZE);
  if (pagesize == -1) {
    throw;
  }
  return pagesize;
}

Buffer allocateAlignedBuffer(long pagesize) {
  // Buffer buffer = (Buffer)memalign(pagesize, pagesize);

  // // Initialize with zeros
  // for (unsigned long i = 0; i < pagesize / sizeof(Buffer); i++) {
  //   *(buffer + i) = 0;
  // }

  // if (buffer == NULL) {
  //   cerr << "Failed to allocate buffer" << endl;
  //   throw;
  // }
  // char buffer[4096];
  // return &buffer;
  return (char*)0xFFFFFF;
}

// PaxPage createPage() {
//   long pagesize = getPagesize();
//   Buffer buffer = allocateAlignedBuffer(pagesize);
//   return Page((Header *)buffer, pagesize, 3);
// }

// Fill page with random data
template <typename T>
void fillPage(PaxPage<T>* page) {
  for (int i = 0; i < 10; i++) {
    T record[] = {i, 42 + i, 69 + i};
    page->writeRecord(record);
  }
}
