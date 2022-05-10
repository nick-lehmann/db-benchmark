#pragma once
#include <stdio.h>

#include <iostream>
#include <memory>

#include "page.cpp"
#include "types.h"

using namespace std;

/**
 * @brief Get the size of the systen page in bytes
 *
 * @return long
 */
long getPagesize() {
  long pagesize = sysconf(_SC_PAGESIZE);
  if (pagesize == -1) {
    throw;
  }
  return pagesize;
}

/**
 * @brief Allocate a aligned memory page and initialize it with zeros
 *
 * @param pagesize
 * @return Buffer
 */
Buffer allocateAlignedBuffer(long pagesize) {
  Buffer buffer = (Buffer)std::aligned_alloc(pagesize, pagesize);

  if (buffer == NULL) {
    cerr << "Failed to allocate buffer" << endl;
    throw;
  }

  // Initialize with zeros
  for (unsigned long i = 0; i < pagesize / sizeof(Buffer); i++) {
    *(buffer + i) = 0;
  }
  return buffer;
}

// PaxPage createPage() {
//   long pagesize = getPagesize();
//   Buffer buffer = allocateAlignedBuffer(pagesize);
//   return Page((Header *)buffer, pagesize, 3);
// }

/**
 * @brief Fill the page with random data
 *
 * @tparam T
 * @param page
 */
template <typename T> void fillPage(PaxPage<T> *page) {
  for (int i = 0; i < 10; i++) {
    T record[] = {i, 42 + i, 69 + i};
    page->writeRecord(record);
  }
}
