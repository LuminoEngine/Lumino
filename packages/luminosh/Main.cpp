//#include "FxcCommand.hpp"

using namespace ln;

int processCommands(int argc, char** argv) {
    //ln::EngineInstance* instance = ln::EngineInstance::instance();

    auto result = kokage::ShaderCompiler::create();
    if (!result) return 1;

    URef<kokage::ShaderCompiler> compiler = std::move(result.unwrap());

    auto result2 = compiler->build("C:/Proj/LN/Lumino/packages/LuminoEngine/shader/CopyScreen.slang");
    if (!result2) return 1;

    return 0;
}


int main(int argc, char** argv) {
#if defined(LN_DEBUG) && defined(_WIN32)
    if (argc == 1) {
        ln::Logger::setLevel(ln::LogLevel::Verbose);

        const char* debugArgv[] = {
            "<program>",
            "E:/Proj/Lumino/packages/LuminoFFI/test/Assets/Test_Shader.Basic1/Test_Shader.Basic1.fx",

        };
        argc = sizeof(debugArgv) / sizeof(char*);
        argv = (char**)debugArgv;
    }
#endif

    int exitCode = 0;
    try {
        setlocale(LC_ALL, "");
        ln::EngineOptions options;
        options.graphics.enabled = false;
        ln::Engine::initialize(options);
        exitCode = processCommands(argc, argv);
    }
    catch (ln::Exception& e) {
        ln::String m = e.message();
        std::cout << m << std::endl;
        exitCode = 1;
    }
    ln::Engine::terminate();
    return exitCode;
}
