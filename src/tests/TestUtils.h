#include <cstdlib>
#include <iostream>

template <typename T>
void printTable(T data, uint64_t rows, uint64_t columns) {
    for (unsigned row = 0; row < rows; row++) {
        for (unsigned column = 0; column < columns; column++) {
            std::cout << data[row][column] << " ";
        }
        std::cout << std::endl;
    }
}