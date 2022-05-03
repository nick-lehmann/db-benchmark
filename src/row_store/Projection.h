#pragma once
#include <cstdint>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include "RowStoreTable.h"

bool columnIndicesValid(std::vector<int> columnIndices, int tupleSize) {
	// check for each column index if it is in range of 0 and tupleSize
	for (uint64_t i = 0; i < columnIndices.size(); i++) {
		if (columnIndices[i] < 0 || columnIndices[i] >= tupleSize) {
			return false;
		}
	}
	return true;
}

template<typename T>
RowStoreTable<T> * projection(RowStoreTable<T> &table, std::vector<int> projection) {
    if (!columnIndicesValid(projection, table.numberOfAttributes)) {
        throw std::invalid_argument("Error! Invalid column indices!");
    }

    // create empty table
    auto result = new Table<T>(projection.size(), true);

    // iterate over given table tuples
    for (int i = 0; i < table.size(); i++) {
        // create empty (temporary) tuple
        auto tuple = result->getTuple(result->createEmptyTuple());

        // fill tuple with data from every row of table
        for (int j = 0; j < projection.size(); j++) {
            tuple[j] = table[i][projection[j]];
        }
    }

    return result;
}
