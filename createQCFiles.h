#pragma once
#include <filesystem>
#include <string>

int createQCFiles(std::filesystem::path inputPath, std::string SMD, std::filesystem::path templateQC);
