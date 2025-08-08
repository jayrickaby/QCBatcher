#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include "createQCFiles.h"

int createQCFiles(std::filesystem::path inputPath, std::string SMD, std::filesystem::path templateQC)
{
    auto copyOptions{ std::filesystem::copy_options::overwrite_existing };

    std::filesystem::path newQCPath = { (inputPath / SMD).string() + ".qc" };

    // Check if already exists
    if (std::filesystem::exists(newQCPath)) {
        std::cout << "'" << SMD << ".qc'" << " already exists!" << '\n' << R"(Replace? (Y\N))" << '\n';

        while (true)
        {
            std::string userInput;
            std::getline(std::cin, userInput);
            if (userInput == "Y")
            {
                break;
            }
            else if (userInput == "N")
            {
                copyOptions = std::filesystem::copy_options::skip_existing;
                std::cout << "Skipping!" << '\n';
                return 0;
            }
            else
            {
                std::cout << "Invalid input!" << '\n';
            }

        }

    }

    // Copy template and rename
    std::filesystem::copy_file(templateQC, newQCPath, copyOptions);
    std::ifstream newQCFile(newQCPath);
    std::string newQCFileContent((std::istreambuf_iterator<char>(newQCFile)), std::istreambuf_iterator<char>());
    newQCFile.close();

    // Replace string with other strign
    std::string modelNameTarget{ "<MODELNAME>" };
    std::string modelNameReplacement{ SMD };
    size_t pos{ 0 };
    while ((pos = newQCFileContent.find(modelNameTarget, pos)) != std::string::npos) {
        newQCFileContent.replace(pos, modelNameTarget.length(), modelNameReplacement);
        pos += modelNameReplacement.length();
    }

    // Write to copied file
    std::ofstream newQCOutFile(newQCPath);
    newQCOutFile << newQCFileContent;
    newQCOutFile.close();

    std::cout << SMD << ".qc" << " has been created!" << '\n';

    return 0;
}