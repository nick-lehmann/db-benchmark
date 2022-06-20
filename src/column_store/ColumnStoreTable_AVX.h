#pragma once

#include <algorithm>
#include <vector>

#include "ITable_AVX.h"
#include "ColumnStoreHelper.h"
#include "Filters/Base.h"

// whether to check the remainder in the filterBasic()-method
// this could have an impact on performance due to unfortunate branch prediction
// TODO benchmark performance
#define WITH_R_0_CHECK 1

namespace ColumnStore::AVX {
    template<typename T>
    class Table : public Tables::AVX::ITable<T> {
    public:
        Table(uint64_t numAttributes, uint64_t numRows, const T **initialData)
                : Tables::AVX::ITable<T>(numAttributes, numRows, initialData) {
            data = new std::vector<T>[numAttributes];
            indexStorage = (uint64_t *) malloc(numRows * sizeof(uint64_t));
            sizeOfIndexStorage = -1;

            for (uint64_t row = 0; row < numRows; row++) {
                for (uint64_t column = 0; column < numAttributes; column++) {
                    data[column].push_back(initialData[row][column]);
                }
            }
        }

        ~Table() override = default;

        T *getRow(uint64_t rowIndex) override {
            auto returnRow = new T[this->numberOfAttributes];
            for (uint64_t i = 0; i < this->numberOfAttributes; i++) {
                returnRow[i] = data[i][rowIndex];
            }
            return returnRow;
        }

        std::tuple<T **, uint64_t, uint64_t>
        queryTable(std::vector<uint64_t> &projection, std::vector<Filter::Filter<T, SIMD::AVX512> *> &filters) override {
            // reset the index storage
            sizeOfIndexStorage = -1;

            // apply each filter, (hopefully) reducing the index vector each iteration
            for (const auto &filter: filters) {
                filterBasic(filter);
            }

            return std::make_tuple(reconstructTable(projection),
                                   (uint64_t) sizeOfIndexStorage, (uint64_t) projection.size());
        }

        uint64_t queryCount(std::vector<uint64_t> &projection, std::vector<Filter::Filter<T, SIMD::AVX512> *> &filters) override {
            // reset the index storage
            sizeOfIndexStorage = -1;

            // apply each filter, (hopefully) reducing the index vector each iteration
            for (const auto &filter: filters) {
                filterBasic(filter);
            }

            // accessing a portion of the memory (if possible) to avoid compiler optimizations
            auto final_result = reconstructTable(projection);
            if (sizeOfIndexStorage != 0) {
                auto access = final_result[sizeOfIndexStorage - 1][projection.size() - 1];
            }

            return (uint64_t) sizeOfIndexStorage;
        }

    private:
        /// Applies a single filter on the stored data based the currently stored filter indices from previous
        /// iterations (if there was a previous iteration).
        /// \param filter the filter to apply
        void filterBasic(const Filter::Filter<T, SIMD::AVX512> *filter) {
            auto integerAmount = (unsigned) 64 / sizeof(T);
            auto filterColIterator = data[filter->index].begin();
            auto *currentIndexStorage = indexStorage;
            auto currentSizeOfIndexStorage = sizeOfIndexStorage;

            if (sizeOfIndexStorage == -1) {
                sizeOfIndexStorage = 0;
                // loop over all rows

                for (size_t rowIndex = 0; rowIndex < this->numberOfRows; rowIndex += integerAmount) {
                    auto [dataRegister, indexRegister] = ColumnStore::Helper::load(&(*filterColIterator), rowIndex);
                    auto filterResult = filter->match(dataRegister);
                    auto addedElements = ColumnStore::Helper::store(indexRegister, filterResult, currentIndexStorage);

                    sizeOfIndexStorage += addedElements;
                    currentIndexStorage += addedElements;
                    filterColIterator += integerAmount;
                }
            } else {
                sizeOfIndexStorage = 0;
                // loop over existing indices

                // calculate how many elements don't divide evenly into the registers
                auto r = currentSizeOfIndexStorage % integerAmount;
#if WITH_R_0_CHECK
                if (r != 0) {
#endif
                    // construct a mask based on how many remaining elements there are
                    auto mask = _mm512_int2mask(~(0xffffffff << r));

                    // apply a filtering using the calculated mask
                    auto [dataReg, indexReg] = ColumnStore::Helper::gather(currentIndexStorage, &(*filterColIterator));
                    auto maskedResult = filter->match(dataReg, mask);
                    auto maskedElements = ColumnStore::Helper::store(indexReg, maskedResult, currentIndexStorage);

                    sizeOfIndexStorage += maskedElements;
                    currentIndexStorage += maskedElements;
#if WITH_R_0_CHECK
                }
#endif

                for (size_t rowIndex = r; rowIndex < currentSizeOfIndexStorage; rowIndex += integerAmount) {
                    auto [dataRegister, indexRegister] = ColumnStore::Helper::gather(currentIndexStorage, &(*filterColIterator));
                    auto filterResult = filter->match(dataRegister);
                    auto addedElements = ColumnStore::Helper::store(indexRegister, filterResult, currentIndexStorage);

                    sizeOfIndexStorage += addedElements;
                    currentIndexStorage += addedElements;
                }
            }
        }

        /// Constructs and returns a table (2-dimensional C-array) based on prior filtering and using projection.
        /// \param projection the columns to pack into the table
        T **reconstructTable(std::vector<uint64_t> &projection) {
            T **to_return = (T **) malloc(sizeOfIndexStorage * sizeof(T *));

            for (uint64_t row = 0; row < sizeOfIndexStorage; row++) {
                T *rowData = (T *) malloc(projection.size() * sizeof(T));
                to_return[row] = rowData;
                for (uint64_t projection_index = 0; projection_index < projection.size(); projection_index++) {
                    to_return[row][projection_index] = data[projection.at(projection_index)][indexStorage[row]];
                }
            }

            return to_return;
        }

    public:
        std::vector<T> *data;

        // the memory to store indices while filtering, is always enough to fit ALL indices
        uint64_t *indexStorage;
        // the actual used size of the index storage
        size_t sizeOfIndexStorage;
    };
}