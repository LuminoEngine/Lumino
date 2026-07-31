// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <CLI/CLI.hpp>

#ifdef LUMINO_USE_SLANG
#include <LuminoShader/ShaderCompiler2.hpp>
#endif
#include <LuminoShader/UnifiedShader2.hpp>
#include <LuminoShader/UnifiedShaderSerializer2.hpp>

namespace fs = std::filesystem;
using namespace ln;
using namespace ln::shader;

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
    CLI::App app{"luminosc - Lumino Shader Compiler"};

    fs::path inputFile;
    app.add_option("file", inputFile, "Input .slang file.")->required();

    bool dumpEnabled = false;
    app.add_flag("--dump", dumpEnabled, "Dump generated shader code (SPIR-V, WGSL, etc.) to files.");

    std::vector<fs::path> searchPaths;
    app.add_option("-I,--search-path", searchPaths, "Additional search paths for import.");

    bool noValidateWgsl = false;
    app.add_flag("--no-validate-wgsl", noValidateWgsl,
        "Skip WGSL validation of the generated WebGPU shader code.");

    CLI11_PARSE(app, argc, argv);

#ifdef LUMINO_USE_SLANG
    auto compilerResult = ShaderCompiler2::create();
    if (!compilerResult) {
        std::cerr << "Error: " << compilerResult.error().message << std::endl;
        return 1;
    }

    auto& compiler = compilerResult.value();
    compiler->setDumpEnabled(dumpEnabled);
    compiler->setWgslValidationEnabled(!noValidateWgsl);
    for (const auto& sp : searchPaths) {
        compiler->addSearchPath(sp);
    }

    auto buildResult = compiler->build(inputFile);
    if (!buildResult) {
        std::cerr << "Error: " << buildResult.error().message << std::endl;
        return 1;
    }

    UnifiedShader2* shader = compiler->shader();

    fs::path outputFilePath = inputFile;
    outputFilePath.replace_extension(UnifiedShaderSerializer2::FileExt);

    auto saveResult = UnifiedShaderSerializer2::saveToFile(shader, outputFilePath);
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
