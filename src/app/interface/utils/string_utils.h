#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdexcept>

/// @brief Convert a string to lowercase.
/// @param t_string Input string.
/// @return Lowercase version of the input string.
inline std::string toLower(std::string t_string) {
    std::transform(t_string.begin(), t_string.end(), t_string.begin(), [](unsigned char c) { return std::tolower(c); });
    return t_string;
}

/// @brief Read the entire contents of a file into a string.
/// @param t_filepath Path to the file to read.
/// @return Contents of the file as a std::string.
/// @throws std::runtime_error if the file cannot be opened.
inline std::string readFile(const std::string& t_filepath) {
    std::ifstream file{t_filepath, std::ios::ate | std::ios::binary};
    if (!file.is_open())
        throw std::runtime_error("Failed to open file: " + t_filepath);

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return std::string(buffer.begin(), buffer.end());
}
