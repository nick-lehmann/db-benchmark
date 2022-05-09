#pragma once
#include <cstdint>
#include <stdexcept>
#include "Filters.h"
#include "RowStoreTable.h"

/*template <typename T>
RowStoreTable<T> * filter_basic(RowStoreTable<T> &table, Filter<T> *predicate) {
	if (predicate->index < 0 || predicate->index >= table.numberOfAttributes) {
		throw std::invalid_argument("Error! Invalid attribute index!");
	}

	RowStoreTable<T> *result = new RowStoreTable<T>(table.numberOfAttributes, 0);
	for (int i = 0; i < table.data.size(); ++i) {
		if (predicate->match(table[i][predicate->index])) {
			// more efficient approach: use tuple-pointer and add the whole tuple to the result table
			result->addTuple(table[i]);
		}
	}
	return result;
}*/
