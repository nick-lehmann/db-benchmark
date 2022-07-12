#include <iostream>

#include "Benchmark.h"
#include "ColumnStoreTable.h"
#include "Filters/All.h"
#include "Helper.h"

#define COLUMNS 5
#define ROWS 512

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

// void debugTest() {
//     const int** initialData = TableHelper::generateRandomData<int>(COLUMNS, ROWS, 1, 10);
//     ColumnStore::Scalar::Table<int> testTable(COLUMNS, ROWS, initialData);

//     std::cout << "Test table: \n" << std::endl;

//     testTable.print();

//     auto equalFilter = new Filters::Scalar::Equal<int>(0, 4);
//     auto greaterFilter = new Filters::Scalar::GreaterThan<int>(4, 4);

//     std::vector<Filters::Scalar::Filter<int>*> filters{equalFilter, greaterFilter};
//     std::vector<uint64_t> projection{0, 2, 4};

//     auto [queried, rows, columns] = testTable.queryTable(projection, filters);

//     std::cout << "Filters (0, =4) and (4, >4) as well as projection on (0, 2, 4): \n" << std::endl;

//     TableHelper::printTable(queried, columns, rows);

//     std::cout << "Number of rows: " << testTable.queryCount(projection, filters) << std::endl;
// }

template <typename T>
void avxTest(const T** initialData) {
    ColumnStore::Table<T> testTable(COLUMNS, ROWS, initialData);

    // testTable.print();

    auto equalFilter = new Filters::GreaterThan<T, SIMD::AVX512>(0, 2);
    auto equalFilter2 = new Filters::NotEqual<T, SIMD::AVX512>(1, 3);
    auto equalFilter3 = new Filters::LessEqual<T, SIMD::AVX512>(3, 4);

    std::vector<Filters::Filter<T, SIMD::AVX512>*> filters{/*equalFilter, equalFilter2,*/ equalFilter3};
    std::vector<uint64_t> projection{0, 1, 2, 3, 4};

    auto [queried, rows, columns] = testTable.queryTable(projection, filters);

    // printFilterInfo<T>(filters);
    std::cout << "AVX: " << std::endl;

    TableHelper::printTable(queried, columns, rows);
    
}

template <typename T>
void scalarTest(const T** initialData) {
    ColumnStore::Table<T> testTable(COLUMNS, ROWS, initialData);

    // testTable.print();

    auto equalFilter = new Filters::GreaterThan<T, SIMD::None>(0, 2);
    auto equalFilter2 = new Filters::NotEqual<T, SIMD::None>(1, 3);
    auto equalFilter3 = new Filters::LessEqual<T, SIMD::None>(3, 4);

    std::vector<Filters::Filter<T, SIMD::None>*> filters{/*equalFilter, equalFilter2,*/ equalFilter3};
    std::vector<uint64_t> projection{0, 1, 2, 3, 4};

    auto [queried, rows, columns] = testTable.queryTable(projection, filters);

    // printFilterInfo<T>(filters);
    std::cout << "SCALAR: " << std::endl;

    TableHelper::printTable(queried, columns, rows);
    
}

// void benchmark() {
//     const uint32_t** initialData = TableHelper::generateRandomData<uint32_t>(COLUMNS, ROWS, 1, 10);
//     ColumnStore::Table<uint32_t> testTable(COLUMNS, ROWS, initialData);

//     auto equalFilter = new Filters::Equal<uint32_t, SIMD::None>(0, 4);
//     auto greaterFilter = new Filters::GreaterThan<uint32_t, SIMD::None>(4, 4);

//     std::vector<Filters::Filter<uint32_t, SIMD::None>*> filters{equalFilter, greaterFilter};
//     std::vector<uint64_t> projection{0, 2, 4};

//     Benchmark::measureTime(testTable, projection, filters);
// }

int main(int argc, char** argv) {
    // uint32_t** initialData = const_cast<uint32_t**>(TableHelper::generateRandomData<uint32_t>(COLUMNS, ROWS, 1, 5));
    // for(int i = 0; i < ROWS; i++) {
    //     initialData[i][4] = i;
    // }

    // avxTest<uint32_t>(const_cast<const uint32_t**>(initialData));
    // scalarTest<uint32_t>(const_cast<const uint32_t**>(initialData));
    
    uint64_t** initialData = const_cast<uint64_t**>(TableHelper::generateRandomData<uint64_t>(COLUMNS, ROWS, 1, 5));
    for(int i = 0; i < ROWS; i++) {
        initialData[i][4] = i;
    }

    avxTest<uint64_t>(const_cast<const uint64_t**>(initialData));
    scalarTest<uint64_t>(const_cast<const uint64_t**>(initialData));

    return 0;
}
