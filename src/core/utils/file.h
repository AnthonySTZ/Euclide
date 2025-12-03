#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

namespace euclide {

/// @brief Read a given file and return a vector of `char`. Useful for shader files
/// @param t_filePath
/// @return Vector of char
inline std::vector<char> readFile(const std::string& t_filePath) {
    std::ifstream file{t_filePath, std::ios::ate | std::ios::binary};

    if (!file.is_open()) {
        std::cout << "Failed to open file: " + t_filePath + "\n";
        throw std::runtime_error("Failed to open file: " + t_filePath);
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();
    return buffer;
}

} // namespace euclide