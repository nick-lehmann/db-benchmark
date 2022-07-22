#include <filesystem>

template <typename T>
void writeDatasetToFile(std::string path) {
    auto numberOfAttributes = 50;
    auto numberOfRows = 100000;
    const T **data = TableHelper::generateRandomData<T>(numberOfAttributes, numberOfRows, 0, 100);

    std::ofstream out;
    out.open(path, std::ios::out);

    // Write header
    for (unsigned column = 0; column < numberOfAttributes; column++) {
        out << "c" << column;
        if (column != numberOfAttributes - 1) {
            out << ",";
        }
    }
    out << endl;

    // Write header
    for (unsigned row = 0; row < numberOfRows; row++) {
        for (unsigned column = 0; column < numberOfAttributes; column++) {
            out << data[row][column];
            if (column != numberOfAttributes - 1) {
                out << ",";
            }
        }
        out << endl;
    }
    out.close();
}

template <typename T>
void ensureDatasetExists(std::string path) {
    if (!std::filesystem::exists(path)) {
        writeDatasetToFile<T>();
    }
}