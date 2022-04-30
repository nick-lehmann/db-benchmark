#include <cstdint>
#include <stdexcept>
#include "Table.h"

#ifndef FILTER_H
#define FILTER_H

/**
This filter method requires the Table to be sorted over the selected attribute
*/
/*template<typename T, int S>
Table<T, S> * filter_binary_search(Table<T, S> &table, const int attribute, const T value) {
    if (attribute < 0 || attribute >= S) {
        throw std::invalid_argument("Error! Invalid attribute index!");
    }

    int index = table.data.size() / 2;
    int step = index / 2;

    while(step > 0) {
        if(table[index][attribute] < value) index -= step;
        else if(table[index][attribute] > value) index += step;
        else break;
    }
    if(step == 0) return new Table<T, S>();
    //I assume there are not that many tuples that match the value
    //(otherwise binary search to find the boundaries may be more efficient)
    //find lower boundary

    auto result = new Table<T, S>();

    while(table.data[--index][attribute] == value);
    while(table.data[++index][attribute] == value) {
        result->addTuple(table.getTuple(index));
    }
    return result;
}*/

template<typename T>
Table<T> * filter_basic(Table<T> &table, const int attribute, const int value) {
	if (attribute < 0 || attribute >= table.tupleSize) {
		throw std::invalid_argument("Error! Invalid attribute index!");
	}

	auto result = new Table<T>(table.tupleSize);
	for (int i = 0; i < table.data.size(); ++i) {
		if (table.data[i][attribute] == value) {
			// more efficient approach: use tuple-pointer and add the whole tuple to the result table
			result->addTuple(table[i]);
		}
	}
	return result;
}

#endif
