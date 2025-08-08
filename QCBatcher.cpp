#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <fstream>
#include "getSMDFiles.h"
#include "createQCFiles.h"

int main()
{
    std::filesystem::path inputPath{R"(.\)"};
    std::filesystem::path templateQC{"template.qc"};
    
    std::string userInput;

    std::cout << "Please enter input path (leave blank for same path as program):" << '\n' << ">";
    std::getline(std::cin, userInput);
    if (!userInput.empty()) {
        inputPath = userInput;
    }
    std::cout << "Please enter target qc name (leave blank for 'target.qc'):" << '\n' << ">";
    std::getline(std::cin, userInput);
    if (!userInput.empty()) {
        templateQC = userInput;
    }
    std::filesystem::path templateQCPath { inputPath / templateQC };
    if (!std::filesystem::exists(templateQCPath)) {
        std::cout << templateQC << " does not exist!" << '\n';
        return 1;
    }

    std::vector<std::string> SMDFiles{ getSMDFiles(inputPath) };


    for (const auto& SMD : SMDFiles)
    {
        createQCFiles(inputPath, SMD, templateQCPath);
    }

    return 0;
}
