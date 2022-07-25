#include <filesystem>
#include <string>

// Default limiter for Sqlite3
// When importing from the shell, it is not possible to set sqlite3 into csv mode and import at the same time. Therefore, we have to use the
// pipe as it is the default delimiter.
const std::string delimiter = "|";

template <typename T>
void writeDatasetToFile(std::string path, uint64_t numberOfAttributes, uint64_t numberOfRows, const T **data) {
    std::remove(path.c_str());

    std::ofstream out;
    out.open(path, std::ios::out);

    // Write data
    for (unsigned row = 0; row < numberOfRows; row++) {
        for (unsigned column = 0; column < numberOfAttributes; column++) {
            out << data[row][column];
            if (column != numberOfAttributes - 1) {
                out << delimiter;
            }
        }
        out << endl;
    }
    out.close();
}
