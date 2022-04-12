#pragma once

class Bin2InlCommand {
public:
    ln::Path outputFile;

    int execute(const ln::Path& inputFile);
};
