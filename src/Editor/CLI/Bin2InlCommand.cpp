#include "Bin2InlCommand.hpp"

int Bin2InlCommand::execute(const ln::Path& inputFile) {
    ln::String actualOutput = (outputFile.isEmpty()) ? inputFile.str() + U".inl" : outputFile.str();
    
    auto data = ln::FileSystem::readAllBytes(inputFile).unwrap();
    auto writer = ln::StreamWriter::open(actualOutput).unwrap();

    for (int i = 0; i < data.size(); i++) {
        writer->write(ln::format(U"{0:#04x}, ", data[i]));
        if (i % 16 == 15) {
            writer->writeLine();
        }
    }

    return 0;
}

