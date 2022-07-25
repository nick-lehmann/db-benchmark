#pragma once

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