//
// Created by tumpe on 5/24/22.
//

#include <iostream>
#include <vector>
#include <array>
#include <cstdlib>
#include <string>

#include "Benchmark.h"

//#include "Table.h"
#include "Helper.h"
//#include "Filters.h"
#include "../row_store/BaseTable.h"
#include "Filters/All.h"

int main(int argc, char ** argv) {

    using Type = uint64_t;

    //std::vector<Filters::Filter<Type, SIMD::AVX512>*> filters  {new Filters::Equal<Type, SIMD::AVX512> (1, 2), new Filters::LessThan<Type, SIMD::AVX512> (3,7)};
    //std::vector<Type> projection {0,1,2,3,4};

    //std::vector<uint64_t> projection = {0, 2, 3};
    //std::vector<Filters::Filter<Type, SIMD::AVX512> *> filters = {new Filters::GreaterThan<Type, SIMD::AVX512>(1, 6),
    //                                                      new Filters::LessThan<Type, SIMD::AVX512>(2, 9)};

    //std::vector<Filter::Filter<Type, SIMD::AVX512> *> filters{gtAVX,eqAVX,ltAVX,
    //                                                          eqAVX2,gtAVX2,ltAVX2};

//    std::vector<uint64_t> projection = {0, 1, 2};
//    std::vector<Filters::Filter<Type, SIMD::AVX512>*> filters = {new Filters::LessEqual<Type, SIMD::AVX512>(0, 3),
//                                                              new Filters::GreaterEqual<Type, SIMD::AVX512>(0, 1),
//                                                              new Filters::Equal<Type, SIMD::AVX512>(0, 2)};


    //array<const std::string,3> files={"RSbenchmark.csv","CSbenchmark.csv","PSbenchmark.csv"};

    auto equalFilter_AVX = new Filters::GreaterThan<Type, SIMD::AVX512>(0, 2);
    auto equalFilter2_AVX = new Filters::NotEqual<Type, SIMD::AVX512>(1, 3);
    auto equalFilter3_AVX = new Filters::LessEqual<Type, SIMD::AVX512>(3, 40);
//
    std::vector<Filters::Filter<Type, SIMD::AVX512>*> filters_AVX{equalFilter_AVX, equalFilter2_AVX, equalFilter3_AVX};
    std::vector<Type> projection{0, 1, 2, 3, 4};


//    std::vector<uint64_t> projection = {0, 1, 2};
//    std::vector<Filters::Filter<Type, SIMD::AVX512>*> filters = {new Filters::LessEqual<Type, SIMD::AVX512>(0, 18),
//                                                              new Filters::GreaterEqual<Type, SIMD::AVX512>(0, 4),
//                                                              new Filters::Equal<Type, SIMD::AVX512>(0, 7)};

//    std::vector<uint64_t> projection = {0, 2, 3};
//    std::vector<Filters::Filter<Type, SIMD::AVX512> *> filters = {new Filters::GreaterThan<T, Variant>(1, 6),
//                                                          new Filters::LessThan<T, Variant>(2, 29)};

    /*
    Benchmark::runBenchmark<uint64_t>(0,projection,filters,10,10,false,50,300,1,
                            1000,0,"RSbenchmark.csv");

    Benchmark::runBenchmark<uint64_t>(1,projection,filters,10,10,false,50,300,1,
                            1000,0,"CSbenchmark.csv");

    Benchmark::runBenchmark<uint64_t>(2,projection,filters,10,10,false,50,300,1,
                            1000,0,"PSbenchmark.csv");*/




    //Benchmark::benchmarkRows<Type,SIMD::AVX512>(2,projection,filters_AVX,100,50,false,100,500,0,
    //                               100,0,"../output_files/PSbenchmark_AVX.csv");
    Benchmark::benchmarkRows<Type,SIMD::AVX512>(1,projection,filters_AVX,100,50,false,100,500,0,
                                      100,0,"../output_files/CSbenchmark_AVX.csv");
    Benchmark::benchmarkRows<Type,SIMD::AVX512>(0,projection,filters_AVX,100,50,false,100,500,0,
                                    100,0,"../output_files/RSbenchmark_AVX.csv");

    auto equalFilter = new Filters::GreaterThan<Type, SIMD::None>(0, 2);
    auto equalFilter2 = new Filters::NotEqual<Type, SIMD::None>(1, 3);
    auto equalFilter3 = new Filters::LessEqual<Type, SIMD::None>(3, 40);
    //
    std::vector<Filters::Filter<Type, SIMD::None>*> filters{equalFilter, equalFilter2, equalFilter3};

    //Benchmark::benchmarkRows<Type,SIMD::AVX512>(2,projection,filters,100,50,false,100,500,0,
    //                                             100,0,"../output_files/PSbenchmark_Scalar.csv");
    Benchmark::benchmarkRows<Type,SIMD::AVX512>(1,projection,filters,100,50,false,100,500,0,
                                                 100,0,"../output_files/CSbenchmark_Scalar.csv");
    Benchmark::benchmarkRows<Type,SIMD::AVX512>(0,projection,filters,100,50,false,100,500,0,
                                                 100,0,"../output_files/RSbenchmark_Scalar.csv");


    return 0;
}
