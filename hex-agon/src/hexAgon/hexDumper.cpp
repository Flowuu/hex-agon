#include "hexDumper.h"

#include <fstream>

#include "../includes/flogger.hpp"

void HexDumper::initialize(std::filesystem::path inputFilePath) {
    console->clear();
    console->log(LogLevel::cyan, "[hex dumper]\n\n");

    std::ofstream outputFile(inputFilePath.filename().string() + ".bin", std::ios::trunc);
    if (!outputFile.is_open()) {
        console->report(LogLevel::error, "%s", console->getLastError().c_str());
        return;
    }

    HANDLE hInputFile = CreateFileA(inputFilePath.string().c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (hInputFile == INVALID_HANDLE_VALUE) {
        console->report(LogLevel::error, "%s", console->getLastError().c_str());
        return;
    }

    size_t inputFileSize = GetFileSize(hInputFile, nullptr);
    if (inputFileSize == 0) {
        console->report(LogLevel::error, "%s", console->getLastError().c_str());
        CloseHandle(hInputFile);
        return;
    }

    console->log("input file: %s | size: %d kb |\n", inputFilePath.filename().string().c_str(), static_cast<int>(inputFileSize) / 1000);

    void* pInputFileBuff = VirtualAlloc(nullptr, inputFileSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (pInputFileBuff == nullptr) {
        console->report(LogLevel::error, "%s", console->getLastError().c_str());
        CloseHandle(hInputFile);
        return;
    }

    DWORD readBytes;
    if (!ReadFile(hInputFile, pInputFileBuff, static_cast<DWORD>(inputFileSize), &readBytes, nullptr)) {
        console->report(LogLevel::error, "%s", console->getLastError().c_str());
        CloseHandle(hInputFile);
        return;
    }

    console->log("read bytes: %d kb\n", static_cast<int>(readBytes) / 1000);

    console->log(LogLevel::orange, "\n[hex view]\n");

    uint8_t* buffer = reinterpret_cast<uint8_t*>(pInputFileBuff);
    for (size_t i = 0; i <= readBytes; i++) {
        outputFile << "0x" << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << static_cast<int>(buffer[i]) << ", ";

        std::cout << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << static_cast<int>(buffer[i]) << " ";

        if ((i + 1) % 16 == 0) {
            outputFile << '\n';
            std::cout << '\n';
        }
    }

    console->log("\n\n");

    CloseHandle(hInputFile);
    outputFile.close();

    console->report(LogLevel::success, "Finished dumping\n\n");
}
