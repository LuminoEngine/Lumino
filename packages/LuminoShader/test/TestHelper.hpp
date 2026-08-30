// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once

#include <fstream>
#include <gtest/gtest.h>
#include <LuminoShader/ShaderCompiler2.hpp>
#include <LuminoShader/UnifiedShader2.hpp>
#include <LuminoShader/UnifiedShaderSerializer2.hpp>

inline std::unique_ptr<ln::shader::ShaderCompiler2> compileParameterBlock1() {
    auto compiler = *ln::shader::ShaderCompiler2::create();
    auto inputFilePath = std::filesystem::path(TEST_DATA_DIR) / "ParameterBlock1.slang";
    auto buildResult = compiler->build(inputFilePath);
    EXPECT_TRUE(buildResult.has_value()) << buildResult.error().message;
    return compiler;
}

// .lcsh に保存し、そのバイト列を読み戻す。一時ファイルは削除する。
inline std::vector<uint8_t> saveAndRead(const ln::shader::UnifiedShader2* shader, const char* fileName) {
    auto tempPath = std::filesystem::temp_directory_path() / fileName;
    auto saveResult = ln::shader::UnifiedShaderSerializer2::saveToFile(shader, tempPath);
    EXPECT_TRUE(saveResult.has_value()) << saveResult.error().message;

    std::vector<uint8_t> fileData;
    std::ifstream ifs(tempPath, std::ios::binary | std::ios::ate);
    EXPECT_TRUE(ifs.is_open());
    size_t fileSize = ifs.tellg();
    fileData.resize(fileSize);
    ifs.seekg(0);
    ifs.read(reinterpret_cast<char*>(fileData.data()), fileSize);
    ifs.close();
    std::filesystem::remove(tempPath);
    return fileData;
}
