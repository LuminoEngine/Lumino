
#include "Workspace.hpp"
#include "EnvironmentSettings.hpp"
#include "../../Engine/src/Engine/EngineDomain.hpp"
#include "../../Engine/src/Engine/EngineManager.hpp"

namespace lna {


//==============================================================================
// BuildEnvironment

BuildEnvironment::BuildEnvironment(Workspace* workspace)
	: m_workspace(workspace)
	, m_toolsDir()
	, m_emsdkVer()
	, m_emsdkRootDir()
	, m_emscriptenRootDir()
	, m_engineDevelopmentRepoRootDir()
{
#if defined(LN_OS_WIN32)
	m_defaultTargetName = u"Windows";
#elif defined(LN_OS_MAC)
	m_defaultTargetName = u"macOS";
#else
	static_assert("Target not supported.");
#endif

	//m_defaultLanguage = Language::Cpp;
	//if (ln::Environment::executablePath().endsWith(u"-rb")) {
	//	m_defaultLanguage = Language::Ruby;
	//}
}

void BuildEnvironment::setupPathes(bool developMode)
{
	// Make package paths.
	{
		if (developMode) {
			// Visual Studio など IDE 上からデバッグする用。
			// 先に build.csproj を実行して NativePackage を作っておく必要がある。
			m_engineDevelopmentRepoRootDir = findRepositoryRootDir();
			if (!m_engineDevelopmentRepoRootDir.isEmpty()) {
				m_luminoPackageRootDir = ln::Path::combine(m_engineDevelopmentRepoRootDir, u"build", u"NativePackage");

				m_projectTemplatesDirPath = ln::Path::combine(m_engineDevelopmentRepoRootDir, u"Tools", u"ProjectTemplates");
			}
		}

		// 実行ファイルからディレクトリを親へ辿って、Package の Root を見つける。
		if (m_luminoPackageRootDir.isEmpty()) {
			m_luminoPackageRootDir = findNativePackageRootDir();
		}


		if (m_projectTemplatesDirPath.isEmpty()) {
			m_projectTemplatesDirPath = ln::Path::combine(m_luminoPackageRootDir, u"Tools", u"ProjectTemplates");
		}

		m_luminoPackageEngineDir = ln::Path::combine(m_luminoPackageRootDir, u"Engine");
		
		// Validation
		{
			if (m_luminoPackageRootDir.isEmpty()) {
				CLI::fatal(u"Not found lumino package root directory.");
				return;
			}
			if (!ln::FileSystem::existsDirectory(m_projectTemplatesDirPath)) {
				CLI::fatal(u"Not found 'ProjectTemplatesDir' directory.");
				return;
			}
		}

		CLI::verbose(ln::String::format(u"PackageRootDir: {0}", m_luminoPackageRootDir));
		CLI::verbose(ln::String::format(u"ProjectTemplatesDir: {0}", m_projectTemplatesDirPath));
	}


#if 0
    // Find package root
    {
        m_luminoPackageRootDir.clear();

		switch (pathBase)
		{
			//case EnvironmentPathBase::EnvironmentVariable:
			//{
			//	auto path = ln::Environment::getEnvironmentVariable(u"LUMINO_PATH");
			//	if (path) {
			//		m_luminoPackageRootDir = *path;
			//		setupPathesFromPackageRoot(m_luminoPackageRootDir);
			//	}
			//	m_actualPathBase = EnvironmentPathBase::EnvironmentVariable;
			//	break;
			//}
			case EnvironmentPathBase::LocalPackage:
			{
				//m_luminoPackageRootDir = findNativePackageRootDir();

#ifdef  LN_DEBUG
				if (m_luminoPackageRootDir.isEmpty()) {
					m_luminoPackageRootDir = findLocalPackageForTesting();
				}
#endif
				setupPathesFromPackageRoot(m_luminoPackageRootDir);
				m_actualPathBase = EnvironmentPathBase::LocalPackage;
				break;
			}
			case EnvironmentPathBase::Repository:
			{
				m_luminoPackageRootDir = ln::detail::EngineManager::findRepositoryRootForTesting();
				m_actualPathBase = EnvironmentPathBase::Repository;
				break;
			}
			default:
				LN_UNREACHABLE();
				break;
		}

        // ここまでで見つからなかったらエラー
        if (m_luminoPackageRootDir.isEmpty())
        {
            CLI::fatal(u"Not found lumino package root directory.");
			return;
        }

		setupPathesFromRepositoryRoot(m_luminoPackageRootDir);

		// Validation
		{
			if (!ln::FileSystem::existsDirectory(m_projectTemplatesDirPath)) {
				CLI::fatal(u"Not found 'projectTemplatesDirPath' directory.");
				return;
			}
		}

		CLI::verbose(ln::String::format(u"PackageRootDir: {0}", m_luminoPackageRootDir));
		CLI::verbose(ln::String::format(u"ProjectTemplatesDir: {0}", m_projectTemplatesDirPath));
    }

#endif


	m_appDataDirPath = ln::Path::combine(ln::Environment::specialFolderPath(ln::SpecialFolder::ApplicationData), u"Lumino");
    m_toolsDir = (ln::Path::combine(m_appDataDirPath, u"BuildTools"));


    // Emscripten
    {
        m_emsdkVer = u"1.39.8";
        m_emsdkName = u"1.39.8";
    }

	// Android
	{
#ifdef _WIN32
		PWSTR path = NULL;
		HRESULT hr = ::SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &path);
		if (SUCCEEDED(hr))
		{
			m_androidSdkRootDir = ln::Path::combine(ln::String::fromCString(path), u"Android", u"Sdk");
			m_androidSdkCMake = ln::Path::combine(m_androidSdkRootDir, u"cmake/3.6.4111459/bin/cmake.exe");
			m_androidSdkNinja = ln::Path::combine(m_androidSdkRootDir, u"cmake/3.6.4111459/bin/ninja.exe");
			m_androidNdkRootDir = ln::Path::combine(m_androidSdkRootDir, u"ndk-bundle");
			m_androidCMakeToolchain = ln::Path::combine(m_androidNdkRootDir, u"build/cmake/android.toolchain.cmake");

			CoTaskMemFree(path);
		}
#endif
	}

#if 0	// Find msbuild
	{
#ifdef LN_OS_WIN32
		ln::String result;
		if (ln::Process::execute(ln::Path(luminoPackageToolsDir(), u"vswhere"), {u"-format", u"json"}, &result) != 0) {
			CLI::error("Failed vswhere.");
			return;
		}

		ln::JsonDocument json;
		json.parse(result);
		auto* obj = static_cast<ln::JsonObject*>(static_cast<ln::JsonArray*>(json.rootElement())->element(0));
		auto* value = static_cast<ln::JsonValue*>(obj->find(u"installationPath"));

		auto path = ln::Path(value->stringValue(), u"MSBuild\\15.0\\Bin\\MSBuild.exe");
		if (ln::FileSystem::existsFile(path)) {
			m_msbuild = path;
		}
		else {
			CLI::warning("MSBuild not found.");
		}
#endif
	}
#endif
}

ln::Result BuildEnvironment::prepareEmscriptenSdk()
{

	// Emscripten
	{
		if (engineDevelopmentMode()) {
			m_emsdkRootDir = ln::Path::combine(m_engineDevelopmentRepoRootDir, u"build", u"Emscripten", u"emsdk");
		}

		if (m_emsdkRootDir.isEmpty()) {
			m_emsdkRootDir = ln::Path::combine(m_toolsDir, u"emsdk");
		}

		ln::FileSystem::createDirectory(m_emsdkRootDir);

		m_emscriptenRootDir = ln::Path::combine(m_emsdkRootDir, u"upstream", u"emscripten");
		m_emscriptenSysRootLocal = ln::Path::combine(m_emscriptenRootDir, u"system", u"local");
		m_python = ln::Path::combine(m_emsdkRootDir, u"python", u"3.7.4_64bit", u"python");

		if (!ln::FileSystem::existsDirectory(m_emsdkRootDir))
		{
			if (!callProcess(u"git", { u"clone", u"https://github.com/juj/emsdk.git" }, m_toolsDir)) {
				return false;
			}
		}

		if (!ln::FileSystem::existsDirectory(m_emscriptenRootDir))
		{
			CLI::info(ln::String::format(u"Setup emscripten : {0}", m_emsdkName));

#ifdef LN_OS_WIN32
			auto emsdk = ln::Path(m_emsdkRootDir, u"emsdk.bat"); // TODO: process クラス内で path 結合
#else
			auto emsdk = u"emsdk";
#endif
			if (!callProcess(u"git", { u"pull" }, m_emsdkRootDir)) {
				return false;
			}
			if (!callProcess(emsdk, { u"update-tags" }, m_emsdkRootDir)) {
				return false;
			}
			if (!callProcess(emsdk, { u"install", m_emsdkName }, m_emsdkRootDir)) {
				return false;
			}
		}

		CLI::verbose(ln::String::format(u"EmsdkRootDir: {0}", m_emsdkRootDir));
		CLI::verbose(ln::String::format(u"EmscriptenRootDir: {0}", m_emscriptenRootDir));
		CLI::verbose(ln::String::format(u"EmscriptenSysLocal: {0}", m_emscriptenSysRootLocal));

		if (!ln::FileSystem::existsDirectory(m_emscriptenSysRootLocal)) {
			CLI::fatal(u"Not found 'EmscriptenSysLocal' directory.");
			return false;
		}
	}

	{
		auto engineRoot = ln::Path::combine(m_emscriptenSysRootLocal, u"LuminoEngine");
		if (!ln::FileSystem::existsDirectory(engineRoot)) {
			auto src = ln::Path::combine(m_luminoPackageEngineDir, u"Emscripten");
			CLI::info(ln::String::format(u"Copy Engine '{0}' to '{1}'", src, engineRoot));
			ln::FileSystem::copyDirectory(src, engineRoot, true, true);
		}
	}

    return true;
}

ln::Path BuildEnvironment::findLocalPackageForTesting()
{
	ln::Path result;

	// デバッグ用。実行ファイルの位置からさかのぼっていって、build.csproj が見つかればそこから必要なパスを作ってみる
	{
		CLI::info("Using debug mode build environment pathes.");

		auto luminoRepoRoot = ln::detail::EngineManager::findRepositoryRootForTesting();
		LN_LOG_DEBUG << luminoRepoRoot;

		result = ln::Path::combine(luminoRepoRoot, u"build", u"NativePackage");
	}

	return result;
}

//void BuildEnvironment::setupPathesFromPackageRoot(const ln::Path& packageRoot)
//{
//	// Setup Basic paths
//	{
//		m_luminoPackageToolsDir = ln::Path(m_luminoPackageRootDir, u"Tools");
//		m_projectTemplatesDirPath = ln::Path(m_luminoPackageToolsDir, u"Templates");
//
//		CLI::info(u"luminoPackageRootDir: " + m_luminoPackageRootDir);
//		CLI::info(u"luminoPackageToolsDir: " + m_luminoPackageToolsDir);
//		CLI::info(u"projectTemplatesDirPath: " + m_projectTemplatesDirPath);
//	}
//
//	// LUMINO_PATH
//	{
//		auto v = ln::Environment::getEnvironmentVariable(u"LUMINO_PATH");
//		if (v.hasValue()) {
//			CLI::info(ln::String::format(u"LUMINO_PATH: {0}", v.value()));
//		}
//		else {
//			CLI::warning(ln::String::format(u"LUMINO_PATH is unset. Use local package instead."));
//			ln::Environment::setEnvironmentVariable(u"LUMINO_PATH", m_luminoPackageRootDir);
//			CLI::info(ln::String::format(u"LUMINO_PATH: {0}", m_luminoPackageRootDir));
//		}
//	}
//}

//void BuildEnvironment::setupPathesFromRepositoryRoot(const ln::Path& repoRoot)
//{
//	m_projectTemplatesDirPath = ln::Path::combine(repoRoot, u"Tools", u"ProjectTemplates");
//}

ln::Result BuildEnvironment::callProcess(const ln::String& program, const ln::List<ln::String>& arguments, const ln::Path& workingDir)
{
	ln::Process proc1;
	proc1.setUseShellExecute(false);
	proc1.setProgram(program);
	proc1.setArguments(arguments);
	proc1.setWorkingDirectory(workingDir);
	proc1.start();
	proc1.wait();
	if (proc1.exitCode() == 0) {
		return true;
	}
	else {
		return false;
	}
}

ln::Path BuildEnvironment::findNativePackageRootDir() const
{
	// 実行ファイルを少しさかのぼってパッケージのルートらしいフォルダがあればそこを採用
	ln::Path packageRootDir = ln::Path(ln::Environment::executablePath()).parent().parent();

	std::cout << m_workspace->primaryLang();

	if (m_workspace->primaryLang() == u"cpp") {
		ln::Path engineDir = ln::Path(packageRootDir, u"Engine");
		ln::Path toolsDir = ln::Path(packageRootDir, u"Tools");
		if (ln::FileSystem::existsDirectory(engineDir) && ln::FileSystem::existsDirectory(toolsDir)) {
			return packageRootDir;
		}
	}
	if (m_workspace->primaryLang() == u"ruby") {
		printf("rb!!\n");
		ln::Path toolsDir = ln::Path(packageRootDir, u"Tools");
		if (ln::FileSystem::existsDirectory(toolsDir)) {
			printf("ok!!\n");
			return packageRootDir;
		}
	}

	return ln::Path();
}

ln::Path BuildEnvironment::findRepositoryRootDir()
{
	return ln::detail::EngineManager::findRepositoryRootForTesting();
}

} // namespace lna
