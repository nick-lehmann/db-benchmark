#include <array>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <cstdarg>
#include <iostream>
#include <stdexcept>

#ifndef TABLE_H
#define TABLE_H

template<typename T>
class Table {
private:
    const bool baseTable;
public:
    const int tupleSize;
    std::vector<T *> data;

    Table(const int tupleSize) : baseTable(false), tupleSize(tupleSize) {}
    Table(const int tupleSize, bool baseTable) : baseTable(baseTable), tupleSize(tupleSize) {}
    ~Table() {
        //only delete the tuple if table is a base table
        if(baseTable) {
            for(int i = 0; i < data.size(); ++i) {
                free(data[i]);
            }
        }
    }

    int size() {
        return data.size();
    }

    int createEmptyTuple() {
        if(!baseTable) throw std::logic_error("Error! Only BaseTables can create Tuples!");

        data.push_back(
            static_cast<T *> (calloc(tupleSize, sizeof(T)))
        );
        return data.size() - 1;
    }

    int createTuple(const int count, ...) {
        if(!baseTable) throw std::logic_error("Error! Only BaseTables can create Tuples!");

        if(count > tupleSize) return -1;

        int index = createEmptyTuple();

        va_list args;
        va_start(args, count);
        for(int i = 0; i < count; ++i) {
            data[index][i] = va_arg(args, T);
        }
        va_end(args);

        return index;
    }

    T * operator[](const int index) {
        return data[index];
    }
    T * getTuple(const int index) {
        return data[index];
    }

    int addTuple(T * tuple) {
        data.push_back(tuple);
        return data.size() - 1;
    }

    void print() {
    	// traverse rows
    	for(int i = 0; i < data.size(); ++i) {
    		// traverse attributes
    		for(int j = 0; j < tupleSize; ++j) {
    			std::cout << data[i][j];
    			// append separator (except for last attribute)
    			if(j < (tupleSize - 1)) std::cout << " | ";
    		}
    		std::cout << std::endl;
    	}

    	std::cout << std::endl;
    }
};

template<typename T>
Table<T> * createSortedTestTable(const int tupleSize, const int tupleCount, int duplicates) {
	auto table = new Table<T>(tupleSize, true);

    T number = 0;
    int stop = tupleCount + duplicates;
	for(int i = 0; i < stop; ++i) {
        table->print();
		// append empty tuple to table
		table->createEmptyTuple();
		// fill appended tuple with data
		for(int j = 0; j < tupleSize; ++j){
			table->getTuple(i)[j] = number++;
        }
        //creates duplicates
        if(duplicates > 0) {
            number-=tupleSize;
            table->createEmptyTuple();
            ++i;
            for(int j = 0; j < tupleSize; ++j){
    			table->getTuple(i)[j] = number++;
            }
            --duplicates;
        }
	}
	return table;
}

//deprecated
std::vector<uint64_t *> * createEmptyTable() {
	return new std::vector<uint64_t *>();
}

//deprecated
uint64_t * createTuple(int tupleSize) {
	return static_cast<uint64_t *>(calloc(tupleSize, sizeof(uint64_t)));
}

//deprecated
std::vector<uint64_t *> * createTestTable(int tupleCount, int tupleSize) {
	auto table = createEmptyTable();

	for(int i = 0; i < tupleCount; ++i) {
		// append empty tuple to table
		table->push_back(createTuple(tupleSize));
		// fill appended tuple with data
		for(int j = 0; j < tupleSize; ++j){
			(* table)[i][j] = (i * tupleSize) + j;
		}
	}

	return table;
}

//deprecated
void printTable(const std::vector<uint64_t *> &table, int tupleSize) {
	// traverse rows
	for(int i = 0; i < static_cast<int>(table.size()); ++i) {
		// traverse attributes
		for(int j = 0; j < tupleSize; ++j) {
			std::cout << table[i][j];
			// append separator (except for last attribute)
			if(j < (tupleSize - 1)) std::cout << " | ";
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;
}





















#endif //
