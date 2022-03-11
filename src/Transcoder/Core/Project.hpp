#pragma once

class Project;
class Module;
class InputFile;

template<class T>
using URef = std::unique_ptr<T>;

template<class T, class... TArgs>
inline URef<T> makeURef(TArgs&&... args) {
    return std::unique_ptr<T>(LN_NEW T(std::forward<TArgs>(args)...));
}

class Project {
public:
    ln::Array<URef<Module>> modules;
};

class Module {
public:
    ln::Array<ln::String> includeDirectories;
    ln::Array<ln::String> forceIncludeFiles;
    ln::Array<URef<InputFile>> inputFiles;
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

class InputFile {
public:
    InputFile(const ln::Path& filePath);

    const ln::Path& filePath() const { return m_filePath; }

private:
    ln::Path m_filePath;
};
