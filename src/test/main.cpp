#include <iostream>
#include <iterator>
#include <vector>

#include "../column_store/ColumnStoreTable.h"
#include "../pax_store/PaxTable.h"
#include "../row_store/BaseTable.h"
#include "Constants.h"
#include "Filters/All.h"
#include "Helper.h"

template <typename T>
using QueryResult = std::tuple<T **, uint64_t, uint64_t>;

template <typename T, SIMD Variation>
struct Scenario {
    std::string name;
    unsigned numberOfAttributes, numberOfRows;
    std::vector<T> data;
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
void scenarioScalarBase() {
    unsigned numberOfRows = 600;
    unsigned numberOfAttributes = 3;
    std::vector<T> projection = {0, 1, 2};
    std::vector<Filters::Filter<T, SIMD::None> *> filters = {new Filters::Equal<T, SIMD::None>(0, 8)};

    Scenario<T, SIMD::None> scenario = {"Base Equal", numberOfAttributes, numberOfRows, projection, filters};
    checkScenario(scenario);
}
template <typename T>
void scenarioScalarTwoEqual() {
    unsigned numberOfRows = 600;
    unsigned numberOfAttributes = 3;
    std::vector<T> projection = {0, 1, 2};
    std::vector<Filters::Filter<T, SIMD::None> *> filters = {new Filters::Equal<T, SIMD::None>(0, 8),
                                                             new Filters::Equal<T, SIMD::None>(1, 69)};

    Scenario<T, SIMD::None> scenario = {"2 Equal", numberOfAttributes, numberOfRows, projection, filters};
    checkScenario(scenario);
}

template <typename T>
void scenarioLT() {
    unsigned numberOfRows = 1000;
    unsigned numberOfAttributes = 3;
    std::vector<T> projection = {0, 1, 2};
    std::vector<Filters::Filter<T, SIMD::None> *> filters = {new Filters::LessThan<T, SIMD::None>(0, 8)};

    Scenario<T, SIMD::None> scenario = {"LEQ", numberOfAttributes, numberOfRows, projection, filters};
    checkScenario(scenario);
}

template <typename T>
void scenarioScalarNotEqual1() {
    unsigned numberOfRows = 600;
    unsigned numberOfAttributes = 3;
    std::vector<T> projection = {0, 1, 2};
    std::vector<Filters::Filter<T, SIMD::None> *> filters = {new Filters::NotEqual<T, SIMD::None>(0, 8)};

    Scenario<T, SIMD::None> scenario = {"Not Equal", numberOfAttributes, numberOfRows, projection, filters};
    checkScenario(scenario);
}

template <typename T>
void scenarioScalar() {
    // 1 Filter
    {
        Scenario<T, SIMD::None> scenario = {"Scalar Equal", 3, 600, {0, 1, 2}, {new Filters::Equal<T, SIMD::None>(0, 8)}};
        checkScenario(scenario);
    }
    {
        Scenario<T, SIMD::None> scenario = {"Scalar Not Equal", 3, 600, {0, 1, 2}, {new Filters::NotEqual<T, SIMD::None>(1, 69)}};
        checkScenario(scenario);
    }
    {
        Scenario<T, SIMD::None> scenario = {"Scalar Less Than", 3, 600, {0, 1, 2}, {new Filters::LessThan<T, SIMD::None>(0, 42)}};
        checkScenario(scenario);
    }
    {
        Scenario<T, SIMD::None> scenario = {"Scalar Less Equal", 3, 600, {0, 1, 2}, {new Filters::LessEqual<T, SIMD::None>(1, 80)}};
        checkScenario(scenario);
    }
    {
        Scenario<T, SIMD::None> scenario = {"Scalar Greater Than", 3, 600, {0, 1, 2}, {new Filters::GreaterThan<T, SIMD::None>(0, 90)}};
        checkScenario(scenario);
    }
    {
        Scenario<T, SIMD::None> scenario = {"Scalar Greater Equal", 3, 600, {0, 1, 2}, {new Filters::GreaterEqual<T, SIMD::None>(0, 95)}};
        checkScenario(scenario);
    }

    // 2 Filter
    {
        Scenario<T, SIMD::None> scenario = {"Scalar Bigger and Smaller",
                                            3,
                                            600,
                                            {0, 1, 2},
                                            {new Filters::GreaterThan<T, SIMD::None>(0, 40), new Filters::LessThan<T, SIMD::None>(0, 60)}};
        checkScenario(scenario);
    }
    {
        Scenario<T, SIMD::None> scenario = {"Scalar Not Equal & Bigger",
                                            3,
                                            600,
                                            {0, 1, 2},
                                            {new Filters::NotEqual<T, SIMD::None>(2, 80), new Filters::NotEqual<T, SIMD::None>(1, 69),
                                             new Filters::GreaterThan<T, SIMD::None>(0, 50)}};
        checkScenario(scenario);
    }
    {
        Scenario<T, SIMD::None> scenario = {"Scalar Equal and Greater",
                                            3,
                                            1000,
                                            {0, 1, 2},
                                            {new Filters::GreaterThan<T, SIMD::None>(0, 50), new Filters::Equal<T, SIMD::None>(1, 69)}};
        checkScenario(scenario);
    }
}

int main() {
    using T = std::uint64_t;
    scenarioScalar<T>();
    return 0;
}
