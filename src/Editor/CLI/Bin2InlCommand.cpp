#include "Bin2InlCommand.hpp"

int Bin2InlCommand::execute(const ln::Path& inputFile) {
    ln::String actualOutput = (outputFile.isEmpty()) ? inputFile.str() + U".inl2" : outputFile.str();
    
    auto data = ln::FileSystem::readAllBytes(inputFile);
    ln::StreamWriter writer(actualOutput);

    for (int i = 0; i < data.size(); i++) {
        writer.write(ln::format(U"{0:#x}, ", data[i]));
        if (i % 16 == 15) {
            writer.writeLine();
        }
    }

    return 0;
}

