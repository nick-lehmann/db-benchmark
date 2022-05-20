#include <fstream>
#include <iostream>
#include <map>
#include <tuple>

template <typename T>
class BenchmarkResult {
 private:
  // result map containing the parameters of the benchmark as a tuple as key and a tuple of the benchmark result as value
  std::map<std::tuple<int, unsigned, unsigned, T, T>, std::tuple<uint64_t, uint64_t, double>> result;

 public:
  /// Constructor
  BenchmarkResult() {}

  /// Add benchmark result to result map
  /// \param tableStoreId ID determines which kind of store is used. 0 -> Row-Store, 1-> Column-Store, 2-> PAX-Store
  /// \param rowCount number of rows of original table
  /// \param columnCount number of columns of original table
  /// \param lowerBound lower bound for table cell values
  /// \param upperBound upper bound for table cell values
  /// \param resultRowCount number of rows in queried table
  /// \param resultCpuTime CPU cycles needed for query
  /// \param resultRealTime time needed for query in ms
  void addBenchmark(int tableStoreId, unsigned rowCount, unsigned columnCount, T lowerBound, T upperBound, uint64_t resultRowCount,
                    uint64_t resultCpuTime, double resultRealTime) {
    std::tuple<int, unsigned, unsigned, T, T> key = {tableStoreId, rowCount, columnCount, lowerBound, upperBound};
    std::tuple<uint64_t, uint64_t, double> value = {resultRowCount, resultCpuTime, resultRealTime};

    result.insert(std::pair{key, value});
  }

  /// Print the benchmark results
  void printBenchmarkResult() {
    std::cout << "Benchmark result: " << std::endl << std::endl;

    std::map<std::tuple<int, unsigned, unsigned, T, T>, std::tuple<uint64_t, uint64_t, double>>::iterator iterator;

    // iterate over result map
    for (iterator = result.begin(); iterator != result.end(); iterator++) {
      // separate key and value of map
      std::tuple<int, unsigned, unsigned, T, T> key = iterator->first;
      std::tuple<uint64_t, uint64_t, double> value = iterator->second;

      // print key values
      std::cout << "Query parameters: " << std::endl;
      std::cout << "Table Store ID: " << std::get<0>(key) << ", ";
      std::cout << "Rows: " << std::get<1>(key) << ", ";
      std::cout << "Columns: " << std::get<2>(key) << ", ";
      std::cout << "Lower bound: " << std::get<3>(key) << ", ";
      std::cout << "Upper bound: " << std::get<4>(key) << std::endl;

      std::cout << "Benchmark results: " << std::endl;
      std::cout << "Rows: " << std::get<0>(value) << ", ";
      std::cout << "CPU cycles: " << std::get<1>(value) << ", ";
      std::cout << "Measured time: " << std::get<2>(value) << "ms" << std::endl << std::endl;
    }
  }

  /// Export the benchmark results to a file.
  /// \param filepath Path to store the file. Should also include the filename and the file ending (e.g. .csv)
  void exportBenchmarkResult(const std::string& filepath) {
    // open file
    std::ofstream file;
    file.open(filepath);

    // add header to file
    file << "tableStoreId,rowCount,columnCount,lowerBound,upperBound,resultRowCount,resultCpuTime,resultRealTime\n";

    // iterate over result map
    std::map<std::tuple<int, unsigned, unsigned, T, T>, std::tuple<uint64_t, uint64_t, double>>::iterator iterator;
    for (iterator = result.begin(); iterator != result.end(); iterator++) {
      // separate key and value of map
      std::tuple<int, unsigned, unsigned, T, T> key = iterator->first;
      std::tuple<uint64_t, uint64_t, double> value = iterator->second;

      // add values to file
      file << std::get<0>(key) << "," << std::get<1>(key) << "," << std::get<2>(key) << "," << std::get<3>(key) << "," << std::get<4>(key)
           << ",";
      file << std::get<0>(value) << "," << std::get<1>(value) << "," << std::get<2>(value) << "\n";
    }

    file.close();
  }
};