#include <errno.h>
#include <immintrin.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <iterator>
#include <vector>

#include "Constants.h"
#include "Filters.h"
#include "Filters2.h"
#include "Helper.h"
#include "Memory.h"
#include "Page.h"
#include "PaxTable.h"
#include "PaxTable_AVX.h"
#include "Types.h"

using namespace std;

template <typename T>
const T** getData(const int numberOfRows) {
    T** data = (T**)malloc(numberOfRows * sizeof(T*));

    for (unsigned row = 0; row < numberOfRows; row++) {
        T* rowData = (T*)malloc(3 * sizeof(T));
        data[row] = rowData;
        data[row][0] = row;
        data[row][1] = 42 + row;
        data[row][2] = 200 + row;
    }

    return const_cast<const T**>(data);
}

template <typename T>
void testGetData() {
    const int numberOfRows = 10;
    const T** data = getData<T>(numberOfRows);
    cout << data << endl;
}

/**
 * Inspect the memory layout of a single PAX page.
 */
template <typename T>
void testPagePageManual() {
    unsigned numberOfRows = 300;
    unsigned numberOfAttributes = 3;

    const T** data = getData<T>(numberOfRows);

    auto pagesize = getPagesize();
    Buffer buffer = allocateAlignedBuffer(pagesize);

    PaxPage<T> page = PaxPage<T>((Header*)buffer, pagesize, numberOfAttributes);

    for (unsigned row = 0; row < numberOfRows; row++) page.writeRecord(data[row]);

    page.print();
}

/**
 * Inspect the memory layout of a table with a
 * single page.
 */
template <typename T>
void testPaxTableSinglePageMemory() {
    unsigned numberOfRows = 10;
    unsigned numberOfAttributes = 3;

    const T** data = getData<T>(numberOfRows);

    PaxTable<T> table(numberOfAttributes, numberOfRows, data);

    table.pages[0].print();
}

/**
 * Inspect the memory layout of a table with a two
 * pages.
 */
template <typename T>
void testPaxTableTwoPagesMemory() {
    unsigned numberOfRows = 1000;
    unsigned numberOfAttributes = 3;

    const T** data = getData<T>(numberOfRows);

    PaxTable<T> table(numberOfAttributes, numberOfRows, data);

    table.pages[0].print();
    table.pages[1].print();
}

/**
 * Print a sample table to console.
 */
template <typename T>
void testPaxTablePrint() {
    unsigned numberOfRows = 1000;
    unsigned numberOfAttributes = 3;

    const T** data = getData<T>(numberOfRows);

    PaxTable<T> table(numberOfAttributes, numberOfRows, data);

    table.pages[0].print();
    table.print();
}

/**
 * Test a single filter on a table with a single
 * page.
 */
template <typename T>
void testBasicScalarFilters() {
    unsigned numberOfRows = 10;
    unsigned numberOfAttributes = 3;

    const T** data = getData<T>(numberOfRows);

    PaxTable<T> table(numberOfAttributes, numberOfRows, data);

    std::vector<uint64_t> projection = {0, 1, 2};
    std::vector<Filters::Scalar::Filter<T>*> filters = {new Filters::Scalar::Equal<T>(0, (T)2)};

    auto [result, rows, columns] = table.queryTable(projection, filters);

    for (unsigned row = 0; row < rows; row++) {
        for (unsigned column = 0; column < columns; column++) {
            cout << result[row][column] << " ";
        }
        cout << endl;
    }

    cout << "Count: " << rows << endl;
}

/**
 * Test a single filter on a table with a single
 * page.
 */
template <typename T>
void testBasicAVXFilters() {
    unsigned numberOfRows = 10;
    unsigned numberOfAttributes = 3;

    const T** data = getData<T>(numberOfRows);

    PaxTableAVX<T> table(numberOfAttributes, numberOfRows, data);

    std::vector<uint64_t> projection = {0, 1, 2};
    std::vector<Filters::AVX::Filter<T>*> filters = {new Filters::AVX::LessEqual<T>(0, 3), new Filters::AVX::GreaterEqual<T>(0, 1),
                                                     new Filters::AVX::Equal<T>(0, 2)};
    // std::vector<Filters::AVX::Filter<T>*> filters = {new Filters::AVX::LessEqual<T>(0, 3)};

    auto [result, rows, columns] = table.queryTable(projection, filters);

    for (unsigned row = 0; row < rows; row++) {
        for (unsigned column = 0; column < columns; column++) {
            cout << result[row][column] << " ";
        }
        cout << endl;
    }

    cout << "Count: " << rows << endl;
}

// void testUnifiedFilters() {
//   std::vector < Filters2::
// }

// void testQuery() {}

// template <typename T>
// class Filter {
//    public:
//     T value;
//     const __m512i value_register;

//     bool match(value T) { return true; }
//     __mmask16 match(__m512i reg, __mmask16 mask = ONE_MASK) { return _mm512_int2mask(INT32_MAX); };
//     __mmask8 match(__m512i reg, __mmask8 mask = ONE_MASK) { return _mm512_int2mask(INT32_MAX); };
// };

// template <typename T>
// class Equal : public Filter<T> {
//    public:
//     bool match(value T) override { return self->value == value; }
//     __mmask16 match(__m512i reg, __mmask16 mask = ONE_MASK) override { return _mm512_mask_cmpeq_epi32_mask(mask, reg, value_register); };
//     __mmask8 match(__m512i reg, __mmask8 mask = ONE_MASK) override { return _mm512_mask_cmpeq_epi64_mask(mask, reg, value_register); };
// };

template <typename T, bool AVX>
class Filter {
   public:
    virtual bool match() = 0;
};

template <>
class Filter<uint32_t, true> {
   public:
    const uint32_t value;
    __m512i value_register;

    Filter(uint32_t value) : value(value), value_register(_mm512_set1_epi32(value)) {}
    virtual __mmask16 match(__m512i reg, __mmask16 mask = ONE_MASK) = 0;
};

template <>
class Filter<uint64_t, true> {
   public:
    const uint64_t value;
    __m512i value_register;

    Filter(uint64_t value) : value(value), value_register(_mm512_set1_epi64(value)) {}
    virtual __mmask8 match(__m512i reg, __mmask8 mask = ONE_MASK) const = 0;
};

template <typename T>
class Filter<T, false> {
   public:
    const T value;

    Filter(T value) : value(value) {}
    virtual bool match(T value) = 0;
};

template <typename T, bool AVX>
class Equal : public Filter<T, AVX> {};

template <>
class Equal<uint32_t, true> : public Filter<uint32_t, true> {
   public:
    using Filter<uint32_t, true>::Filter;

    __mmask16 match(__m512i reg, __mmask16 mask = ONE_MASK) {
        cout << "Equal AVX with 32bit" << endl;
        return _mm512_mask_cmpeq_epi32_mask(mask, reg, value_register);
    }
};

template <typename T>
class Equal<T, false> : public Filter<T, false> {
   public:
    using Filter<T, false>::Filter;

    bool match(T value) override {
        cout << "Equal Scalar with T" << endl;
        return this->value == value;
    }
};

template <typename T, bool AVX>
class LessThan : public Filter<T, AVX> {};

template <>
class LessThan<uint32_t, true> : public Filter<uint32_t, true> {
   public:
    using Filter<uint32_t, true>::Filter;

    __mmask16 match(__m512i reg, __mmask16 mask = ONE_MASK) {
        cout << "LessThan AVX with 32bit" << endl;
        return _mm512_mask_cmpeq_epi32_mask(mask, reg, value_register);
    }
};

template <typename T>
void queryAVX(Filter<T, true>& filter) {
    __m512i reg32bit = _mm512_set1_epi32(1);
    auto mask = filter.match(reg32bit);
}

template <typename T>
void queryNormal(Filter<T, false>& filter) {
    T value = 1;
    auto result = filter.match(value);
    cout << "Scalar operations says: " << result << endl;
}

int main() {
    Equal<uint32_t, true> equal32bitAVX(1);
    LessThan<uint32_t, true> lessThan32bitAVX(1);
    Equal<uint32_t, false> equal32bitScalar(1);

    queryAVX(equal32bitAVX);
    queryAVX(lessThan32bitAVX);

    queryNormal(equal32bitScalar);

    return 0;
}
