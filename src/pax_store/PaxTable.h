#pragma once
#include <functional>
#include <vector>

#include "ITable.h"
#include "Memory.h"
#include "Page.h"
#include "SIMD.h"
#include "Types.h"

template <typename T>
class PaxTable : public Tables::ITable<T> {
   private:
    unsigned int rowsPerPage;
    unsigned int numberOfPages;

    /**
     * @brief Get index of the page
     *
     * @param rowIndex
     * @return int
     */
    int pageIndex(const int rowIndex) { return rowIndex / rowsPerPage; }

   public:
    PaxPage<T> *pages;

    PaxTable(int numberOfAttributes, int numberOfRows, const T **initialData)
        : Tables::ITable<T>(numberOfAttributes, numberOfRows, initialData) {
        auto pagesize = getPagesize();
        this->rowsPerPage = PaxPage<T>::getMaximumRows(pagesize, numberOfAttributes);

        cout << "Rows per page: " << rowsPerPage << endl;

        // Resize vector to contain all needed pages
        this->numberOfPages = (numberOfRows + (rowsPerPage - 1)) / rowsPerPage;  // Rounding up
        cout << "Needed pages: " << this->numberOfPages << endl;
        this->pages = (PaxPage<T> *)malloc(this->numberOfPages * sizeof(PaxPage<T>));

        // Initialize all pages
        for (int pageIndex = 0; pageIndex < numberOfPages; pageIndex++) {
            Buffer buffer = allocateAlignedBuffer(pagesize);
            PaxPage<T> page = PaxPage<T>((Header *)buffer, pagesize, numberOfAttributes);

            // Write data to page
            for (int rowIndex = pageIndex * rowsPerPage; rowIndex < (pageIndex + 1) * rowsPerPage && rowIndex < numberOfRows; rowIndex++) {
                page.writeRecord(initialData[rowIndex]);
            }

            *(this->pages + pageIndex) = page;
        }
    }

    // TODO: Free memory
    virtual ~PaxTable() { cout << "Destroy pax table" << endl; }

    T *getRow(uint64_t rowIndex) override {
        PaxPage<T> &page = pages[pageIndex(rowIndex)];
        return page.readRow(rowIndex % this->rowsPerPage);
    }

    T *projectRow(unsigned rowIndex, std::vector<uint64_t> &projection) {
        PaxPage<T> &page = pages[pageIndex(rowIndex)];
        return page.projectRow(rowIndex % this->rowsPerPage, projection);
    }

    T *getCell(const int rowIndex, const int columnIndex) {
        T *row = this->getRow(rowIndex);
        return row + columnIndex;
    }

    // Scalar query
    std::tuple<T **, uint64_t, uint64_t> queryTable(std::vector<uint64_t> &projection,
                                                    std::vector<Filters::Filter<T, SIMD::None> *> &filters) override {
        vector<unsigned> positions;

        // Find positions of all rows that match the given filters.
        for (unsigned pageIndex = 0; pageIndex < this->numberOfPages; pageIndex++) {
            PaxPage<T> page = this->pages[pageIndex];
            vector<unsigned> newPositions = page.query(filters);
            positions.insert(positions.end(), newPositions.begin(), newPositions.end());
        }

        // Allocate enough memory for all rows.
        T **data = (T **)malloc(positions.size() * sizeof(T *));
        for (int i = 0; i < positions.size(); i++) data[i] = this->projectRow(positions[i], projection);

        return std::make_tuple(data, positions.size(), projection.size());
    };

    // AVX512 query
    std::tuple<T **, uint64_t, uint64_t> queryTable(std::vector<uint64_t> &projection,
                                                    std::vector<Filters::Filter<T, SIMD::AVX512> *> &filters) override {
        vector<uint64_t> positions;

        // Find positions of all rows that match the given filters.
        for (unsigned pageIndex = 0; pageIndex < this->numberOfPages; pageIndex++) {
            PaxPage<T> page = this->pages[pageIndex];
            auto [pagePositions, count] = page.queryAVX(filters);
            positions.insert(positions.end(), pagePositions, pagePositions + count);
        }

        // Allocate enough memory for all rows.
        T **data = (T **)malloc(positions.size() * sizeof(T *));
        for (int i = 0; i < positions.size(); i++) data[i] = this->projectRow(positions[i], projection);

        return std::make_tuple(data, positions.size(), projection.size());
    };

    uint64_t queryCount(std::vector<uint64_t> &projection, std::vector<Filters::Filter<T, SIMD::None> *> &filters) override {
        return get<1>(queryTable(projection, filters));
    }

    uint64_t queryCount(std::vector<uint64_t> &projection, std::vector<Filters::Filter<T, SIMD::AVX512> *> &filters) override {
        return get<1>(queryTable(projection, filters));
    }
};
