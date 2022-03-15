#pragma once

class BuildAssetCommand {
public:
    ln::Path inputDir;
    ln::Path outputDir;

    int execute();

private:
};
