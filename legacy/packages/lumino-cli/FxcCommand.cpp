#include "BuildAssetHelper.hpp"
#include "FxcCommand.hpp"

int FxcCommand::execute(const ln::Path& inputFile) {
    auto actualOutputFile = outputFile;
    if (outputFile.isEmpty()) {
        actualOutputFile = inputFile.replaceExtension(U".lcfx");
    }
    auto result = ln::BuildAssetHelper::buildShader(inputFile, actualOutputFile, exportDir);
    return result ? 0 : 1;
}
