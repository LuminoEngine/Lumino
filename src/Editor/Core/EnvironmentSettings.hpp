#pragma once
#include "Common.hpp"

namespace lna {
class Workspace;

// プロジェクトテンプレートなどの検索パスの基準
//enum class EnvironmentPathBase
//{
//	// 環境変数 LUMINO_PATH を基準とする
//	//EnvironmentVariable,
//
//	// 実行ファイルからフォルダをさかのぼり、パッケージのルートを検索する (インストールせずに使用する)
//	LocalPackage,
//
//	// 実行ファイルからフォルダをさかのぼり、リポジトリのルートを検索する (主にツールのデバッグで使用する)
//	Repository,
//};

class BuildEnvironment : public ln::RefObject {
public:
    BuildEnvironment();
    void setupPathes(bool developMode);
    ln::Result<> prepareEmscriptenSdk();

    const ln::String& defaultTargetName() const { return m_defaultTargetName; }

    //Language defaultLanguage() const { return m_defaultLanguage; }
    //EnvironmentPathBase actualPathBase() const { return m_actualPathBase; }

    // Pathes
    ln::Path projectTemplatesDirPath() const { return m_projectTemplatesDirPath; }
    const ln::Path& luminoPackageRootDir() const { return m_luminoPackageRootDir; }
    const ln::Path& luminoPackageToolsDir() const { return m_luminoPackageToolsDir; }
    const ln::Path& luminoPackageEngineDir() const { return m_luminoPackageEngineDir; }

    // CMake
    const ln::Path& cmakePackagesDir() const { return m_cmakePackagesDir; }
    const ln::Path& vcpkgDir() const { return m_vcpkgDir; }
    const ln::Path& ninja() const { return m_ninja; }

    // Emscripten
    const ln::Path& luminoEmscriptenSdkDirPath() const { return m_luminoEmscriptenSdkDirPath; }
    const ln::String& msdkVer() const { return m_emsdkVer; }
    const ln::String& emsdkName() const { return m_emsdkName; }
    const ln::Path& emsdkDirPath() const { return m_emsdkRootDir; }
    const ln::Path& emscriptenDirPath() const { return m_emscriptenRootDir; }
    //const ln::Path& emscriptenSysRootLocalDir() const { return m_emscriptenSysRootLocal; }
    const ln::Path& python() const { return m_python; }

    // Visual Studio
    const ln::Path& msbuild() const { return m_msbuild; }

    // Android
    const ln::Path& androidSdkRootDir() const { return m_androidSdkRootDir; }
    const ln::Path& androidSdkCMake() const { return m_androidSdkCMake; }
    const ln::Path& androidSdkNinja() const { return m_androidSdkNinja; }
    const ln::Path& androidNdkRootDir() const { return m_androidNdkRootDir; }
    const ln::Path& androidCMakeToolchain() const { return m_androidCMakeToolchain; }

    bool engineDevelopmentMode() const { return !m_engineDevelopmentRepoRootDir.isEmpty(); }
    const ln::Path& engineDevelopmentRepoRootDir() const { return m_engineDevelopmentRepoRootDir; }

    static ln::Path findLocalPackageForTesting();

private:
    //void setupPathesFromPackageRoot(const ln::Path& packageRoot);
    //void setupPathesFromRepositoryRoot(const ln::Path& repoRoot);
    static ln::Result<> callProcess(const ln::String& program, const ln::Array<ln::String>& arguments, const ln::Path& workingDir);
    ln::Path findNativePackageRootDir() const;
    static ln::Path findRepositoryRootDir();

    ln::String m_defaultTargetName;
    Language m_defaultLanguage;

    ln::Path m_appDataDirPath;
    ln::Path m_luminoPackageRootDir;
    ln::Path m_luminoPackageToolsDir;
    ln::Path m_luminoPackageEngineDir;

    ln::Path m_cmakePackagesDir;
    ln::Path m_vcpkgDir;
    ln::Path m_ninja;

    ln::Path m_luminoEmscriptenSdkDirPath;
    ln::Path m_projectTemplatesDirPath;
    ln::Path m_toolsDir;

    ln::String m_emsdkVer;
    ln::String m_emsdkName;
    ln::Path m_emsdkRootDir;
    ln::Path m_emscriptenRootDir;
    ln::Path m_emscriptenSysRoot;
    ln::Path m_python;

    ln::Path m_msbuild;

    ln::Path m_androidSdkRootDir;
    ln::Path m_androidSdkCMake;
    ln::Path m_androidSdkNinja;
    ln::Path m_androidNdkRootDir;
    ln::Path m_androidCMakeToolchain;

    ln::Path m_engineDevelopmentRepoRootDir;
    //bool m_engineDevelopmentMode;
};

class ScopedCurrentDir final {
public:
    static ScopedCurrentDir enter(const ln::Path& path) {
        return ScopedCurrentDir(path);
    }

    ~ScopedCurrentDir() {
        ln::Environment::setCurrentDirectory(m_prev);
    }

private:
    ScopedCurrentDir(const ln::Path& path)
        : m_prev(ln::Environment::currentDirectory()) {
        ln::Environment::setCurrentDirectory(path);
    }

    ln::Path m_prev;
};

} // namespace lna
