#include "Project.hpp"

//==============================================================================
// Module

Module::Module(const ln::String& name, const ln::Path& moduleRoot)
    : m_name(name)
    , m_moduleRoot(moduleRoot) {
}

void Module::addInputFile(const ln::Path& filePath) {
    inputFiles.push(makeURef<InputFile>(ln::Path(m_moduleRoot, filePath)));
}

//==============================================================================
// InputFile

InputFile::InputFile(const ln::Path& filePath)
    : m_filePath(filePath) {
}
