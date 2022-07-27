#pragma once

#include <numeric>
#include <vector>

vector<uint64_t> createProjection(uint64_t lower, uint64_t upper, uint64_t step) {
    std::vector<uint64_t> projection;
    for (uint64_t i = lower; i < upper; i += step) {
        projection.push_back(i);
    }
    return projection;
}