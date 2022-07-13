#include <iostream>

#include "Benchmark.h"
#include "ColumnStoreTable.h"
#include "Filters/All.h"
#include "Helper.h"

// settings for the table
#define COLUMNS 5
#define ROWS 512
#define TEST_DATA_TYPE uint64_t

// taken from https://www.tutorialspoint.com/cplusplus-equivalent-of-instanceof
// template<typename Base, typename T>
// inline bool instanceof(const T *ptr) {
//     return dynamic_cast<const Base*>(ptr) != nullptr;
// }

/// Small helper function for debugging output. It is not very pretty, it will be removed sooner or later.
// template <typename T>
// void printFilterInfo(std::vector<Filters::AVX::Filter<T>*> filters) {
//     std::cout << "Filters ";
//     for (auto i = 0; i < filters.size(); i++) {
//         std::cout << "(" << filters.at(i)->index << ", ";
//         if (instanceof<Filters::AVX::Equal<T>>(filters.at(i))) std::cout << "==";
//         else if (instanceof<Filters::AVX::NotEqual<T>>(filters.at(i))) std::cout << "!=";
//         else if (instanceof<Filters::AVX::GreaterThan<T>>(filters.at(i))) std::cout << " >";
//         else if (instanceof<Filters::AVX::GreaterEqual<T>>(filters.at(i))) std::cout << ">=";
//         else if (instanceof<Filters::AVX::LessThan<T>>(filters.at(i))) std::cout << " <";
//         else if (instanceof<Filters::AVX::LessEqual<T>>(filters.at(i))) std::cout << "<=";
//         std::cout << filters.at(i)->value << ")" << (i == filters.size() - 2 ? " and " : (i < filters.size() - 2 ? ", " : ""));
//     }
//     std::cout << std::endl;
// }

template <typename T>
void avxTest(const T** initialData) {
    ColumnStore::Table<T> testTable(COLUMNS, ROWS, initialData);

    auto equalFilter = new Filters::GreaterThan<T, SIMD::AVX512>(0, 2);
    auto equalFilter2 = new Filters::NotEqual<T, SIMD::AVX512>(1, 3);
    auto equalFilter3 = new Filters::LessEqual<T, SIMD::AVX512>(3, 4);

    std::vector<Filters::Filter<T, SIMD::AVX512>*> filters{equalFilter, equalFilter2, equalFilter3};
    std::vector<uint64_t> projection{0, 1, 2, 3, 4};

    auto [queried, rows, columns] = testTable.queryTable(projection, filters);

    std::cout << "AVX: " << rows << " rows,\nTable: " << std::endl;

    TableHelper::printTable(queried, columns, rows);
    
}

template <typename T>
void scalarTest(const T** initialData) {
    ColumnStore::Table<T> testTable(COLUMNS, ROWS, initialData);

    auto equalFilter = new Filters::GreaterThan<T, SIMD::None>(0, 2);
    auto equalFilter2 = new Filters::NotEqual<T, SIMD::None>(1, 3);
    auto equalFilter3 = new Filters::LessEqual<T, SIMD::None>(3, 4);

    std::vector<Filters::Filter<T, SIMD::None>*> filters{equalFilter, equalFilter2, equalFilter3};
    std::vector<uint64_t> projection{0, 1, 2, 3, 4};

    auto [queried, rows, columns] = testTable.queryTable(projection, filters);

    std::cout << "SCALAR: " << rows << " rows,\nTable: " << std::endl;

    TableHelper::printTable(queried, columns, rows);
    
}

int main(int argc, char** argv) {
    auto** initialData = const_cast<TEST_DATA_TYPE**>(TableHelper::generateRandomData<TEST_DATA_TYPE>(COLUMNS, ROWS, 1, 5));
    for (int i = 0; i < ROWS; i++) {
        initialData[i][4] = i;
    }

    avxTest<TEST_DATA_TYPE>(const_cast<const TEST_DATA_TYPE**>(initialData));
    scalarTest<TEST_DATA_TYPE>(const_cast<const TEST_DATA_TYPE**>(initialData));

    return 0;
}
