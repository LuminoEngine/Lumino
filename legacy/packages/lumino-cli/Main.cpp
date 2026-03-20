#include "FxcCommand.hpp"

static int processCommands(int argc, char** argv) {

    ln::CommandLineParser parser;
    parser.addHelpOption();

    // auto langOption = parser.addNamedValueOption(_TT("l", _TT("lang", _TT("language.", { _TT("cpp", _TT("rb" });
    auto devOption = parser.addFlagOption(_TT("d"), _TT("dev"), _TT("Development mode."));

    //--------------------------------------------------------------------------------
    // fxc command
    auto fxcCommand = parser.addCommand(
        _TT("fxc"),
        _TT("Compile shader."));
    auto fxcCommand_inputArg = fxcCommand->addPositionalArgument(
        _TT("input"),
        _TT("Input file."));
    auto fxcCommand_outputArg = fxcCommand->addPositionalArgument(
        _TT("output"),
        _TT("Output file."),
        ln::CommandLinePositionalArgumentFlags::Optional);
    auto fxcCommand_exportArg = fxcCommand->addValueOption(
        _TT("e"),
        _TT("export"),
        _TT("Export folder of output code."));


    if (parser.process(argc, argv)) {
        //--------------------------------------------------------------------------------
        // fxc command
        if (parser.has(fxcCommand)) {
            FxcCommand cmd;
            if (fxcCommand_outputArg->hasValue()) {
                cmd.outputFile = fxcCommand_outputArg->value();
            }
            if (fxcCommand_exportArg->hasValue()) {
                cmd.exportDir = fxcCommand_exportArg->value();
            }
            return cmd.execute(fxcCommand_inputArg->value());
        }
        else {
            parser.printHelp();
        }
    }
    else {
        parser.printHelp();
    }

    return 1;
}


int main(int argc, char** argv) {
#if defined(LN_DEBUG) && defined(_WIN32)
    if (argc == 1) {
        ln::Logger::setLevel(ln::LogLevel::Verbose);

        const char* debugArgv[] = {
            "<program>",
            "fxc",
            "E:/Proj/Lumino/packages/LuminoFFI/test/Assets/Test_Shader.Basic1/Test_Shader.Basic1.fx",

        };
        argc = sizeof(debugArgv) / sizeof(char*);
        argv = (char**)debugArgv;
    }
#endif

    int exitCode = 0;
    try {
        setlocale(LC_ALL, "");
        ln::Logger::addStdErrAdapter();
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
