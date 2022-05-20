#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

namespace Benchmark {
/// Apply a query using given projection attributes and filters on a table and measure the time.
/// Print the used cpu time and real time in ms, as well as the number of rows of the result table, and returns those
/// results as well.
/// \param table original table to apply the query on
/// \param projection projection attributes for the query
/// \param filters filters to apply for the query
template <typename T>
std::tuple<uint64_t, uint64_t, double> measureTime(Table<T> &table, std::vector<unsigned> &projection, std::vector<Filter<T> *> &filters) {
  // measure both cpu time and real time
  auto clockStartTime = std::clock();
  auto realStartTime = std::chrono::steady_clock::now();

  // call query count function
  uint64_t count = table.query_count(projection, filters);

  auto clockEndTime = std::clock();
  auto realEndTime = std::chrono::steady_clock::now();

  // store results
  uint64_t clockDuration = clockEndTime - clockStartTime;
  double realDuration = std::chrono::duration<double, std::milli>(realEndTime - realStartTime).count();

  // print result
  std::cout << std::fixed << std::setprecision(8) << "CPU cycles: " << clockEndTime - clockStartTime << std::endl
            << "Real time: " << std::chrono::duration<double, std::milli>(realEndTime - realStartTime).count() << " ms" << std::endl
            << "Row Count: " << count << std::endl;

  return std::make_tuple(count, clockDuration, realDuration);
}
}; // namespace Benchmark