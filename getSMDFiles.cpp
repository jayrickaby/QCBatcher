#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include "getSMDFiles.h"

std::vector<std::string> getSMDFiles(std::filesystem::path path)
{
    std::vector<std::string> fileList;

    // Goes through each file in the path
    for (const auto& fileEntry : std::filesystem::directory_iterator(path)) {
        const auto& filePath = fileEntry.path();

        // If no extension / doesn't end with .smd, continue
        if (!filePath.has_extension() || filePath.extension().string() != ".smd")
            continue;

        // Make it lowercase
        std::string stem = filePath.stem().string();
        std::transform(stem.begin(), stem.end(), stem.begin(),
            [](unsigned char c) { return std::tolower(c); });

        // Check if lowercase
        if (stem.size() >= 8 && stem.ends_with("physics"))
            continue;

        // All conditions are good, then add to vector!! :D
        fileList.push_back(filePath.stem().string());

        std::cout << filePath.filename() << " has been detected." << "\n";
    }

    return fileList;
}
