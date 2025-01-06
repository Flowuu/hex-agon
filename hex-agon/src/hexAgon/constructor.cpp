#include "constructor.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "../includes/flogger.hpp"

void Constructor::initialize(std::filesystem::path inputFilePath) {
    console->clear();
    console->log(LogLevel::cyan, "[constructor]\n\n");

    std::string extension = console->getInput<std::string>("input the output file extension: ");
    if (extension.empty()) {
        size_t lastDot = inputFilePath.string().rfind('.');

        if (lastDot != std::string::npos && lastDot != 0 && lastDot != inputFilePath.string().length() - 1) {
            extension = inputFilePath.string().substr(lastDot + 1);
        }
    }

    std::filesystem::path outputFilePath = inputFilePath.stem();
    outputFilePath.replace_extension(extension);

    std::ifstream inputFile(inputFilePath, std::ios_base::in);
    if (!inputFile.is_open()) {
        console->report(LogLevel::error, "%s", console->getLastError().c_str());
        return;
    }

    std::ofstream outputFile(outputFilePath, std::ios_base::trunc);
    if (!outputFile.is_open()) {
        console->report(LogLevel::error, "%s", console->getLastError().c_str());
        return;
    }

    // thinking

    outputFile.close();
    inputFile.close();
}
