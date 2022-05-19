#include <chrono>
#include <iostream>
#include <string.h>
#include <vector>

template <typename T> class Benchmark {

public:
  Benchmark() = default;

  uint64_t measureTime(Table<T> *table, std::vector<unsigned> &projection,
                       std::vector<Filter<T> *> &filters) {
    auto startTime = std::clock();
    uint64_t count = table->query_count(projection, filters);
    auto endTime = std::clock();
    std::cout << std::fixed << std::setprecision(4)
              << "Query time: " << 1000 * (endTime - startTime) / CLOCKS_PER_SEC
              << " ms" << std::endl;
    return count;
  }
};