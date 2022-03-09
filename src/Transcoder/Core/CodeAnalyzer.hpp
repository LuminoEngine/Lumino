
#pragma once
#include <unordered_map>
#include "Common.hpp"
#include "../Parser/ParserIntermediates.hpp"

class Project;
class Module;
class InputFile;

class CodeAnalyzer {
public:
    // env
    ln::Path parserExecutable;
    ln::Path luminoIncludePath;
    ln::Path forceInclude_LuminoCore;
    ln::Path forceInclude_LuminoEngine;

    // input params
    // const ln::List<ln::Path>* files_LuminoCore;
    // const ln::List<ln::Path>* files_LuminoEngine;
    ln::List<CompilationDatabase> inputs;

    CodeAnalyzer();

    void makeInputs(const Project* project);

    Result analyze(PIDatabase* pidb, ln::DiagnosticsManager* diag);

    const Ref<PIDatabase>& pidb() const { return m_pidb; }

private:
    void callParser(CompilationDatabase& cdb, int index);

    ln::DiagnosticsManager* m_diag;
    PIDatabase* m_pidb;
};

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

    Module(const ln::Path& moduleRoot);
    const ln::Path& moduleRoot() const { return m_moduleRoot; }
    void addInputFile(const ln::Path& filePath);
    void addIncludeDirectory(const ln::Path& value) { includeDirectories.push(ln::Path(m_moduleRoot, value)); }
    void addForceIncludeFile(const ln::Path& value) { forceIncludeFiles.push(ln::Path(m_moduleRoot, value)); }

private:
    ln::Path m_moduleRoot;
};

class InputFile {
public:
    InputFile(const ln::Path& filePath);

    const ln::Path& filePath() const { return m_filePath; }

private:
    ln::Path m_filePath;
};
