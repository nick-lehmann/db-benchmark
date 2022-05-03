#pragma once
#include <array>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <cstdarg>
#include <iostream>
#include <stdexcept>

#include "../Table.h"
#include "Filter.h"
#include "Projection.h"



template<typename T>
class RowStoreTable : public Table<T> {

private:
    //true: table owns the original pointer to a tuple
    //      (and must free the memory behind that pointer when deconstucted)
    //false:table only contains copies of tuples
    //      (and must not free the memory behind that pointer)
    const bool baseTable;
public:
    std::vector<T *> data;

    RowStoreTable(const int numberOfAttributes, const int numberOfRows) :
            Table<T>(numberOfAttributes, numberOfRows), baseTable(false) {}
    RowStoreTable(const int numberOfAttributes, const int numberOfRows, bool baseTable) :
            Table<T>(numberOfAttributes, numberOfRows), baseTable(baseTable) {}
    ~RowStoreTable() {
        //only delete the tuple if table is a base table
        if(baseTable) {
            for(int i = 0; i < data.size(); ++i) {
                free(data[i]);
            }
        }
    }

    void addRow(T *row) {
        data.push_back(row);
    }

    T *getRow(const int rowIndex) {
        return data[rowIndex];
    };
    T *getCell(const int rowIndex, const int columnIndex) {
        return data[rowIndex][columnIndex];
    };

    Table<T>* query(std::vector<int> &projection, std::vector<Filter<T>  *> &filters) {
        //TODO: for highest performance do projections as early as possible
        //(AQP WiSe19/20, Lecture 8 Query Processing, Slide 23)

        Table<T> filter_result = this;
        // apply all filters on the table
        for(int i = 0; i < filters.size(); ++i) {
            filter_result = filter_basic(filter_result);
        }
        // project to the desired columns
        return projection(filter_result, projection);
    }
};

template<typename T>
RowStoreTable<T> * createSortedTestTable(const int tupleSize, const int tupleCount, int duplicates) {
	RowStoreTable<T> *table = new RowStoreTable<T>(tupleSize, true);

    T number = 0;
    int stop = tupleCount + duplicates;
	for(int i = 0; i < stop; ++i) {
        //create empty tuple
        T *tuple = malloc(tupleSize * sizeof(T));
		// fill tuple with data
		for(int j = 0; j < tupleSize; ++j){
			tuple[j] = number++;
        }
        // append tuple to table
        table.addRow(tuple);
        //creates duplicates
        if(duplicates > 0) {
            number-=tupleSize;
            T *dtuple = malloc(tupleSize * sizeof(T));
            ++i;
            for(int j = 0; j < tupleSize; ++j){
    			tuple[j] = number++;
            }
            table.addRow(dtuple);
            --duplicates;
        }
	}
	return table;
}
