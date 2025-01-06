#include "hexAgon/constructor.h"
#include "hexAgon/hexDumper.h"
#include "includes/flogger.hpp"

int main(int argc, char* argv[]) {
    console->clear();
    console->log(LogLevel::orange, "[Hex-Agon]\n\n");

    std::filesystem::path inputFilePath;
    bool dump      = false;
    bool construct = false;

    if (argc == 1) {
        inputFilePath = console->getInput<std::string>("input file/path ->");

    } else if (argc == 2) {
        inputFilePath = argv[1];
        console->getInput<std::string>(
            "----------\n"
            "[c] construct\n"
            "[d] dump"
            "\n----------\n") == "c"
            ? construct = true
            : dump      = true;

    } else if (argc == 3) {
        inputFilePath = argv[1];

        for (int i = 1; i < argc; i++) {
            if (construct == false) construct = !strcmp(argv[i], "-c");
            if (dump == false) dump = !strcmp(argv[i], "-d");
        }
    }

    if (!std::filesystem::exists(inputFilePath)) {
        console->clear();
        console->report(LogLevel::error, "%s", console->getLastError().c_str());
        return 1;
    }

    if (dump == construct) {
        console->clear();
        console->report(LogLevel::error, "%s", console->getLastError().c_str());
        return 1;
    } else if (dump) {
        HexDumper::initialize(inputFilePath);
    } else if (construct) {
        Constructor::initialize(inputFilePath);
    }

    system("pause");
    return 0;
}
