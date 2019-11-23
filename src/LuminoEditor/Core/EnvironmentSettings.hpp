#pragma once

namespace lna {

// プロジェクトテンプレートなどの検索パスの基準
enum class EnvironmentPathBase
{
	// 環境変数 LUMINO_PATH を基準とする
	EnvironmentVariable,

	// 実行ファイルからフォルダをさかのぼり、パッケージのルートを検索する (インストールせずに使用する)
	LocalPackage,

	// 実行ファイルからフォルダをさかのぼり、リポジトリのルートを検索する (主にツールのデバッグで使用する)
	Repository,
};

class BuildEnvironment
	: public ln::RefObject
{
public:
	BuildEnvironment();
	void setupPathes(EnvironmentPathBase pathBase);
	ln::Result prepareEmscriptenSdk();

	const ln::String& defaultTargetName() const { return m_defaultTargetName; }

	const ln::Path& luminoEmscriptenSdkDirPath() const { return m_luminoEmscriptenSdkDirPath; }
	
	ln::Path projectTemplatesDirPath() const { return m_projectTemplatesDirPath; }

	const ln::Path& luminoPackageRootDir() const { return m_luminoPackageRootDir; }
	const ln::Path& luminoPackageToolsDir() const { return m_luminoPackageToolsDir; }
	
	// Emscripten
	const ln::String& msdkVer() const { return m_emsdkVer; }
	const ln::String& emsdkName() const { return m_emsdkName; }
	const ln::Path& emsdkDirPath() const { return m_emsdkRootDir; }
	const ln::Path& emscriptenDirPath() const { return m_emscriptenRootDir; }
	const ln::Path& python2() const { return m_python2; }

	// Visual Studio
	const ln::Path& msbuild() const { return m_msbuild; }

	// Android
	const ln::Path& androidSdkRootDir() const { return m_androidSdkRootDir; }
	const ln::Path& androidSdkCMake() const { return m_androidSdkCMake; }
	const ln::Path& androidSdkNinja() const { return m_androidSdkNinja; }
	const ln::Path& androidNdkRootDir() const { return m_androidNdkRootDir; }
	const ln::Path& androidCMakeToolchain() const { return m_androidCMakeToolchain; }

	static ln::Path findLocalPackageForTesting();
	static ln::Path findRepositoryRootForTesting();

private:
	void setupPathesFromPackageRoot(const ln::Path& packageRoot);
	void setupPathesFromRepositoryRoot(const ln::Path& repoRoot);
	static ln::Result callProcess(const ln::String& program, const ln::List<ln::String>& arguments, const ln::Path& workingDir);

	ln::String m_defaultTargetName;
	ln::Path m_appDataDirPath;
	ln::Path m_luminoPackageRootDir;
	ln::Path m_luminoPackageToolsDir;
	ln::Path m_luminoEmscriptenSdkDirPath;
	ln::Path m_projectTemplatesDirPath;
	ln::Path m_toolsDir;

	ln::String m_emsdkVer;
	ln::String m_emsdkName;
	ln::Path m_emsdkRootDir;
	ln::Path m_emscriptenRootDir;
	ln::Path m_python2;

	ln::Path m_msbuild;

	ln::Path m_androidSdkRootDir;
	ln::Path m_androidSdkCMake;
	ln::Path m_androidSdkNinja;
	ln::Path m_androidNdkRootDir;
	ln::Path m_androidCMakeToolchain;
};

} // namespace lna
