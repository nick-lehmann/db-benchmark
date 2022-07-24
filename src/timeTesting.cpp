//
// Created by tumpe on 5/24/22.
//


#include <cstdlib>
#include <array>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "Benchmark.h"
#include "Helper.h"
#include "Filters/All.h"
#include "SIMD.h"

template<typename T, SIMD Variant>
void benchmark(std::vector<uint64_t> &projection,
              std::vector<Filters::Filter<T, Variant> *> &filters, const std::string &fileId) {


    std::string fileName="../output_files/PSbenchmark_" + fileId + ".csv";
    Benchmark::benchmarkRows<T,Variant>(2,projection,filters,100,50,false,100,500,0,
                                                   100,42,fileName);
    fileName="../output_files/CSbenchmark_" + fileId + ".csv";
    Benchmark::benchmarkRows<T,Variant>(1,projection,filters,100,50,false,100,500,0,
                                                   100,42,fileName);
    fileName="../output_files/RSbenchmark_" + fileId + ".csv";
    Benchmark::benchmarkRows<T,Variant>(0,projection,filters,100,50,false,100,500,0,
                                                   100,42,fileName);

}


int main(int argc, char** argv) {
    using Type64 = std::uint64_t;
    using Type32 = std::uint32_t;

    // AVX with 64 bit

    auto Filter_AVX64 = new Filters::GreaterThan<Type64, SIMD::AVX512>(0, 72);
    auto Filter2_AVX64 = new Filters::NotEqual<Type64, SIMD::AVX512>(1, 3);
    auto Filter3_AVX64 = new Filters::LessEqual<Type64, SIMD::AVX512>(3, 39);


    std::vector<Filters::Filter<Type64, SIMD::AVX512>*> filters_AVX64{Filter_AVX64, Filter2_AVX64, Filter3_AVX64};
    std::vector<Type64> projection{0, 1, 2};

    benchmark<Type64 ,SIMD::AVX512>(projection,filters_AVX64,"AVX64");


    // AVX with 32 bit

    auto Filter_AVX32 = new Filters::GreaterThan<Type32, SIMD::AVX512>(0, 72);
    auto Filter2_AVX32 = new Filters::NotEqual<Type32, SIMD::AVX512>(1, 3);
    auto Filter3_AVX32 = new Filters::LessEqual<Type32, SIMD::AVX512>(3, 39);


    std::vector<Filters::Filter<Type32, SIMD::AVX512>*> filters_AVX32{Filter_AVX32, Filter2_AVX32, Filter3_AVX32};

    benchmark<Type32 ,SIMD::AVX512>(projection,filters_AVX32,"AVX32");


    // Scalar with 32 bit

    auto Filter32 = new Filters::GreaterThan<Type32, SIMD::None>(0, 72);
    auto Filter2_32 = new Filters::NotEqual<Type32, SIMD::None>(1, 3);
    auto Filter3_32 = new Filters::LessEqual<Type32, SIMD::None>(3, 39);


    std::vector<Filters::Filter<Type32, SIMD::None>*> filters32{Filter32, Filter2_32, Filter3_32};

    benchmark<Type32 ,SIMD::None>(projection,filters32,"Scalar32");


    // Scalar with 64 bit

    auto Filter_S64 = new Filters::GreaterThan<Type64, SIMD::None>(0, 72);
    auto Filter2_S64 = new Filters::NotEqual<Type64, SIMD::None>(1, 3);
    auto Filter3_S64 = new Filters::LessEqual<Type64, SIMD::None>(3, 39);

    std::vector<Filters::Filter<Type64, SIMD::None>*> filters_S64{Filter_S64, Filter2_S64, Filter3_S64};

    benchmark<Type64 ,SIMD::None>(projection,filters_S64,"Scalar64");



    auto Filter_S = new Filters::GreaterThan<Type64, SIMD::AVX512_Strided>(0, 72);
    auto Filter2_S = new Filters::NotEqual<Type64, SIMD::AVX512_Strided>(1, 3);
    auto Filter3_S = new Filters::LessEqual<Type64, SIMD::AVX512_Strided>(3, 39);

    std::vector<Filters::Filter<Type64, SIMD::AVX512_Strided>*> filters_strided{Filter_S, Filter2_S, Filter3_S};

    Benchmark::benchmarkRows<Type64,SIMD::AVX512_Strided>(0,projection,filters_strided,100,50,false,100,500,0,
                                               100,42,"../output_files/RSbenchmark_Strided64.csv");

    auto Filter_S32 = new Filters::GreaterThan<Type32, SIMD::AVX512_Strided>(0, 72);
    auto Filter2_S32 = new Filters::NotEqual<Type32, SIMD::AVX512_Strided>(1, 3);
    auto Filter3_S32 = new Filters::LessEqual<Type32, SIMD::AVX512_Strided>(3, 39);

    std::vector<Filters::Filter<Type32, SIMD::AVX512_Strided>*> filters_strided32{Filter_S32, Filter2_S32, Filter3_S32};

    Benchmark::benchmarkRows<Type32,SIMD::AVX512_Strided>(0,projection,filters_strided32,100,50,false,100,500,0,
                                               100,42,"../output_files/RSbenchmark_Strided32.csv");


    return 0;
}
