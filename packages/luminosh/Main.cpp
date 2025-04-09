#include <LuminoEngine/Graphics/ShaderCompiler/ShaderCompiler.hpp>
#include <LuminoEngine/Graphics/ShaderCompiler/UnifiedShader2.hpp>
#include <LuminoEngine/Graphics/ShaderCompiler/UnifiedShaderSerializer.hpp>
using namespace ln;
namespace fs = std::filesystem;

class CompileShaderCommand final {
public:
    MaybeResult execute(const fs::path& inputFiles) {
        //ln::EngineInstance* instance = ln::EngineInstance::instance();

        auto result = kokage::ShaderCompiler::create();
        if (!result) return LN_TO_ERROR(result);

        URef<kokage::ShaderCompiler> compiler = std::move(result).value();

        auto result2 = compiler->build(inputFiles);
        //auto result2 = compiler->build("C:/Proj/LN/Lumino/packages/LuminoEngine/shader/CopyScreen.slang");
        if (!result2) return LN_TO_ERROR(result2);

        kokage::UnifiedShader2* shader = compiler->shader();

        fs::path outputFilePath = inputFiles;
        outputFilePath.replace_extension(kokage::UnifiedShaderSerializer::FileExt);


        auto result3 = kokage::UnifiedShaderSerializer::saveToFile(shader, outputFilePath);
        if (!result3) return LN_TO_ERROR(result3);

        return LN_MAKE_SUCCESS();
    }
};

int main(int argc, char** argv) {
#if defined(LN_DEBUG) && defined(_WIN32)
    if (argc == 1) {
        ln::Logger::setLevel(ln::LogLevel::Verbose);

        const char* debugArgv[] = {
            "<program>",
            "E:/Proj/Lumino/packages/LuminoEngine/shader/CopyScreen.slang",
        };
        argc = sizeof(debugArgv) / sizeof(char*);
        argv = (char**)debugArgv;
    }
#endif
    setlocale(LC_ALL, "");
    CLI::App app{ "luminosc" };

    //std::vector<std::filesystem::path> inputFiles;
    //app.add_option("files", inputFiles, "Input files.");

    CompileShaderCommand command;
    fs::path inputFile;
    app.add_option("file", inputFile, "Input file.");

    CLI11_PARSE(app, argc, argv);


    int exitCode = 0;
    try {
        ln::EngineOptions options;
        options.graphics.enabled = false;
        ln::Engine::initialize(options);

        if (!command.execute(inputFile)) {
            exitCode = 1;
        }
    }
    catch (ln::Exception& e) {
        ln::String m = e.message();
        std::cout << m << std::endl;
        exitCode = 1;
    }
    ln::Engine::terminate();
    return exitCode;
}
