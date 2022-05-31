#include <iostream>

#include "ColumnStoreTable.h"
#include "ColumnStoreTable_AVX.h"

#include "Helper.h"

#define COLUMNS 5
#define ROWS 2048

// taken from https://www.tutorialspoint.com/cplusplus-equivalent-of-instanceof
template<typename Base, typename T>
inline bool instanceof(const T *ptr) {
    return dynamic_cast<const Base*>(ptr) != nullptr;
}

/// Small helper function for debugging output. It is not very pretty, it will be removed sooner or later.
template <typename T>
void printFilterInfo(std::vector<Filters::AVX::Filter<T>*> filters) {
    std::cout << "Filters ";
    for (auto i = 0; i < filters.size(); i++) {
        std::cout << "(" << filters.at(i)->index << ", ";
        if (instanceof<Filters::AVX::Equal<T>>(filters.at(i))) std::cout << "==";
        else if (instanceof<Filters::AVX::NotEqual<T>>(filters.at(i))) std::cout << "!=";
        else if (instanceof<Filters::AVX::GreaterThan<T>>(filters.at(i))) std::cout << " >";
        else if (instanceof<Filters::AVX::GreaterEqual<T>>(filters.at(i))) std::cout << ">=";
        else if (instanceof<Filters::AVX::LessThan<T>>(filters.at(i))) std::cout << " <";
        else if (instanceof<Filters::AVX::LessEqual<T>>(filters.at(i))) std::cout << "<=";
        std::cout << filters.at(i)->value << ")" << (i == filters.size() - 2 ? " and " : (i < filters.size() - 2 ? ", " : ""));
    }
    std::cout << std::endl;
}

void debugTest() {
    const int** initialData = TableHelper::generateRandomData<int>(COLUMNS, ROWS, 1, 10);
    ColumnStore::Basic::Table<int> testTable(COLUMNS, ROWS, initialData);

    std::cout << "Test table: \n" << std::endl;

    testTable.print();

    auto equalFilter = new Filters::Basic::Equal<int>(0, 4);
    auto greaterFilter = new Filters::Basic::GreaterThan<int>(4, 4);

    std::vector<Filters::Basic::Filter<int>*> filters {equalFilter, greaterFilter};
    std::vector<uint64_t> projection {0, 2, 4};

    auto [queried, rows, columns] = testTable.queryTable(projection, filters);

    std::cout << "Filters (0, =4) and (4, >4) as well as projection on (0, 2, 4): \n" << std::endl;

    TableHelper::printTable(queried, columns, rows);

    std::cout << "Number of rows: " << testTable.queryCount(projection, filters) << std::endl;
}

template <typename T>
void avxTest() {
    const T** initialData = TableHelper::generateRandomData<T>(COLUMNS, ROWS, 1, 5);
    ColumnStore::AVX::Table<T> testTable(COLUMNS, ROWS, initialData);

    // testTable.print();

    auto equalFilter = new Filters::AVX::GreaterThan<T>(0, 2);
    auto equalFilter2 = new Filters::AVX::NotEqual<T>(1, 3);
    auto equalFilter3 = new Filters::AVX::LessEqual<T>(3, 4);

    std::vector<Filters::AVX::Filter<T>*> filters {equalFilter, equalFilter2, equalFilter3};
    std::vector<uint64_t> projection {0, 1, 2, 3, 4};

    auto [queried, rows, columns] = testTable.queryTable(projection, filters);

    printFilterInfo<T>(filters);
    std::cout << std::endl;

    TableHelper::printTable(queried, columns, rows);
}

int main(int argc, char ** argv) {
    avxTest<uint64_t>();
    return 0;
}
