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
#include "Filters/Base.h"
#include "Filters/Equal.h"
#include "Filters/LessThan.h"
#include "Filters/LessEqual.h"
#include "Filters/GreaterThan.h"



int main(int argc, char ** argv) {

    using Type = uint64_t;

    std::vector<Filters::Filter<Type, SIMD::AVX512>*> filters  {new Filters::Equal<Type, SIMD::AVX512> (1, 20),new Filters::GreaterThan<Type, SIMD::AVX512> (5,31),new Filters::LessThan<Type, SIMD::AVX512> (3,76),
                                                                 new Filters::Equal<Type, SIMD::AVX512> (4,42),new Filters::GreaterThan<Type, SIMD::AVX512> (8,42),new Filters::LessEqual<Type, SIMD::AVX512> (6,56)};
    std::vector<Type> projection {1,2,3,4,5,6,8};

    //std::vector<Filter::Filter<Type, SIMD::AVX512> *> filters{gtAVX,eqAVX,ltAVX,
    //                                                          eqAVX2,gtAVX2,ltAVX2};

    //array<const std::string,3> files={"RSbenchmark.csv","CSbenchmark.csv","PSbenchmark.csv"};


    /*
    Benchmark::runBenchmark<uint64_t>(0,projection,filters,10,10,false,50,300,1,
                            1000,0,"RSbenchmark.csv");

    Benchmark::runBenchmark<uint64_t>(1,projection,filters,10,10,false,50,300,1,
                            1000,0,"CSbenchmark.csv");

    Benchmark::runBenchmark<uint64_t>(2,projection,filters,10,10,false,50,300,1,
                            1000,0,"PSbenchmark.csv");*/

    //Benchmark::benchmarkRows<Type>(0,projection,filters,100,50,false,100,500,0,
    //                                100,42,"../output_files/RSbenchmark_AVX.csv");
    Benchmark::benchmarkRows<Type>(1,projection,filters,100,50,false,100,500,0,
                                      100,42,"../output_files/CSbenchmark_AVX.csv");
    Benchmark::benchmarkRows<Type>(2,projection,filters,100,50,false,100,500,0,
                                   100,42,"../output_files/PSbenchmark_AVX.csv");




    return 0;
}
