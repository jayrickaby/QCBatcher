#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <fstream>

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

int createQCFiles(std::filesystem::path inputPath, std::string SMD, std::filesystem::path templateQC)
{
    auto copyOptions{ std::filesystem::copy_options::overwrite_existing };

    std::filesystem::path newQCPath = { (inputPath / SMD).string() + ".qc" };

    // Check if already exists
    if (std::filesystem::exists(newQCPath)) {
        std::cout << "'" << SMD << ".qc'" << " already exists!" << '\n' << R"(Replace? (Y\N))" << '\n';
        
        while(true)
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
