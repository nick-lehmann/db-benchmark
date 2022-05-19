#include <chrono>
#include <iomanip>
#include <iostream>
#include <string.h>
#include <tuple>
#include <vector>

template <typename T> class Benchmark {

public:
  Benchmark() = default;

  uint64_t measureTime(Table<T> *table, std::vector<unsigned> &projection, std::vector<Filter<T> *> &filters) {
    // measure both cpu time and real time
    auto clockStartTime = std::clock();
    auto realStartTime = std::chrono::steady_clock::now();

    // call query count function
    uint64_t count = table->query_count(projection, filters);

    auto clockEndTime = std::clock();
    auto realEndTime = std::chrono::steady_clock::now();

    // print result
    std::cout << std::fixed << std::setprecision(8)
              << "Query time: " << 1000 * (clockEndTime - clockStartTime) / CLOCKS_PER_SEC << " ms" << std::endl
              << "Real time: " << std::chrono::duration<double, std::milli>(realEndTime - realStartTime).count()
              << " ms" << std::endl
              << "Row Count: " << count << std::endl;

    return count;
  }
};