#include "constructor.h"

#include <fstream>

#include "../includes/flogger.hpp"

void Constructor::initialize(std::filesystem::path inputFilePath) {
    console->clear();
    console->log(LogLevel::cyan, "[contructor]\n\n");
}
