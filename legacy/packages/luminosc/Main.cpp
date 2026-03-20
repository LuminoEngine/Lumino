#include <LuminoEngine/Graphics/ShaderCompiler/ShaderCompiler.hpp>
#include <LuminoEngine/Graphics/ShaderCompiler/UnifiedShader2.hpp>
#include <LuminoEngine/Graphics/ShaderCompiler/UnifiedShaderSerializer.hpp>
using namespace ln;
namespace fs = std::filesystem;

class CompileShaderCommand final {
public:
    MaybeResult execute(const fs::path& inputFiles) {
        auto result = kokage::ShaderCompiler::create();
        if (!result) return LN_TO_ERROR(result);

        URef<kokage::ShaderCompiler> compiler = std::move(result).value();

        auto result2 = compiler->build(inputFiles);
        if (!result2) return LN_TO_ERROR(result2);

        kokage::UnifiedShader2* shader = compiler->shader();

        fs::path outputFilePath = inputFiles;
        outputFilePath.replace_extension(kokage::UnifiedShaderSerializer::FileExt);


        auto result3 = kokage::UnifiedShaderSerializer::saveToFile(shader, outputFilePath);
        if (!result3) return LN_TO_ERROR(result3);

        if (!binaryToHexArray(outputFilePath)) {
            std::cerr << "Error: Could not create file " << outputFilePath << std::endl;
            return LN_MAKE_ERROR();
        }

        return LN_MAKE_SUCCESS();
    }

    
    bool binaryToHexArray(const fs::path& inputFile) {
        std::ifstream inFile(inputFile, std::ios::binary);
        if (!inFile) {
            std::cerr << "Error: Could not open file " << inputFile << std::endl;
            return false;
        }

        // Read the binary file into a vector
        std::vector<unsigned char> binaryData(
            (std::istreambuf_iterator<char>(inFile)),
            std::istreambuf_iterator<char>());
        inFile.close();

        fs::path outputFile = inputFile;
        outputFile += ".inl";
        //outputFile.append(".inl");
        std::ofstream outFile(outputFile);
        if (!outFile) {
            std::cerr << "Error: Could not create file " << outputFile << std::endl;
            return false;
        }

        // Write the binary data as comma-separated hex values
        for (size_t i = 0; i < binaryData.size(); ++i) {
            outFile << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(binaryData[i]);
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
};

int main(int argc, char** argv) {
#if defined(LN_DEBUG) && defined(_WIN32)
    if (argc == 1) {
        ln::Logger::setLevel(ln::LogLevel::Verbose);

        const char* debugArgv[] = {
            "<program>",
            //"E:/Proj/Lumino/packages/LuminoEngine/shader/CopyScreen.slang",
            "C:/Proj/Lumino/packages/LuminoEngine/src/Rendering/Resource/Sprite.slang"
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
        //ln::EngineOptions options;
        //options.graphics.enabled = false;
        //ln::Engine::initialize(options);

        if (!command.execute(inputFile)) {
            exitCode = 1;
        }
    }
    catch (ln::Exception& e) {
        ln::String m = e.message();
        std::cout << m << std::endl;
        exitCode = 1;
    }
    //ln::Engine::terminate();
    return exitCode;
}
