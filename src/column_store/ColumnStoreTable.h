#pragma once

#include <algorithm>
#include <vector>

#include "ColumnStoreHelper.h"
#include "Filters/Base.h"
#include "ITable.h"
#include "SIMD.h"

namespace ColumnStore {

template <typename T>
class Table : public Tables::ITable<T> {
   public:
    // the memory to store indices while filtering, is always enough to fit ALL indices
    uint64_t *indexStorage;
    // the actual used size of the index storage
    size_t sizeOfIndexStorage;

    Table(uint64_t numAttributes, uint64_t numRows, const T **initialData) : Tables::ITable<T>(numAttributes, numRows, initialData) {
        data = new std::vector<T>[numAttributes];
        for (uint64_t row = 0; row < numRows; row++) {
            for (uint64_t column = 0; column < numAttributes; column++) {
                data[column].push_back(initialData[row][column]);
            }
        }

        indexStorage = (uint64_t *)malloc(numRows * sizeof(uint64_t));
        sizeOfIndexStorage = -1;
    }

    ~Table() override = default;

    T *getRow(uint64_t rowIndex) override {
        auto returnRow = new T[this->numberOfAttributes];
        for (uint64_t i = 0; i < this->numberOfAttributes; i++) {
            returnRow[i] = data[i][rowIndex];
        }
        return returnRow;
    }

    std::tuple<T **, uint64_t, uint64_t> queryTable(std::vector<uint64_t> &projection,
                                                    std::vector<Filters::Filter<T, SIMD::None> *> &filters) override {
        std::vector<uint64_t> *filter_indices = nullptr;
        for (const auto &filter : filters) {
            filter_indices = filterBasic(filter, filter_indices);
        }

        return std::make_tuple(reconstructTable(projection, filter_indices), (uint64_t)filter_indices->size(), (uint64_t)projection.size());
    }

    std::tuple<T **, uint64_t, uint64_t> queryTable(std::vector<uint64_t> &projection,
                                                    std::vector<Filters::Filter<T, SIMD::AVX512> *> &filters) override {
        // reset the index storage
        sizeOfIndexStorage = -1;

        // apply each filter, (hopefully) reducing the index vector each iteration
        for (const auto &filter : filters) {
            filterBasic(filter);
        }

        return std::make_tuple(reconstructTableAVX(projection), (uint64_t)sizeOfIndexStorage, (uint64_t)projection.size());
    }

    std::tuple<T **, uint64_t, uint64_t> queryTable(std::vector<uint64_t> &projection,
                                                    std::vector<Filters::Filter<T, SIMD::AVX512_Strided> *> &filters) override {
        return std::make_tuple<(T **)nullptr, (uint64_t)-1, (uint64_t)-1>;
    }

    uint64_t queryCount(std::vector<uint64_t> &projection, std::vector<Filters::Filter<T, SIMD::None> *> &filters) override {
        // the first list of filtered indices is empty
        std::vector<uint64_t> *filter_indices = nullptr;

        // apply each filter, (hopefully) reducing the index vector each iteration
        for (const auto &filter : filters) {
            filter_indices = filterBasic(filter, filter_indices);
        }

        // accessing a portion of the memory (if possible) to avoid compiler optimizations
        auto final_result = reconstructTable(projection, filter_indices);
        if (!filter_indices->empty()) {
            auto access = final_result[filter_indices->size() - 1][projection.size() - 1];
        }

        return (uint64_t)filter_indices->size();
    }

    uint64_t queryCount(std::vector<uint64_t> &projection, std::vector<Filters::Filter<T, SIMD::AVX512> *> &filters) override {
        // reset the index storage
        sizeOfIndexStorage = -1;

        // apply each filter, (hopefully) reducing the index vector each iteration
        for (const auto &filter : filters) {
            filterBasic(filter);
        }

        // accessing a portion of the memory (if possible) to avoid compiler optimizations
        auto final_result = reconstructTableAVX(projection);
        if (sizeOfIndexStorage != 0) {
            auto access = final_result[sizeOfIndexStorage - 1][projection.size() - 1];
        }

        return (uint64_t)sizeOfIndexStorage;
    }

    uint64_t queryCount(std::vector<uint64_t> &projection, std::vector<Filters::Filter<T, SIMD::AVX512_Strided> *> &filters) override {
        return (uint64_t)-1;
    }

   private:
    /// Applies a single filter on the stored data based on a vector of input indices from previous filtering.
    /// \param filter the filter to apply
    /// \param input_indices the vector of indices that are still in the filtering. This can be a nullptr in which case the entire set of
    /// data is accessed. \return a (reduced) vector of indices for rows that match this filter
    std::vector<uint64_t> *filterBasic(Filters::Filter<T, SIMD::None> *filter, std::vector<uint64_t> *input_indices = nullptr) {
        auto output_indices = new std::vector<uint64_t>();
        auto filter_col = data[filter->index];

        if (input_indices) {  // input_indices is not empty
            for (const auto &row : *input_indices) {
                if (filter->match(filter_col.at(row))) output_indices->push_back(row);
            }
        } else {  // input_indices is empty, iterate over the entire set of data
            for (uint64_t row = 0; row < this->numberOfRows; row++) {
                if (filter->match(filter_col.at(row))) output_indices->push_back(row);
            }
        }

        // free the old input_indices memory
        free(input_indices);
        return output_indices;
    }

    /// Applies a single filter on the stored data based the currently stored filter indices from previous
    /// iterations (if there was a previous iteration).
    /// \param filter the filter to apply
    void filterBasic(const Filters::Filter<T, SIMD::AVX512> *filter) {
        auto integerAmount = (unsigned)64 / sizeof(T);
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
    /// \param row_indices the rows to pack into the table
    T **reconstructTable(std::vector<uint64_t> &projection, std::vector<uint64_t> *row_indices) {
        T **to_return = (T **)malloc(row_indices->size() * sizeof(T *));

        for (uint64_t row = 0; row < row_indices->size(); row++) {
            T *rowData = (T *)malloc(projection.size() * sizeof(T));
            to_return[row] = rowData;
            for (uint64_t projection_index = 0; projection_index < projection.size(); projection_index++) {
                to_return[row][projection_index] = data[projection.at(projection_index)][row_indices->at(row)];
            }
        }

        return to_return;
    }

    /// Constructs and returns a table (2-dimensional C-array) based on prior filtering and using projection.
    /// \param projection the columns to pack into the table
    T **reconstructTableAVX(std::vector<uint64_t> &projection) {
        T **to_return = (T **)malloc(sizeOfIndexStorage * sizeof(T *));

        for (uint64_t row = 0; row < sizeOfIndexStorage; row++) {
            T *rowData = (T *)malloc(projection.size() * sizeof(T));
            to_return[row] = rowData;
            for (uint64_t projection_index = 0; projection_index < projection.size(); projection_index++) {
                to_return[row][projection_index] = data[projection.at(projection_index)][indexStorage[row]];
            }
        }

        return to_return;
    }

   public:
    std::vector<T> *data;
};
}  // namespace ColumnStore