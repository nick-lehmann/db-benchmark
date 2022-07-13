#pragma once
#include <stdio.h>

#include <iostream>
#include <memory>

#include "Page.h"
#include "Types.h"

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
  for (unsigned long i = 0; i < pagesize; i++) {
    *(buffer + i) = 0;
  }
  return buffer;
}
