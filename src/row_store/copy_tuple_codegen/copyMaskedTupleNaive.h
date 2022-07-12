#pragma once
#include <cstdint>
#include <cstring>

namespace RowStore {

// TODO code is wrong -> bit positions need to be reversed

uint8_t copyMaskedTupleN(uint64_t* destAddr, uint64_t* sourceAddr, size_t tupleSize, uint8_t mask) {
    if (!mask) return 0;

    uint8_t popCount = 0;
    if (mask & 0b00000001) {
        std::memcpy(destAddr, sourceAddr, tupleSize * sizeof(uint64_t));
        popCount++;
    }
    if (mask & 0b00000010) {
        std::memcpy(destAddr + (tupleSize * popCount), sourceAddr + (tupleSize * 1), tupleSize * sizeof(uint64_t));
        popCount++;
    }
    if (mask & 0b00000100) {
        std::memcpy(destAddr + (tupleSize * popCount), sourceAddr + (tupleSize * 2), tupleSize * sizeof(uint64_t));
        popCount++;
    }
    if (mask & 0b00001000) {
        std::memcpy(destAddr + (tupleSize * popCount), sourceAddr + (tupleSize * 3), tupleSize * sizeof(uint64_t));
        popCount++;
    }
    if (mask & 0b00010000) {
        std::memcpy(destAddr + (tupleSize * popCount), sourceAddr + (tupleSize * 4), tupleSize * sizeof(uint64_t));
        popCount++;
    }
    if (mask & 0b00100000) {
        std::memcpy(destAddr + (tupleSize * popCount), sourceAddr + (tupleSize * 5), tupleSize * sizeof(uint64_t));
        popCount++;
    }
    if (mask & 0b01000000) {
        std::memcpy(destAddr + (tupleSize * popCount), sourceAddr + (tupleSize * 6), tupleSize * sizeof(uint64_t));
        popCount++;
    }
    if (mask & 0b10000000) {
        std::memcpy(destAddr + (tupleSize * popCount), sourceAddr + (tupleSize * 7), tupleSize * sizeof(uint64_t));
        popCount++;
    }

    return popCount;
}

uint8_t copyMaskedTupleN(uint32_t* destAddr, uint32_t* sourceAddr, size_t tupleSize, uint16_t mask) {
    if (!mask) return 0;

    uint8_t popCount = 0;
    if (mask & 0b0000000000000001) {
        std::memcpy(destAddr, sourceAddr, tupleSize * sizeof(uint32_t));
        popCount++;
    }
    if (mask & 0b0000000000000010) {
        std::memcpy(destAddr + (tupleSize * popCount), sourceAddr + (tupleSize * 1), tupleSize * sizeof(uint32_t));
        popCount++;
    }
    if (mask & 0b0000000000000100) {
        std::memcpy(destAddr + (tupleSize * popCount), sourceAddr + (tupleSize * 2), tupleSize * sizeof(uint32_t));
        popCount++;
    }
    if (mask & 0b0000000000001000) {
        std::memcpy(destAddr + (tupleSize * popCount), sourceAddr + (tupleSize * 3), tupleSize * sizeof(uint32_t));
        popCount++;
    }
    if (mask & 0b0000000000010000) {
        std::memcpy(destAddr + (tupleSize * popCount), sourceAddr + (tupleSize * 4), tupleSize * sizeof(uint32_t));
        popCount++;
    }
    if (mask & 0b0000000000100000) {
        std::memcpy(destAddr + (tupleSize * popCount), sourceAddr + (tupleSize * 5), tupleSize * sizeof(uint32_t));
        popCount++;
    }
    if (mask & 0b0000000001000000) {
        std::memcpy(destAddr + (tupleSize * popCount), sourceAddr + (tupleSize * 6), tupleSize * sizeof(uint32_t));
        popCount++;
    }
    if (mask & 0b0000000010000000) {
        std::memcpy(destAddr + (tupleSize * popCount), sourceAddr + (tupleSize * 7), tupleSize * sizeof(uint32_t));
        popCount++;
    }
    if (mask & 0b0000000100000000) {
        std::memcpy(destAddr + (tupleSize * popCount), sourceAddr + (tupleSize * 8), tupleSize * sizeof(uint32_t));
        popCount++;
    }
    if (mask & 0b0000001000000000) {
        std::memcpy(destAddr + (tupleSize * popCount), sourceAddr + (tupleSize * 9), tupleSize * sizeof(uint32_t));
        popCount++;
    }
    if (mask & 0b0000010000000000) {
        std::memcpy(destAddr + (tupleSize * popCount), sourceAddr + (tupleSize * 10), tupleSize * sizeof(uint32_t));
        popCount++;
    }
    if (mask & 0b0000100000000000) {
        std::memcpy(destAddr + (tupleSize * popCount), sourceAddr + (tupleSize * 11), tupleSize * sizeof(uint32_t));
        popCount++;
    }
    if (mask & 0b0001000000000000) {
        std::memcpy(destAddr + (tupleSize * popCount), sourceAddr + (tupleSize * 12), tupleSize * sizeof(uint32_t));
        popCount++;
    }
    if (mask & 0b0010000000000000) {
        std::memcpy(destAddr + (tupleSize * popCount), sourceAddr + (tupleSize * 13), tupleSize * sizeof(uint32_t));
        popCount++;
    }
    if (mask & 0b0100000000000000) {
        std::memcpy(destAddr + (tupleSize * popCount), sourceAddr + (tupleSize * 14), tupleSize * sizeof(uint32_t));
        popCount++;
    }
    if (mask & 0b1000000000000000) {
        std::memcpy(destAddr + (tupleSize * popCount), sourceAddr + (tupleSize * 15), tupleSize * sizeof(uint32_t));
        popCount++;
    }

    return popCount;
}
}  // namespace RowStore
