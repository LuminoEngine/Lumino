
#include <EnvironmentSettings.hpp>
#include <Workspace.hpp>
#include <Project/Project.hpp>
#include <Project/LanguageContext.hpp>
#include "BuildAssetCommand.hpp"
#include "FxcCommand.hpp"

int BuildAssetCommand::execute() {
    const auto intermediateDir = ln::Path(inputDir.parent(), U".ln");
    const auto outputFile = ln::Path(outputDir, inputDir.fileName() + U".lna");
    if (!lna::LanguageContext::buildAssets(intermediateDir, inputDir, outputFile)) {
        return 1;
    }
    return 0;
}
