#include <gtest/gtest.h>
#include <lumino_shader/UnifiedShader.hpp>
#include <lumino_shader/ShaderCompiler.hpp>

using namespace ln;
using namespace ln::shader;

TEST(Test_ParameterBlock, Basic1) {
    auto compiler = *ShaderCompiler::create();
    //if (!compilerResult) {
    //    std::cerr << "Error: " << compilerResult.error().message << std::endl;
    //    return 1;
    //}
    auto inputFilePath = std::filesystem::path(TEST_DATA_DIR) / "ParameterBlock1.slang";    

    auto buildResult = compiler->build(inputFilePath);
    if (!buildResult) {
        std::cerr << "Error: " << buildResult.error().message << std::endl;
    }

    UnifiedShader* shader = compiler->shader();

    printf("");
}
