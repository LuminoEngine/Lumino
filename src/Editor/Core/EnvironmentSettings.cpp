
#include "Workspace.hpp"
#include "EnvironmentSettings.hpp"
#include "../../../lumino/LuminoEngine/src/Engine/EngineDomain.hpp"
#include "../../../lumino/LuminoEngine/src/Engine/EngineManager.hpp"

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
	m_defaultTargetName = _TT("Windows");
#elif defined(LN_OS_MAC)
	m_defaultTargetName = _TT("macOS");
#else
	static_assert("Target not supported.");
#endif

	//m_defaultLanguage = Language::Cpp;
	//if (ln::Environment::executablePath().endsWith(_TT("-rb")) {
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
				m_luminoPackageRootDir = ln::Path::combine(m_engineDevelopmentRepoRootDir, _TT("build"), _TT("NativePackage"));

				m_projectTemplatesDirPath = ln::Path::combine(m_engineDevelopmentRepoRootDir, _TT("Tools"), _TT("ProjectTemplates"));
			}
		}

		// 実行ファイルからディレクトリを親へ辿って、Package の Root を見つける。
		if (m_luminoPackageRootDir.isEmpty()) {
			m_luminoPackageRootDir = findNativePackageRootDir();
		}


		if (m_projectTemplatesDirPath.isEmpty()) {
			m_projectTemplatesDirPath = ln::Path::combine(m_luminoPackageRootDir, _TT("Tools"), _TT("ProjectTemplates"));
		}

		m_luminoPackageEngineDir = ln::Path::combine(m_luminoPackageRootDir, _TT("Engine"));
		
		// Validation
		{
			if (m_luminoPackageRootDir.isEmpty()) {
				CLI::fatal(_TT("Not found lumino package root directory."));
				return;
			}
			if (!ln::FileSystem::existsDirectory(m_projectTemplatesDirPath)) {
				CLI::fatal(_TT("Not found 'ProjectTemplatesDir' directory."));
				return;
			}
		}

		CLI::verbose(ln::String::format(_TT("PackageRootDir: {0}"), m_luminoPackageRootDir.str()));
		CLI::verbose(ln::String::format(_TT("ProjectTemplatesDir: {0}"), m_projectTemplatesDirPath.str()));
	}


#if 0
    // Find package root
    {
        m_luminoPackageRootDir.clear();

		switch (pathBase)
		{
			//case EnvironmentPathBase::EnvironmentVariable:
			//{
			//	auto path = ln::Environment::getEnvironmentVariable(_TT("LUMINO_PATH");
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
            CLI::fatal(_TT("Not found lumino package root directory.");
			return;
        }

		setupPathesFromRepositoryRoot(m_luminoPackageRootDir);

		// Validation
		{
			if (!ln::FileSystem::existsDirectory(m_projectTemplatesDirPath)) {
				CLI::fatal(_TT("Not found 'projectTemplatesDirPath' directory.");
				return;
			}
		}

		CLI::verbose(ln::String::format(_TT("PackageRootDir: {0}", m_luminoPackageRootDir));
		CLI::verbose(ln::String::format(_TT("ProjectTemplatesDir: {0}", m_projectTemplatesDirPath));
    }

#endif


	m_appDataDirPath = ln::Path::combine(ln::Environment::specialFolderPath(ln::SpecialFolder::ApplicationData), _TT("Lumino"));
    m_toolsDir = (ln::Path::combine(m_appDataDirPath, _TT("BuildTools")));


    // Emscripten
    {
        m_emsdkVer = _TT("1.39.8");
        m_emsdkName = _TT("1.39.8");
    }

	// Android
	{
#ifdef _WIN32
		PWSTR path = NULL;
		HRESULT hr = ::SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &path);
		if (SUCCEEDED(hr))
		{
			m_androidSdkRootDir = ln::Path::combine(ln::String::fromCString(path), _TT("Android"), _TT("Sdk"));
			m_androidSdkCMake = ln::Path::combine(m_androidSdkRootDir, _TT("cmake/3.6.4111459/bin/cmake.exe"));
			m_androidSdkNinja = ln::Path::combine(m_androidSdkRootDir, _TT("cmake/3.6.4111459/bin/ninja.exe"));
			m_androidNdkRootDir = ln::Path::combine(m_androidSdkRootDir, _TT("ndk-bundle"));
			m_androidCMakeToolchain = ln::Path::combine(m_androidNdkRootDir, _TT("build/cmake/android.toolchain.cmake"));

			CoTaskMemFree(path);
		}
#endif
	}

#if 0	// Find msbuild
	{
#ifdef LN_OS_WIN32
		ln::String result;
		if (ln::Process::execute(ln::Path(luminoPackageToolsDir(), _TT("vswhere"), {_TT("-format", _TT("json"}, &result) != 0) {
			CLI::error("Failed vswhere.");
			return;
		}

		ln::JsonDocument json;
		json.parse(result);
		auto* obj = static_cast<ln::JsonObject*>(static_cast<ln::JsonArray*>(json.rootElement())->element(0));
		auto* value = static_cast<ln::JsonValue*>(obj->find(_TT("installationPath"));

		auto path = ln::Path(value->stringValue(), _TT("MSBuild\\15.0\\Bin\\MSBuild.exe");
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
			m_emsdkRootDir = ln::Path::combine(m_engineDevelopmentRepoRootDir, _TT("build"), _TT("Emscripten"), _TT("emsdk"));
		}

		if (m_emsdkRootDir.isEmpty()) {
			m_emsdkRootDir = ln::Path::combine(m_toolsDir, _TT("emsdk"));
		}

		ln::FileSystem::createDirectory(m_emsdkRootDir);

		m_emscriptenRootDir = ln::Path::combine(m_emsdkRootDir, _TT("upstream"), _TT("emscripten"));
		m_emscriptenSysRootLocal = ln::Path::combine(m_emscriptenRootDir, _TT("system"), _TT("local"));
		m_python = ln::Path::combine(m_emsdkRootDir, _TT("python"), _TT("3.7.4_64bit"), _TT("python"));

		if (!ln::FileSystem::existsDirectory(m_emsdkRootDir))
		{
			if (!callProcess(_TT("git"), { _TT("clone"), _TT("https://github.com/juj/emsdk.git") }, m_toolsDir)) {
				return false;
			}
		}

		if (!ln::FileSystem::existsDirectory(m_emscriptenRootDir))
		{
			CLI::info(ln::String::format(_TT("Setup emscripten : {0}"), m_emsdkName));

#ifdef LN_OS_WIN32
			auto emsdk = ln::Path(m_emsdkRootDir, _TT("emsdk.bat")); // TODO: process クラス内で path 結合
#else
			auto emsdk = _TT("emsdk");
#endif
			if (!callProcess(_TT("git"), { _TT("pull") }, m_emsdkRootDir)) {
				return false;
			}
			if (!callProcess(emsdk, { _TT("update-tags") }, m_emsdkRootDir)) {
				return false;
			}
			if (!callProcess(emsdk, { _TT("install"), m_emsdkName }, m_emsdkRootDir)) {
				return false;
			}
		}

		CLI::verbose(ln::String::format(_TT("EmsdkRootDir: {0}"), m_emsdkRootDir.str()));
		CLI::verbose(ln::String::format(_TT("EmscriptenRootDir: {0}"), m_emscriptenRootDir.str()));
		CLI::verbose(ln::String::format(_TT("EmscriptenSysLocal: {0}"), m_emscriptenSysRootLocal.str()));

		if (!ln::FileSystem::existsDirectory(m_emscriptenSysRootLocal)) {
			CLI::fatal(_TT("Not found 'EmscriptenSysLocal' directory."));
			return false;
		}
	}

	{
		auto engineRoot = ln::Path::combine(m_emscriptenSysRootLocal, _TT("LuminoEngine"));
		if (!ln::FileSystem::existsDirectory(engineRoot)) {
			auto src = ln::Path::combine(m_luminoPackageEngineDir, _TT("Emscripten"));
			CLI::info(ln::String::format(_TT("Copy Engine '{0}' to '{1}'"), src.str(), engineRoot.str()));
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
		CLI::info(_TT("Using debug mode build environment pathes."));

		auto luminoRepoRoot = ln::detail::EngineManager::findRepositoryRootForTesting();
		LN_LOG_DEBUG << luminoRepoRoot;

		result = ln::Path::combine(luminoRepoRoot, _TT("build"), _TT("NativePackage"));
	}

	return result;
}

//void BuildEnvironment::setupPathesFromPackageRoot(const ln::Path& packageRoot)
//{
//	// Setup Basic paths
//	{
//		m_luminoPackageToolsDir = ln::Path(m_luminoPackageRootDir, _TT("Tools");
//		m_projectTemplatesDirPath = ln::Path(m_luminoPackageToolsDir, _TT("Templates");
//
//		CLI::info(_TT("luminoPackageRootDir: " + m_luminoPackageRootDir);
//		CLI::info(_TT("luminoPackageToolsDir: " + m_luminoPackageToolsDir);
//		CLI::info(_TT("projectTemplatesDirPath: " + m_projectTemplatesDirPath);
//	}
//
//	// LUMINO_PATH
//	{
//		auto v = ln::Environment::getEnvironmentVariable(_TT("LUMINO_PATH");
//		if (v.hasValue()) {
//			CLI::info(ln::String::format(_TT("LUMINO_PATH: {0}", v.value()));
//		}
//		else {
//			CLI::warning(ln::String::format(_TT("LUMINO_PATH is unset. Use local package instead."));
//			ln::Environment::setEnvironmentVariable(_TT("LUMINO_PATH", m_luminoPackageRootDir);
//			CLI::info(ln::String::format(_TT("LUMINO_PATH: {0}", m_luminoPackageRootDir));
//		}
//	}
//}

//void BuildEnvironment::setupPathesFromRepositoryRoot(const ln::Path& repoRoot)
//{
//	m_projectTemplatesDirPath = ln::Path::combine(repoRoot, _TT("Tools", _TT("ProjectTemplates");
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

	if (m_workspace->primaryLang() == _TT("cpp")) {
		ln::Path engineDir = ln::Path(packageRootDir, _TT("Engine"));
		ln::Path toolsDir = ln::Path(packageRootDir, _TT("Tools"));
		if (ln::FileSystem::existsDirectory(engineDir) && ln::FileSystem::existsDirectory(toolsDir)) {
			return packageRootDir;
		}
	}
	if (m_workspace->primaryLang() == _TT("ruby")) {
		printf("rb!!\n");
		ln::Path toolsDir = ln::Path(packageRootDir, _TT("Tools"));
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
