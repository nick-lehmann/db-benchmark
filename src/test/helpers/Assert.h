#include <string>

#include "../Database.h"
#include "QueryResult.h"

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