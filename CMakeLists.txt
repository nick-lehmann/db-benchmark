cmake_minimum_required(VERSION 3.10)

# set the project name
project(DB-Benchmark VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CFLAGS -O3 -fno-tree-vectorize)

# pass flags to compiler
add_definitions(${CFLAGS})

# build directory
set(CMAKE_BINARY_DIR "bin")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# include directories
include_directories(src/include)

# specify executable to build   TODO make cariable specifiying common build files
add_executable(row_store src/row_store/main.cpp src/row_store/BaseTable.h src/row_store/IntermediateTable.h)
add_executable(column_store src/column_store/main.cpp)
add_executable(pax_store src/pax_store/main.cpp)
