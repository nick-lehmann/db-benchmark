//
// Created by tumpe on 5/24/22.
//

#include <array>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include "../row_store/BaseTable.h"
#include "Benchmark.h"
#include "Filters/All.h"
#include "Helper.h"
#include "SIMD.h"

template <typename T, SIMD Variant>
void benchmark(std::vector<uint64_t> &projection, std::vector<Filters::Filter<T, Variant> *> &filters,
               const std::string &fileId, unsigned rows=200000, unsigned columns=50,
               unsigned increase=200000,unsigned steps=50) {
    std::string fileName = "../output_files/PSbenchmark_" + fileId + ".csv";
    Benchmark::benchmarkRows<T, Variant>(2, projection, filters, rows, columns, false, increase, steps, 0, 100, 42, fileName);
    fileName = "../output_files/CSbenchmark_" + fileId + ".csv";
    Benchmark::benchmarkRows<T, Variant>(1, projection, filters, rows, columns, false, increase, steps, 0, 100, 42, fileName);
    fileName = "../output_files/RSbenchmark_" + fileId + ".csv";
    Benchmark::benchmarkRows<T, Variant>(0, projection, filters, rows, columns, false, increase, steps, 0, 100, 42, fileName);
}

template<typename T, SIMD Variant>
std::vector<Filters::Filter<T, Variant> *> genFilters(unsigned int filterNr) {
    std::srand(42);
    std::vector<Filters::Filter<T, Variant> *> filters;
    unsigned int rFilter;
    for (int i = 0; i < filterNr; i++) {
        rFilter = i % 5;
        switch (rFilter) {
            case 0:
                filters.push_back(
                        new Filters::NotEqual<T, Variant>(i, std::rand() % 21 + 40));
                break;
                /* case 1:
                    filters.push_back(
                        new Filters::Equal<T, Variant>(i, std::rand() % 21 +40));
                    break; */
            case 1:
                filters.push_back(
                        new Filters::GreaterEqual<T, Variant>(i, std::rand() % 21 + 40));
                break;
            case 2:
                filters.push_back(
                        new Filters::GreaterThan<T, Variant>(i, std::rand() % 21 + 40));
                break;
            case 3:
                filters.push_back(
                        new Filters::LessEqual<T, Variant>(i, std::rand() % 21 + 40));
                break;
            case 4:
                filters.push_back(
                        new Filters::LessThan<T, Variant>(i, std::rand() % 21 + 40));
                break;
        }
    }
    return filters;
}

void increasingRows(std::vector<uint64_t> &projection) {
    using Type64 = std::uint64_t;
    using Type32 = std::uint32_t;

    unsigned int filterNr=5;

    // AVX with 64 bit
    std::vector<Filters::Filter<Type64, SIMD::AVX512> *> filters_AVX64 = genFilters<Type64,SIMD::AVX512>(filterNr);
    benchmark<Type64, SIMD::AVX512>(projection, filters_AVX64, "AVX64");
    std::vector<Filters::Filter<Type64, SIMD::AVX512> *>().swap(filters_AVX64);

    // AVX with 32 bit
    std::vector<Filters::Filter<Type32, SIMD::AVX512> *> filters_AVX32 = genFilters<Type32,SIMD::AVX512>(filterNr);
    benchmark<Type32, SIMD::AVX512>(projection, filters_AVX32, "AVX32");
    std::vector<Filters::Filter<Type32, SIMD::AVX512> *>().swap(filters_AVX32);

    // Scalar with 32 bit
    std::vector<Filters::Filter<Type32, SIMD::None> *> filters32 = genFilters<Type32,SIMD::None>(filterNr);
    benchmark<Type32, SIMD::None>(projection, filters32, "Scalar32");
    std::vector<Filters::Filter<Type32, SIMD::None> *>().swap(filters32);

    // Scalar with 64 bit
    std::vector<Filters::Filter<Type64, SIMD::None> *> filters64 = genFilters<Type64,SIMD::None>(filterNr);
    benchmark<Type64, SIMD::None>(projection, filters64, "Scalar64");
    std::vector<Filters::Filter<Type64, SIMD::None> *>().swap(filters64);

    // AVX strided with 64 bit
    std::vector<Filters::Filter<Type64, SIMD::AVX512_Strided> *> filters64_S = genFilters<Type64,SIMD::AVX512_Strided>(filterNr);
    Benchmark::benchmarkRows<Type64, SIMD::AVX512_Strided>(0, projection, filters64_S, 200000, 50, false, 200000, 50, 0, 100, 42,
                                                           "../output_files/RSbenchmark_Strided64.csv");
    std::vector<Filters::Filter<Type64, SIMD::AVX512_Strided> *>().swap(filters64_S);

    // AVX strided with 32 bit
    std::vector<Filters::Filter<Type32, SIMD::AVX512_Strided> *> filters32_S = genFilters<Type32,SIMD::AVX512_Strided>(filterNr);
    Benchmark::benchmarkRows<Type32, SIMD::AVX512_Strided>(0, projection, filters32_S, 200000, 50, false, 200000, 50, 0, 100, 42,
                                                           "../output_files/RSbenchmark_Strided32.csv");
    std::vector<Filters::Filter<Type32, SIMD::AVX512_Strided> *>().swap(filters32_S);
}

template <typename T, SIMD Variant>
void benchmarkFilter(std::vector<uint64_t> &projection, std::vector<Filters::Filter<T, Variant> *> &filters,
                     const std::string &fileId, unsigned rows=5000000, unsigned columns=50,
                     unsigned increase=100,unsigned steps=1) {
    std::string fileName = "../output_files/PSbenchmark_" + fileId +"_fNR_"+std::to_string(filters.size())+".csv";
    Benchmark::benchmarkRows<T, Variant>(2, projection, filters, rows, columns, false, increase, steps, 0, 100, 42, fileName);
    fileName = "../output_files/CSbenchmark_" + fileId +"_fNR_"+std::to_string(filters.size())+ ".csv";
    Benchmark::benchmarkRows<T, Variant>(1, projection, filters, rows, columns, false, increase, steps, 0, 100, 42, fileName);
    fileName = "../output_files/RSbenchmark_" + fileId +"_fNR_"+std::to_string(filters.size())+ ".csv";
    Benchmark::benchmarkRows<T, Variant>(0, projection, filters, rows, columns, false, increase, steps, 0, 100, 42, fileName);
}

template <typename T, SIMD Variant>
void increaseFilterNumber(unsigned int filterNr, std::vector<uint64_t> &projection, const std::string &type) {
    std::srand(42);
    std::vector<Filters::Filter<T, Variant> *> filters;
    unsigned int rFilter;
    for (int i = 0; i < filterNr; i++) {
        rFilter= i % 5;
        switch (rFilter) {
            case 0:
                filters.push_back(
                    new Filters::NotEqual<T, Variant>(i, std::rand() % 21 +40));
                break;
            /*case 1:
                filters.push_back(
                    new Filters::Equal<T, Variant>(i, std::rand() % 21 +40));
                break;*/
            case 1:
                filters.push_back(
                    new Filters::GreaterEqual<T, Variant>(i, std::rand() % 21 +40));
                break;
            case 2:
                filters.push_back(
                    new Filters::GreaterThan<T, Variant>(i, std::rand() % 21 +40));
                break;
            case 3:
                filters.push_back(
                    new Filters::LessEqual<T, Variant>(i, std::rand() % 21 +40));
                break;
            case 4:
                filters.push_back(
                    new Filters::LessThan<T, Variant>(i, std::rand() % 21 +40));
                break;
        }
        benchmarkFilter<T, Variant>(projection, filters, type);
    }
}

int main(int argc, char **argv) {
    std::vector<uint64_t> projection{0, 1, 2, 3, 12, 28};

    //increasingRows(projection);
    increaseFilterNumber<uint64_t,SIMD::AVX512>(30,projection,"AVX64");
    increaseFilterNumber<uint64_t,SIMD::None>(30,projection,"Scalar64");

    increaseFilterNumber<uint32_t,SIMD::AVX512>(30,projection,"AVX32");
    increaseFilterNumber<uint32_t,SIMD::None>(30,projection,"Scalar32");

    return 0;
}
