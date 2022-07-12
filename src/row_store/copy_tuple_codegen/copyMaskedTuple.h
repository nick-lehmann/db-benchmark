#pragma once
#include <cstring>
#include <cstdint>

namespace RowStore {

template<typename T>
uint8_t _copyTuple(T * destAddr, T * sourceAddr,
                   size_t tupleSize, uint8_t mask) {
   switch(mask) {
   case 0:
       return 0;
   case 1:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 1;
   case 2:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 1;
   case 4:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       return 1;
   case 8:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       return 1;
   case 16:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       return 1;
   case 32:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       return 1;
   case 64:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       return 1;
   case 128:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       return 1;
   case 3:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 2;
   case 5:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 2;
   case 6:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 2);
       return 2;
   case 9:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 2;
   case 10:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 2;
   case 12:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 2);
       return 2;
   case 17:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 2;
   case 18:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 2;
   case 20:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       return 2;
   case 24:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 2);
       return 2;
   case 33:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 2;
   case 34:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 2;
   case 36:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       return 2;
   case 40:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       return 2;
   case 48:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 2);
       return 2;
   case 65:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 2;
   case 66:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 2;
   case 68:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       return 2;
   case 72:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       return 2;
   case 80:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       return 2;
   case 96:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 2);
       return 2;
   case 129:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 2;
   case 130:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 2;
   case 132:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       return 2;
   case 136:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       return 2;
   case 144:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       return 2;
   case 160:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       return 2;
   case 192:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       return 2;
   case 7:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 3);
       return 3;
   case 11:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 3;
   case 13:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 3;
   case 14:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 3);
       return 3;
   case 19:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 3;
   case 21:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 3;
   case 22:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 2);
       return 3;
   case 25:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 3;
   case 26:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 3;
   case 28:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 3);
       return 3;
   case 35:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 3;
   case 37:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 3;
   case 38:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 2);
       return 3;
   case 41:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 3;
   case 42:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 3;
   case 44:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 2);
       return 3;
   case 49:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 3;
   case 50:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 3;
   case 52:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       return 3;
   case 56:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 3);
       return 3;
   case 67:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 3;
   case 69:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 3;
   case 70:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 2);
       return 3;
   case 73:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 3;
   case 74:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 3;
   case 76:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 2);
       return 3;
   case 81:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 3;
   case 82:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 3;
   case 84:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       return 3;
   case 88:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 2);
       return 3;
   case 97:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 3;
   case 98:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 3;
   case 100:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       return 3;
   case 104:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       return 3;
   case 112:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 3);
       return 3;
   case 131:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 3;
   case 133:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 3;
   case 134:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 2);
       return 3;
   case 137:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 3;
   case 138:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 3;
   case 140:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 2);
       return 3;
   case 145:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 3;
   case 146:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 3;
   case 148:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       return 3;
   case 152:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 2);
       return 3;
   case 161:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 3;
   case 162:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 3;
   case 164:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       return 3;
   case 168:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       return 3;
   case 176:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 2);
       return 3;
   case 193:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 3;
   case 194:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 3;
   case 196:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       return 3;
   case 200:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       return 3;
   case 208:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       return 3;
   case 224:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 3);
       return 3;
   case 15:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 4);
       return 4;
   case 23:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 3);
       return 4;
   case 27:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 4;
   case 29:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 4;
   case 30:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 4);
       return 4;
   case 39:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 3);
       return 4;
   case 43:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 4;
   case 45:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 4;
   case 46:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 3);
       return 4;
   case 51:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 4;
   case 53:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 4;
   case 54:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 2);
       return 4;
   case 57:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 4;
   case 58:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 4;
   case 60:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 4);
       return 4;
   case 71:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 3);
       return 4;
   case 75:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 4;
   case 77:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 4;
   case 78:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 3);
       return 4;
   case 83:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 4;
   case 85:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 4;
   case 86:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 2);
       return 4;
   case 89:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 4;
   case 90:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 4;
   case 92:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 3);
       return 4;
   case 99:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 4;
   case 101:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 4;
   case 102:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 2);
       return 4;
   case 105:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 4;
   case 106:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 4;
   case 108:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 2);
       return 4;
   case 113:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 4;
   case 114:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 4;
   case 116:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       return 4;
   case 120:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 4);
       return 4;
   case 135:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 3);
       return 4;
   case 139:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 4;
   case 141:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 4;
   case 142:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 3);
       return 4;
   case 147:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 4;
   case 149:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 4;
   case 150:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 2);
       return 4;
   case 153:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 4;
   case 154:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 4;
   case 156:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 3);
       return 4;
   case 163:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 4;
   case 165:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 4;
   case 166:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 2);
       return 4;
   case 169:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 4;
   case 170:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 4;
   case 172:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 2);
       return 4;
   case 177:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 4;
   case 178:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 4;
   case 180:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       return 4;
   case 184:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 3);
       return 4;
   case 195:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 4;
   case 197:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 4;
   case 198:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 2);
       return 4;
   case 201:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 4;
   case 202:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 4;
   case 204:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 2);
       return 4;
   case 209:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 4;
   case 210:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 4;
   case 212:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       return 4;
   case 216:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 2);
       return 4;
   case 225:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 4;
   case 226:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 4;
   case 228:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       return 4;
   case 232:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       return 4;
   case 240:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 4);
       return 4;
   case 31:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 5);
       return 5;
   case 47:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 4);
       return 5;
   case 55:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 3);
       return 5;
   case 59:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 5;
   case 61:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 4);
       std::memcpy(destAddr + (tupleSize * 4), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 5;
   case 62:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 5);
       return 5;
   case 79:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 4);
       return 5;
   case 87:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 3);
       return 5;
   case 91:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 5;
   case 93:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 4), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 5;
   case 94:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 4);
       return 5;
   case 103:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 3);
       return 5;
   case 107:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 5;
   case 109:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 4), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 5;
   case 110:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 3);
       return 5;
   case 115:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 5;
   case 117:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 4), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 5;
   case 118:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 2);
       return 5;
   case 121:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 4);
       std::memcpy(destAddr + (tupleSize * 4), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 5;
   case 122:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 4);
       std::memcpy(destAddr + (tupleSize * 4), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 5;
   case 124:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 5);
       return 5;
   case 143:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 4);
       return 5;
   case 151:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 3);
       return 5;
   case 155:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 5;
   case 157:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 4), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 5;
   case 158:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 4);
       return 5;
   case 167:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 3);
       return 5;
   case 171:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 5;
   case 173:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 4), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 5;
   case 174:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 3);
       return 5;
   case 179:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 5;
   case 181:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 4), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 5;
   case 182:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 2);
       return 5;
   case 185:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 4), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 5;
   case 186:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 4), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 5;
   case 188:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 4);
       return 5;
   case 199:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 3);
       return 5;
   case 203:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 5;
   case 205:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 4), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 5;
   case 206:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 3);
       return 5;
   case 211:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 5;
   case 213:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 4), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 5;
   case 214:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 2);
       return 5;
   case 217:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 4), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 5;
   case 218:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 4), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 5;
   case 220:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 3);
       return 5;
   case 227:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 5;
   case 229:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 4), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 5;
   case 230:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 2);
       return 5;
   case 233:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 4), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 5;
   case 234:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 4), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 5;
   case 236:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 2);
       return 5;
   case 241:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 4);
       std::memcpy(destAddr + (tupleSize * 4), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 5;
   case 242:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 4);
       std::memcpy(destAddr + (tupleSize * 4), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 5;
   case 244:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 4);
       std::memcpy(destAddr + (tupleSize * 4), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       return 5;
   case 248:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 5);
       return 5;
   case 63:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 6);
       return 6;
   case 95:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 5);
       return 6;
   case 111:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 4);
       return 6;
   case 119:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 3);
       return 6;
   case 123:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 4);
       std::memcpy(destAddr + (tupleSize * 4), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 6;
   case 125:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 5);
       std::memcpy(destAddr + (tupleSize * 5), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 6;
   case 126:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 6);
       return 6;
   case 159:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 5);
       return 6;
   case 175:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 4);
       return 6;
   case 183:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 3);
       return 6;
   case 187:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 4), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 6;
   case 189:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 4);
       std::memcpy(destAddr + (tupleSize * 5), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 6;
   case 190:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 5);
       return 6;
   case 207:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 4);
       return 6;
   case 215:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 3);
       return 6;
   case 219:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 4), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 6;
   case 221:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 5), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 6;
   case 222:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 4);
       return 6;
   case 231:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 3);
       return 6;
   case 235:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 4), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 6;
   case 237:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 5), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 6;
   case 238:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 3);
       return 6;
   case 243:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 4);
       std::memcpy(destAddr + (tupleSize * 4), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 6;
   case 245:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 4);
       std::memcpy(destAddr + (tupleSize * 4), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 5), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 6;
   case 246:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 4);
       std::memcpy(destAddr + (tupleSize * 4), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 2);
       return 6;
   case 249:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 5);
       std::memcpy(destAddr + (tupleSize * 5), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 6;
   case 250:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 5);
       std::memcpy(destAddr + (tupleSize * 5), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 1);
       return 6;
   case 252:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 6);
       return 6;
   case 127:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 1), tupleSize * sizeof(T) * 7);
       return 7;
   case 191:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 1);
       std::memcpy(destAddr + (tupleSize * 1), sourceAddr + (tupleSize * 2), tupleSize * sizeof(T) * 6);
       return 7;
   case 223:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 2);
       std::memcpy(destAddr + (tupleSize * 2), sourceAddr + (tupleSize * 3), tupleSize * sizeof(T) * 5);
       return 7;
   case 239:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 3);
       std::memcpy(destAddr + (tupleSize * 3), sourceAddr + (tupleSize * 4), tupleSize * sizeof(T) * 4);
       return 7;
   case 247:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 4);
       std::memcpy(destAddr + (tupleSize * 4), sourceAddr + (tupleSize * 5), tupleSize * sizeof(T) * 3);
       return 7;
   case 251:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 5);
       std::memcpy(destAddr + (tupleSize * 5), sourceAddr + (tupleSize * 6), tupleSize * sizeof(T) * 2);
       return 7;
   case 253:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 6);
       std::memcpy(destAddr + (tupleSize * 6), sourceAddr + (tupleSize * 7), tupleSize * sizeof(T) * 1);
       return 7;
   case 254:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 7);
       return 7;
   case 255:
       std::memcpy(destAddr + (tupleSize * 0), sourceAddr + (tupleSize * 0), tupleSize * sizeof(T) * 8);
       return 8;
   }
}


uint8_t copyMaskedTuple(uint64_t * destAddr, uint64_t * sourceAddr, size_t tupleSize, uint8_t mask) {
    return _copyTuple(destAddr, sourceAddr, tupleSize, mask);
}

uint8_t copyMaskedTuple(uint32_t * destAddr, uint32_t * sourceAddr, size_t tupleSize, uint16_t mask) {
    uint8_t num1 = _copyTuple(destAddr                     , sourceAddr                  , tupleSize, (uint8_t) mask);
    uint8_t num2 = _copyTuple(destAddr + (tupleSize * num1), sourceAddr + (tupleSize * 8), tupleSize, (uint8_t)(mask>>8));
    return num1 + num2;
} 
} // namespace RowStore
