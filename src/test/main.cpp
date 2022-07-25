#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

#include "Constants.h"
#include "Filters/All.h"
#include "Helper.h"
#include "Runner.h"

const uint64_t numberOfAttributes = 50;
const uint64_t numberOfRows = 10000;

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

    const std::uint64_t **data = TableHelper::generateRandomData<uint64_t>(numberOfAttributes, numberOfRows, 0, 100);
    // const std::uint32_t **data_32 = TableHelper::generateRandomData<uint32_t>(numberOfAttributes, numberOfRows, 0, 100);
    Database db = setupEnvironment(data, numberOfAttributes, numberOfRows);

    run<uint64_t>(db, data);
    // run<uint32_t>(db, data_32);
}
