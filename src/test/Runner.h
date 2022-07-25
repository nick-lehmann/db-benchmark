#pragma once

#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>

#include "../column_store/ColumnStoreTable.h"
#include "../pax_store/PaxTable.h"
#include "../row_store/BaseTable.h"
#include "Database.h"
#include "Dataset.h"
#include "Filters/All.h"
#include "Helper.h"
#include "ITable.h"
#include "Runner.h"
#include "helpers/Assert.h"
#include "helpers/Projection.h"
#include "helpers/QueryResult.h"

const auto dbPath = "test.db";
const auto datasetPath = "/workspaces/db-benchmark/data.csv";

// Test inputs
std::vector<uint64_t> rows = {8, 16, 100, 1000, 10000, 1000000};
std::vector<uint64_t> columns = {5, 10};
std::vector<std::vector<uint64_t>> projections = {
    {0, 1, 2},
    // createProjection(0, numberOfAttributes, 1),
    // createProjection(0, numberOfAttributes, 2),
    // createProjection(0, 5, 1),
    // createProjection(0, 2, 1),
    // createProjection(0, 1, 1),
};

std::vector<std::vector<std::tuple<unsigned, uint64_t>>> filterInputs = {
    {{0, 10}},
};

template <typename T>
void run(Database &db, const T **data) {
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
                        vector<Filters::Filter<T, SIMD::None> *> filters = {new Filters::GreaterThan<T, SIMD::None>(index, value)};

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
                            // printResult(result);
                        }
                    }
                }
            }
        }
    }
}

Database setupEnvironment(const uint64_t **data, uint64_t numberOfAttributes, uint64_t numberOfRows) {
    writeDatasetToFile<uint64_t>(datasetPath, numberOfAttributes, numberOfRows, data);
    Database db("./test.db", datasetPath, numberOfRows, numberOfAttributes);
    return db;
}