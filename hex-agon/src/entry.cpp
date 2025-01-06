#include "hexAgon/constructor.h"
#include "hexAgon/hexDumper.h"
#include "includes/flogger.hpp"

std::filesystem::path inputFilePath;
bool dump      = false;
bool construct = false;

int main(int argc, char* argv[]) {
    console->clear();
    console->log(LogLevel::orange, "[Hex-Agon]\n\n");

    if (argc == 1)
        inputFilePath = console->getInput<std::string>("input file/path ->");
    else if (argc == 2)
        inputFilePath = argv[1];
    else if (argc == 3) {
        inputFilePath = argv[1];

        for (int i = 1; i < argc; i++) {
            if (construct == false) construct = !strcmp(argv[i], "-c");
            if (dump == false) dump = !strcmp(argv[i], "-d");
        }

        // cant do both
        if (dump == construct) {
            console->clear();
            console->report(LogLevel::error, "%s", console->getLastError().c_str());
            return 1;
        }
    }

    // dumping dump XD
    if (!strcmp(inputFilePath.extension().string().c_str(), ".bin") && !strcmp(inputFilePath.extension().string().c_str(), ".dump")) {
        dump      = false;
        construct = true;
    }

    if (!std::filesystem::exists(inputFilePath)) {
        console->clear();
        console->report(LogLevel::error, "%s", console->getLastError().c_str());
        return 1;
    }

    // for argc 1 and 2
    if (!construct && !dump) {
        console->getInput<std::string>(
            "----------\n"
            "[c] construct\n"
            "[d] dump"
            "\n----------\n") == "c"
            ? construct = true
            : dump      = true;
    }

    if (dump)
        HexDumper::initialize(inputFilePath);
    else if (construct)
        Constructor::initialize(inputFilePath);

    system("pause");
    return 0;
}
