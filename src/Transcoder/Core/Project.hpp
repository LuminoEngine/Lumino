#pragma once

class Project;
class Module;
class InputFile;

class Project : public ln::URefObject {
public:
    ln::Array<ln::URef<Module>> modules;
};

class Module : public ln::URefObject {
public:
    ln::Array<ln::String> includeDirectories;
    ln::Array<ln::String> forceIncludeFiles;
    ln::Array<ln::URef<InputFile>> inputFiles;
    ln::String outputRegisterTypesFile;

    Module(const ln::String& name, const ln::Path& moduleRoot);
    const ln::String& name() const { return m_name; }
    const ln::Path& moduleRoot() const { return m_moduleRoot; }
    void addInputFile(const ln::Path& filePath);
    void addIncludeDirectory(const ln::Path& value) { includeDirectories.push(ln::Path(m_moduleRoot, value)); }
    void addForceIncludeFile(const ln::Path& value) { forceIncludeFiles.push(ln::Path(m_moduleRoot, value)); }
    void setOutputRegisterTypesFile(const ln::Path& value) { outputRegisterTypesFile = ln::Path(m_moduleRoot, value); }

private:
    ln::String m_name;
    ln::Path m_moduleRoot;
};

class InputFile : public ln::URefObject {
public:
    InputFile(const ln::Path& filePath);

    const ln::Path& filePath() const { return m_filePath; }

private:
    ln::Path m_filePath;
};
