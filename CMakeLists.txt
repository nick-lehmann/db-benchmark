cmake_minimum_required(VERSION 3.10)

# set the project name
project(DB-Benchmark VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CFLAGS -fno-tree-vectorize)

# available avx flags for knights landing (KNL) architecture
set(PHI_FLAGS -mavx512f -mavx512cd -mavx512er -mavx512pf)

# pass flags to compiler
add_compile_options("$<$<CONFIG:RELEASE>:-O3;${CFLAGS};${PHI_FLAGS}>")
add_compile_options("$<$<CONFIG:DEBUG>:${CFLAGS};${PHI_FLAGS}>")

# build directory
set(CMAKE_BINARY_DIR "bin")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# include directories
include_directories(src/include)

# specify executable to build
add_executable(row_store src/row_store/main.cpp
  src/row_store/BaseTable.h
  src/row_store/Filter.h
  src/row_store/Projection.h
)
add_executable(column_store src/column_store/main.cpp)
add_executable(pax_store src/pax_store/main.cpp)
add_executable(timeTesting src/timeTesting.cpp)
add_executable(test src/test/main.cpp)
