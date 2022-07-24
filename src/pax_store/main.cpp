#include <errno.h>
#include <immintrin.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <iterator>
#include <vector>

#include "Constants.h"
#include "Filters/All.h"
#include "Helper.h"
#include "Memory.h"
#include "Page.h"
#include "PaxTable.h"
#include "SIMD.h"
#include "Types.h"

using namespace std;

/**
 * Example of scalar operation
 */
template <typename T>
void exampleScalar() {
    const unsigned rows = 1000;
    const unsigned cols = 3;
    const T** data = TableHelper::generateRandomData<T>(cols, rows, 0, 100);
    PaxTable<T> table(cols, rows, data);

    std::vector<uint64_t> projection = {0, 1, 2};
    std::vector<Filters::Filter<T, SIMD::None>*> filters = {new Filters::LessEqual<T, SIMD::None>(0, 90),
                                                            new Filters::GreaterEqual<T, SIMD::None>(0, 10),
                                                            new Filters::Equal<T, SIMD::None>(0, 69)};

    {
        // Run query
        auto [result, rows, columns] = table.queryTable(projection, filters);
        cout << "Results:" << endl;
        for (unsigned row = 0; row < rows; row++) {
            for (unsigned column = 0; column < columns; column++) {
                cout << result[row][column] << " ";
            }
            cout << endl;
        }
        cout << "Count: " << rows << endl;
    }
}

/**
 * Example AVX operation
 */
template <typename T>
void exampleAVX() {
    const unsigned rows = 1000;
    const unsigned cols = 3;
    const T** data = TableHelper::generateRandomData<T>(cols, rows, 0, 100);
    PaxTable<T> table(cols, rows, data);

    std::vector<uint64_t> projection = {0, 1, 2};
    std::vector<Filters::Filter<T, SIMD::AVX512>*> filters = {new Filters::LessEqual<T, SIMD::AVX512>(0, 90),
                                                              new Filters::GreaterEqual<T, SIMD::AVX512>(0, 10),
                                                              new Filters::Equal<T, SIMD::AVX512>(0, 69)};

    {
        // Run query
        auto [result, rows, columns] = table.queryTable(projection, filters);
        cout << "Results:" << endl;
        for (unsigned row = 0; row < rows; row++) {
            for (unsigned column = 0; column < columns; column++) {
                cout << result[row][column] << " ";
            }
            cout << endl;
        }
        cout << "Count: " << rows << endl;
    }
}

int main() {
    using T = uint64_t;

    cout << "Scalar" << endl;
    exampleScalar<T>();

    cout << endl;
    cout << "AVX" << endl;
    exampleAVX<T>();

    return 0;
}
