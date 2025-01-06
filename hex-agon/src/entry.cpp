#include "hexDumper.h"
#include "includes/flogger.hpp"

int main(int argc, char* argv[]) {
    console->clear();
    console->log(LogLevel::orange, "[Hex-Agon]\n\n");

    std::filesystem::path inputFilePath;

    if (argc == 1) {
        inputFilePath = console->getInput<std::string>("input file/path ->");
    } else {
        inputFilePath = argv[1];
    }

    HexDumper::initialize(inputFilePath);

    return 0;
}
