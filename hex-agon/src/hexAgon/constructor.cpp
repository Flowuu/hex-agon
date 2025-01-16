#include "constructor.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "../includes/flogger.hpp"

PIMAGE_DOS_HEADER pDosHeader;
PIMAGE_NT_HEADERS pNtHeaders;

void Constructor::initialize(std::filesystem::path inputFilePath) {
    std::string outputFileName = inputFilePath.stem().string();

    console->clear();
    console->log(LogLevel::cyan, "[constructor]\n\n");

    std::ifstream inputFile(inputFilePath);
    if (!inputFile.is_open()) {
        console->report(LogLevel::error, "%s\n", console->getLastError().c_str());
        return;
    }

    std::vector<uint8_t> u8Data;
    std::string strData;
    while (inputFile >> strData) {
        static int i = 0;
        i++;

        strData.erase(0, 2);
        strData.erase(strData.find(','), strData.find(','));
        // cooked
        u8Data.push_back(static_cast<uint8_t>(std::stoi(strData, nullptr, 16)));
    }

    inputFile.close();

    // safety check
    bool hasHeaders = u8Data.size() >= sizeof(IMAGE_DOS_HEADER) + sizeof(IMAGE_NT_HEADERS);

    if (hasHeaders) {
        pDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(u8Data.data());
        pNtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>((LPBYTE)pDosHeader + pDosHeader->e_lfanew);
    }

    if (inputFilePath.stem().extension().string().empty()) {
        if (hasHeaders) {
            if (pNtHeaders->FileHeader.Characteristics & (WORD)IMAGE_FILE_DLL) {
                outputFileName += ".dll";
            } else if (pNtHeaders->FileHeader.Characteristics & (WORD)IMAGE_FILE_EXECUTABLE_IMAGE) {
                outputFileName += ".exe";
            } else {
                std::string ouputFileExt = console->getInput<std::string>("\ninput the output file extension: ");
                if (ouputFileExt.contains("."))
                    outputFileName += ouputFileExt;
                else
                    outputFileName += "." + ouputFileExt;
            }
        } else {
            std::string ouputFileExt = console->getInput<std::string>("\ninput the output file extension: ");
            if (ouputFileExt.contains("."))
                outputFileName += ouputFileExt;
            else
                outputFileName += "." + ouputFileExt;
        }
    } else {
        outputFileName = inputFilePath.stem().string();
    }

    std::ofstream ouputFile(outputFileName, std::ios_base::binary);
    if (!ouputFile.is_open()) {
        console->report(LogLevel::error, "%s\n", console->getLastError().c_str());
        return;
    }

    for (uint8_t binary : u8Data) ouputFile << static_cast<unsigned char>(binary);

    ouputFile.close();

    console->log(LogLevel::lightcyan, "\n\n[%s]\n", outputFileName.c_str());

    if (hasHeaders) console->log("architecture: %s\n", (pNtHeaders->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64) ? "64-bit" : "32-bit");

    console->log("size: %d kb\n\n", static_cast<int>(u8Data.size()) / 1000);

    console->report(LogLevel::success, "finished constructing the file\n\n");
}
