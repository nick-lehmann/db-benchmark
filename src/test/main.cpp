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
#include "ITable.h"
#include "dataset.h"
#include "sqlite.h"

const auto tableName = "data";
const auto dbPath = "test.db";
const auto datasetPath = "/workspaces/db-benchmark/data.csv";

const uint64_t numberOfAttributes = 50;
const uint64_t numberOfRows = 10000;

template <typename T>
using QueryResult = std::tuple<T **, uint64_t, uint64_t>;

template <typename T>
void printResult(QueryResult<T> &result) {
    auto [data, rows, columns] = result;
    for (unsigned row = 0; row < rows; row++) {
        for (unsigned column = 0; column < columns; column++) {
            cout << data[row][column] << " ";
        }
        cout << endl;
    }
}

template <typename T, unsigned width>
tuple<bool, string> assertResult(QueryResult<T> result, Database::Result<T, width> expected) {
    auto [data, rows, columns] = result;

    if (rows != expected.size()) {
        ostringstream msg;
        msg << "Expected " << expected.size() << " rows but got " << rows;
        return {false, msg.str()};
    }

    for (unsigned row = 0; row < rows; row++) {
        for (unsigned column = 0; column < columns; column++) {
            if (data[row][column] != expected[row][column]) {
                ostringstream msg;
                msg << "Expected at row " << row << " and column " << column << " a " << expected[row][column] << " but got "
                    << data[row * columns + column] << "(" << row << "," << column << ")";
                return {false, msg.str()};
            }
        }
    }

    return {true, ""};
}

Database setupEnvironment(const uint64_t **data) {
    writeDatasetToFile<uint64_t>(datasetPath, numberOfAttributes, numberOfRows, data);
    Database db("./test.db", datasetPath, numberOfRows, numberOfAttributes);
    return db;
}

template <typename T, SIMD Variation>
struct Scenario {
    std::string name;
    unsigned numberOfAttributes, numberOfRows;
    std::vector<T> projection;
    std::vector<Filters::Filter<T, Variation> *> filters;
};

std::vector<uint64_t> createProjection(uint64_t lower, uint64_t upper, uint64_t step) {
    std::vector<uint64_t> projection;
    for (uint64_t i = lower; i < upper; i += step) {
        projection.push_back(i);
    }
    return projection;
}

/**
 * Test Scenarios:
 * 1) Variant     // results independent
 * 2) Data type   // results independent
 * 3) Number of Filters & Types of filter
 * 4) Number of Rows
 * 5) Number of Columns
 * 6) Projection
 */

int main() {
    using T = std::uint64_t;

    const T **data = TableHelper::generateRandomData<T>(numberOfAttributes, numberOfRows, 0, 100);
    Database db = setupEnvironment(data);

    // Test inputs
    vector<uint64_t> rows = {8, 16, 100, 1000, 10000, 1000000};
    vector<uint64_t> columns = {5, 10, numberOfAttributes};
    vector<vector<uint64_t>> projections = {
        {0, 1, 2},
        // createProjection(0, numberOfAttributes, 1),
        // createProjection(0, numberOfAttributes, 2),
        // createProjection(0, 5, 1),
        // createProjection(0, 2, 1),
        // createProjection(0, 1, 1),
    };

    vector<vector<tuple<unsigned, uint64_t>>> filterInputs = {
        {{0, 10}},
    };

    for (auto &row : rows) {
        for (auto &column : columns) {
            RowStore::BaseTable<T, PAGE_SIZE> rowTable(column, row, data);
            ColumnStore::Table<T> columnTable(column, row, data);
            PaxTable<T> paxTable(column, row, data);

            array<Tables::ITable<T> *, 3> tables = {&rowTable, &columnTable, &paxTable};
            array<string, 3> tableNames = {"RowStore", "ColumnStore", "PaxTable"};

            for (unsigned tableIndex = 0; tableIndex < tables.size(); tableIndex++) {
                for (auto &projection : projections) {
                    for (auto &filterInput : filterInputs) {
                        auto [index, value] = filterInput[0];
                        std::vector<Filters::Filter<T, SIMD::None> *> filters = {new Filters::GreaterThan<T, SIMD::None>(index, value)};

                        // Fetch expected result from sqlite3.
                        auto expected = db.query<T, SIMD::None, 3>(projection, filters, (unsigned)row);

                        // Query table.
                        auto table = tables[tableIndex];
                        auto result = table->queryTable(projection, filters);

                        cout << tableNames[tableIndex] << " (" << row << "," << column << "): ";
                        auto [success, msg] = assertResult<T, 3>(result, expected);

                        if (success) {
                            cout << "success" << endl;
                        } else {
                            cout << "failed: " << msg << endl;
                            printResult(result);
                        }
                    }
                }
            }
        }
    }
}