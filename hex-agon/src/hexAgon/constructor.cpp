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
        std::cout << inputFilePath.stem() << '\n';
    }
}
