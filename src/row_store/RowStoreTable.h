#pragma once

#include <array>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <cstdarg>
#include <iostream>
#include <stdexcept>

#include "Table.h"
#include "Filter.h"
#include "Projection.h"

template<typename T>
class RowStoreTable : public Table<T> {

public:
    std::vector<T *> data;

public:
    RowStoreTable(unsigned numAttributes, unsigned numRows, const T **initialData)
        : Table<T>(numAttributes, numRows, initialData) {}

    ~RowStoreTable() override {
        //only delete the tuple if table is a base table
        for(uint64_t i = 0; i < data.size(); ++i) {
            free(data[i]);
        }
    }

    void addRow(T *row) {
        data.push_back(row);
        this->numberOfRows = data.size();
    }

    T *getRow(unsigned rowIndex) override {
        return data[rowIndex];
    }

    T **query_table(std::vector<unsigned> &projection, std::vector<Filter<T>> &filters) override {
        // TODO
        return nullptr;
    }

    uint64_t query_count(std::vector<unsigned> &projection, std::vector<Filter<T>> &filters) override {
        // TODO
        return -1;
    }

    bool columnIndicesValid(std::vector<int> columnIndices, int tupleSize) {
    	// check for each column index if it is in range of 0 and tupleSize
    	for (uint64_t i = 0; i < columnIndices.size(); i++) {
    		if (columnIndices[i] < 0 || columnIndices[i] >= tupleSize) {
    			return false;
    		}
    	}
    	return true;
    }

    RowStoreTable<T> *projection(RowStoreTable<T> &table, std::vector<int> &projection) {
        if (!columnIndicesValid(projection, table.numberOfAttributes)) {
            throw std::invalid_argument("Error! Invalid column indices!");
        }

        // create empty table
        //TODO result is if type intermediate result, which we need to implement first
        auto result = new RowStoreTable<T>(projection.size(), true);

        // iterate over given table tuples
        for (int i = 0; i < table.data.size(); i++) {
            // create empty (temporary) tuple
            auto tuple = (T *) calloc(projection.size(), sizeof(T));

            // fill tuple with data from every row of table
            for (int j = 0; j < projection.size(); j++) {
                tuple[j] = table[i][projection[j]];
            }
            result->addRow(tuple);
        }

        return result;
    }

    RowStoreTable<T> * filter_basic(RowStoreTable<T> &table, Filter<T> *predicate) {
    	if (predicate->index < 0 || predicate->index >= table.numberOfAttributes) {
    		throw std::invalid_argument("Error! Invalid attribute index!");
    	}

        //TODO needs to be intermediate table (see above)
    	RowStoreTable<T> *result = new RowStoreTable<T>(table.numberOfAttributes, 0);
    	for (int i = 0; i < table.data.size(); ++i) {
    		if (predicate->match(table[i][predicate->index])) {
    			// more efficient approach: use tuple-pointer and add the whole tuple to the result table
                // TODO create actual
    			result->addRow(table[i]);
    		}
    	}
    	return result;
    }

    Table<T>* query(std::vector<int> &projectionColumns, std::vector<Filter<T>  *> &filters) {
        //TODO: for highest performance do projections as early as possible
        //(AQP WiSe19/20, Lecture 8 Query Processing, Slide 23)

        RowStoreTable<T> *filterResult = this;
        // apply all filters on the table
        for(int i = 0; i < filters.size(); ++i) {
            auto oldResult = filterResult;
            filterResult = filter_basic(*filterResult, filters[i]);

            if(oldResult != this) {
                delete oldResult;
            }
        }
        // project to the desired columns
        RowStoreTable<T> * projectionResult = projection(*filterResult, projectionColumns);
        return projectionResult;
    }
};

template<typename T>
RowStoreTable<T> * createSortedTestTable(const int tupleSize, const int tupleCount, int duplicates) {
    //TODO needs to be intermediate table (see above)
	RowStoreTable<T> *table = new RowStoreTable<T>(tupleSize, true);

	for(int i = 0; i < tupleCount; ++i) {
    //create empty tuple
    T *tuple = (T *) malloc(tupleSize * sizeof(T));
    // fill tuple with data
    for(int j = 0; j < tupleSize; ++j){
      tuple[j] = i*tupleSize + j;
    }
    // append tuple to table
    table->addRow(tuple);
	}
	return table;
}


bool columnIndicesValid(std::vector<int> columnIndices, int tupleSize) {
	// check for each column index if it is in range of 0 and tupleSize
	for (uint64_t i = 0; i < columnIndices.size(); i++) {
		if (columnIndices[i] < 0 || columnIndices[i] >= tupleSize) {
			return false;
		}
	}
	return true;
}

/*template <typename T>
RowStoreTable<T> *projection(RowStoreTable<T> &table, std::vector<int> &projection) {
    if (!columnIndicesValid(projection, table.numberOfAttributes)) {
        throw std::invalid_argument("Error! Invalid column indices!");
    }

    // create empty table
    auto result = new RowStoreTable<T>(projection.size(), true);

    // iterate over given table tuples
    for (int i = 0; i < table.data.size(); i++) {
        // create empty (temporary) tuple
        auto tuple = (T *) calloc(projection.size(), sizeof(T));

        // fill tuple with data from every row of table
        for (int j = 0; j < projection.size(); j++) {
            tuple[j] = table[i][projection[j]];
        }
        result->addRow(tuple);
    }

    return result;
}*/

/*template <typename T>
RowStoreTable<T> * filter_basic(RowStoreTable<T> &table, Filter<T> *predicate) {
	if (predicate->index < 0 || predicate->index >= table.numberOfAttributes) {
		throw std::invalid_argument("Error! Invalid attribute index!");
	}

	RowStoreTable<T> *result = new RowStoreTable<T>(table.numberOfAttributes, 0);
	for (int i = 0; i < table.data.size(); ++i) {
		if (predicate->match(table[i]+predicate->index)) {
			// more efficient approach: use tuple-pointer and add the whole tuple to the result table
			result->addRow(table[i]);
		}
	}
	return result;
}*/
