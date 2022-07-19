#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "../column_store/ColumnStoreTable.h"
#include "../pax_store/PaxTable.h"
#include "../row_store/BaseTable.h"
#include "Constants.h"
#include "Filters/All.h"
#include "Helper.h"
#include "dataset.h"
#include "sqlite.h"

/**
 * Test Scenarios:
 * 1) Variant     // results independent
 * 2) Data type   // results independent
 * 3) Number of Filters
 * 4) Number of Rows
 * 5) Number of Columns
 * 6) Projection
 */

template <typename T>
using QueryResult = std::tuple<T **, uint64_t, uint64_t>;

template <typename T, SIMD Variation>
struct Scenario {
    std::string name;
    unsigned numberOfAttributes, numberOfRows;
    std::vector<T> projection;
    std::vector<Filters::Filter<T, Variation> *> filters;
};

template <typename T>
void printResult(QueryResult<T> &result, std::string name, bool showTable = false) {
    auto [data, rows, columns] = result;
    cout << name << ":\t" << rows << endl;

    if (!showTable) return;
    for (unsigned row = 0; row < rows; row++) {
        for (unsigned column = 0; column < columns; column++) {
            cout << data[row][column] << " ";
        }
        cout << endl;
    }
}

template <typename T, SIMD Variation>
Scenario<T, Variation> createScenario(std::string name, unsigned numberOfAttributes, unsigned numberOfRows, std::vector<T> data,
                                      std::vector<Filters::Filter<T, Variation> *> filters) {
    return Scenario<T, Variation>{name, numberOfAttributes, numberOfRows, data, filters};
}

template <typename T, SIMD Variation>
void checkScenario(const Scenario<T, Variation> &scenario) {
    auto [name, numberOfAttributes, numberOfRows, projection, filters] = scenario;
    cout << "Checking scenario:\t" << name << endl;
    const T **data = TableHelper::generateRandomData<T>(numberOfAttributes, numberOfRows, 0, 100);

    // Row
    {
        RowStore::BaseTable<T, PAGE_SIZE> table(numberOfAttributes, numberOfRows, data);
        auto result = table.queryTable(projection, filters);
        printResult(result, "Row");
    }

    // Column
    {
        ColumnStore::Table<T> table(numberOfAttributes, numberOfRows, data);
        auto result = table.queryTable(projection, filters);
        printResult(result, "Column");
    }

    // Pax
    {
        PaxTable<T> table(numberOfAttributes, numberOfRows, data);
        auto result = table.queryTable(projection, filters);
        printResult(result, "Pax");
    }

    cout << endl;
}

template <typename T>
void scenarioScalar() {
    // 1 Filter
    {
        // clang-format off
        struct Scenario<T, SIMD::None> scenario = {
            .name = "Scalar Equal", 
            .numberOfAttributes = 3, 
            .numberOfRows = 600, 
            .projection = {0, 1, 2},
            .filters = { new Filters::Equal<T, SIMD::None>(0, 8) }
        };
        checkScenario(scenario);
    }
    {
        // clang-format off
        struct Scenario<T, SIMD::None> scenario = {
          .name = "Scalar Not Equal", 
          .numberOfAttributes = 3, 
          .numberOfRows = 600, 
          .projection = {0, 1, 2}, 
          .filters = {new Filters::NotEqual<T, SIMD::None>(1, 69)}
        };
        checkScenario(scenario);
    }
    {
        // clang-format off
        struct Scenario<T, SIMD::None> scenario = {
          .name = "Scalar Less Than", 
          .numberOfAttributes = 3, 
          .numberOfRows = 600, 
          .projection = {0, 1, 2}, 
          .filters = {new Filters::LessThan<T, SIMD::None>(0, 42)}
        };
        checkScenario(scenario);
    }
    {
        // clang-format off
        struct Scenario<T, SIMD::None> scenario = {
          .name = "Scalar Less Equal", 
          .numberOfAttributes = 3, 
          .numberOfRows = 600, 
          .projection = {0, 1, 2}, 
          .filters = {new Filters::LessEqual<T, SIMD::None>(1, 80)}
        };
        checkScenario(scenario);
    }
    {
        // clang-format off
        struct Scenario<T, SIMD::None> scenario = {
          .name = "Scalar Greater Than", 
          .numberOfAttributes = 3, 
          .numberOfRows = 600, 
          .projection = {0, 1, 2}, 
          .filters = {new Filters::GreaterThan<T, SIMD::None>(0, 90)}
        };
        checkScenario(scenario);
    }
    {
        // clang-format off
        struct Scenario<T, SIMD::None> scenario = {
          .name = "Scalar Greater Equal", 
          .numberOfAttributes = 3, 
          .numberOfRows = 600, 
          .projection = {0, 1, 2}, 
          .filters = {new Filters::GreaterEqual<T, SIMD::None>(0, 95)}
        };
        checkScenario(scenario);
    }

    // 2 Filter
    {
        // clang-format off
        struct Scenario<T, SIMD::None> scenario = {
          .name = "Scalar Bigger and Smaller",
          .numberOfAttributes = 3,
          .numberOfRows = 600,
          .projection = {0, 1, 2},
          .filters = {
            new Filters::GreaterThan<T, SIMD::None>(0, 40), 
            new Filters::LessThan<T, SIMD::None>(0, 60)
          }
        };
        checkScenario(scenario);
    }
    {
        // clang-format off
        struct Scenario<T, SIMD::None> scenario = {
          .name = "Scalar Not Equal & Bigger",
          .numberOfAttributes = 3,
          .numberOfRows = 600,
          .projection = {0, 1, 2},
          .filters = {
            new Filters::NotEqual<T, SIMD::None>(2, 80), 
            new Filters::NotEqual<T, SIMD::None>(1, 69),
            new Filters::GreaterThan<T, SIMD::None>(0, 50)
          }
        };
        checkScenario(scenario);
    }
    {
        struct Scenario<T, SIMD::None> scenario = {
          .name = "Scalar Equal and Greater",
          .numberOfAttributes = 3,
          .numberOfRows = 1000,
          .projection = {0, 1, 2},
          .filters = {
            new Filters::GreaterThan<T, SIMD::None>(0, 50), 
            new Filters::Equal<T, SIMD::None>(1, 69)
          }
        };
        checkScenario(scenario);
    }
}


// int main() {
    // using T = std::uint64_t;
    // scenarioScalar<T>();
    // return 0;
// }

int main(int argc, char const *argv[]) {
    using T = std::uint64_t;

    const auto tableName = "data";
    const auto dbPath = "test.db";
    const auto dataPath = "/workspaces/db-benchmark/data.csv";

    // Dimensions
    const auto numberOfAttributes = 50;
    const auto numberOfRows = 100000;

    const T **data = TableHelper::generateRandomData<T>(numberOfAttributes, numberOfRows, 0, 100);

    std::vector<std::string> columnNames = getColumnNames(numberOfAttributes);

    // Create table
    std::string createStatement = createTableStatement(tableName, columnNames, "INTEGER");

    std::remove(dbPath);

    createTable(dbPath, createStatement);
    importPSV(dbPath, tableName, dataPath);

    // Test Query
    std::vector<T> projection = {0, 1, 2};
    std::vector<Filters::Filter<T, SIMD::None> *> filters = {
        new Filters::GreaterThan<T, SIMD::None>(0, 40)
    };

    auto query = buildSQLQuery<T, SIMD::None>(projection, filters, tableName, numberOfRows);
    cout << "Query: \n" << query << endl;

    const auto resultColumns = 3;
    std::string output = runQuery(dbPath, query);

    auto result = parseQueryResult<T, resultColumns>(output);

    for (const auto& line : result) {
        for (const auto& value : line) {
            std::cout << value << " ";
        }
        std::cout << std::endl; 
    }

    std::remove(dbPath);
    return 0;
}