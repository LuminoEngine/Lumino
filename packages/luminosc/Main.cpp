// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#ifdef LUMINO_USE_SLANG
#include <lumino_shader/ShaderCompiler.hpp>
#endif
#include <lumino_shader/UnifiedShader.hpp>
#include <lumino_shader/UnifiedShaderSerializer.hpp>

namespace fs = std::filesystem;
using namespace lumino;
using namespace lumino::shader;

static bool binaryToHexArray(const fs::path& inputFile) {
    std::ifstream inFile(inputFile, std::ios::binary);
    if (!inFile) {
        std::cerr << "Error: Could not open file " << inputFile << std::endl;
        return false;
    }

    std::vector<unsigned char> binaryData(
        (std::istreambuf_iterator<char>(inFile)),
        std::istreambuf_iterator<char>());
    inFile.close();

    fs::path outputFile = inputFile;
    outputFile += ".inl";
    std::ofstream outFile(outputFile);
    if (!outFile) {
        std::cerr << "Error: Could not create file " << outputFile << std::endl;
        return false;
    }

    for (size_t i = 0; i < binaryData.size(); ++i) {
        outFile << "0x" << std::hex << std::setw(2) << std::setfill('0')
                << static_cast<int>(binaryData[i]);
        if (i < binaryData.size() - 1) {
            outFile << ", ";
        }
        if (i > 0 && (i % 16 == 15)) {
            outFile << "\n";
        }
    }

    outFile.close();
    std::cout << "Output written to " << outputFile << std::endl;
    return true;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: luminosc <input.slang>" << std::endl;
        return 1;
    }

    fs::path inputFile = argv[1];

#ifdef LUMINO_USE_SLANG
    auto compilerResult = ShaderCompiler::create();
    if (!compilerResult) {
        std::cerr << "Error: " << compilerResult.error().message << std::endl;
        return 1;
    }

    auto& compiler = compilerResult.value();

    auto buildResult = compiler->build(inputFile);
    if (!buildResult) {
        std::cerr << "Error: " << buildResult.error().message << std::endl;
        return 1;
    }

    UnifiedShader* shader = compiler->shader();

    fs::path outputFilePath = inputFile;
    outputFilePath.replace_extension(UnifiedShaderSerializer::FileExt);

    auto saveResult = UnifiedShaderSerializer::saveToFile(shader, outputFilePath);
    if (!saveResult) {
        std::cerr << "Error: " << saveResult.error().message << std::endl;
        return 1;
    }

    if (!binaryToHexArray(outputFilePath)) {
        return 1;
    }

    std::cout << "Compiled: " << outputFilePath << std::endl;
#else
    std::cerr << "Error: luminosc requires LUMINO_USE_SLANG" << std::endl;
    return 1;
#endif

    return 0;
}
