#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

std::string exec(const std::string cmd) {
    auto pPipe = ::popen(cmd.c_str(), "r");
    if (pPipe == nullptr) {
        throw std::runtime_error("Cannot open pipe");
    }
    std::array<char, 256> buffer;
    std::string result;
    while (not std::feof(pPipe)) {
        auto bytes = std::fread(buffer.data(), 1, buffer.size(), pPipe);
        result.append(buffer.data(), bytes);
    }

    auto rc = ::pclose(pPipe);

    return result;
}
