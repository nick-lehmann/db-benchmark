#include <cstdint>
#include <stdexcept>
#include <algorithm>
#include "Table.h"

#ifndef PROJECTION_H
#define PROJECTION_H

bool columnIndicesValid(int columnIndices[], const int tupleSize) {
	// check for each column index if it is in range of 0 and tupleSize
	for (int i = 0; i < sizeof(columnIndices) / sizeof(columnIndices[0]); i++) {
		if (columnIndices[i] < 0 || columnIndices[i] >= tupleSize) {
			return false;
		}
	}
	return true;
}

template<typename T>
Table<T> * projection(Table<T> &table, int columnIndices[], int newTupleSize) {
    if (!columnIndicesValid(columnIndices, table.tupleSize)) {
        throw std::invalid_argument("Error! Invalid column indices!");
    }

    // create empty table
    auto result = new Table<T>(newTupleSize, true);

    // iterate over given table tuples
    for (int i = 0; i < table.size(); i++) {
        // create empty (temporary) tuple
        auto tuple = result->getTuple(result->createEmptyTuple());

        // fill tuple with data from every row of table
        for (int j = 0; j < newTupleSize; j++) {
            tuple[j] = table[i][columnIndices[j]];
        }
    }

    return result;
}

//deprecated
std::vector<uint64_t *> * projection(const std::vector<uint64_t *> &table, int columnIndices[], int tupleSize) {
	if (!columnIndicesValid(columnIndices, tupleSize)) {
		throw std::invalid_argument("Error! Invalid column indices!");
	}

	// get the tuple size from given column indices
	int projectionTupleSize = sizeof(columnIndices) / sizeof(columnIndices[0]);

	// create empty table
	auto projectionResultTable = createEmptyTable();

	// iterate over given table tuples
	for (int i = 0; i < table.size(); i++) {
		// create empty (temporary) tuple
		uint64_t* tempTuple = createTuple(projectionTupleSize);

		// fill tuple with data from every row of table
		for (int j = 0; j < projectionTupleSize; j++) {
			tempTuple[j] = (table)[i][columnIndices[j]];
		}

		// add tuple to result table
		projectionResultTable->push_back(tempTuple);
	}

	return projectionResultTable;
}

#endif
