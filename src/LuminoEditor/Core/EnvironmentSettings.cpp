
#include "EnvironmentSettings.hpp"
#include "../../LuminoEngine/src/Engine/EngineDomain.hpp"
#include "../../LuminoEngine/src/Engine/EngineManager.hpp"

namespace lna {


//==============================================================================
// BuildEnvironment

BuildEnvironment::BuildEnvironment()
	: m_toolsDir()
	, m_emsdkVer()
	, m_emsdkRootDir()
	, m_emscriptenRootDir()
{
#if defined(LN_OS_WIN32)
	m_defaultTargetName = u"Windows";
#elif defined(LN_OS_MAC)
	m_defaultTargetName = u"macOS";
#else
	static_assert("Target not supported.");
#endif

	m_defaultLanguage = Language::Cpp;
	if (ln::Environment::executablePath().endsWith(u"-rb")) {
		m_defaultLanguage = Language::Ruby;
	}
}

void BuildEnvironment::setupPathes(EnvironmentPathBase pathBase)
{
    // Find package root
    {
        m_luminoPackageRootDir.clear();

		switch (pathBase)
		{
			case EnvironmentPathBase::EnvironmentVariable:
			{
				auto path = ln::Environment::getEnvironmentVariable(u"LUMINO_PATH");
				if (path) {
					m_luminoPackageRootDir = *path;
					setupPathesFromPackageRoot(m_luminoPackageRootDir);
				}
				m_actualPathBase = EnvironmentPathBase::EnvironmentVariable;
				break;
			}
			case EnvironmentPathBase::LocalPackage:
			{
				// 実行ファイルを少しさかのぼってパッケージのルートらしいフォルダがあればそこを採用
				ln::Path packageRootDir = ln::Path(ln::Environment::executablePath()).parent().parent();
				ln::Path engineDir = ln::Path(packageRootDir, u"Engine");
				ln::Path toolsDir = ln::Path(packageRootDir, u"Tools");
				if (ln::FileSystem::existsDirectory(engineDir) && ln::FileSystem::existsDirectory(toolsDir))
				{
					m_luminoPackageRootDir = packageRootDir;
				}

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
				setupPathesFromRepositoryRoot(m_luminoPackageRootDir);
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

		// Validation
		{
			if (!ln::FileSystem::existsDirectory(m_projectTemplatesDirPath)) {
				CLI::fatal(u"Not found 'projectTemplatesDirPath' directory.");
				return;
			}
		}
    }





    // Emscripten
    {
        m_appDataDirPath = ln::Path::combine(ln::Environment::specialFolderPath(ln::SpecialFolder::ApplicationData), u"Lumino");
        m_toolsDir = (ln::Path::combine(m_appDataDirPath, u"BuildTools"));
        m_emsdkVer = u"1.38.12";
        m_emsdkName = u"sdk-1.38.12-64bit";
        m_emsdkRootDir = (ln::Path::combine(m_toolsDir, u"emsdk"));
        m_emscriptenRootDir = (ln::Path::combine(m_emsdkRootDir, u"emscripten", m_emsdkVer));
        m_python2 = ln::Path::combine(m_emsdkRootDir, u"python", u"2.7.13.1_64bit", u"python-2.7.13.amd64", u"python");
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
	ln::FileSystem::createDirectory(m_toolsDir);

	// Emscripten
	{
		if (!ln::FileSystem::existsDirectory(m_emsdkRootDir))
		{
			if (!callProcess(u"git", { u"clone", u"https://github.com/juj/emsdk.git" }, m_toolsDir)) {
				return false;
			}
		}

		if (!ln::FileSystem::existsDirectory(m_emscriptenRootDir))
		{
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

		result = ln::Path::combine(luminoRepoRoot, u"build", u"LocalPackage");
	}

	return result;
}

void BuildEnvironment::setupPathesFromPackageRoot(const ln::Path& packageRoot)
{
	// Setup Basic paths
	{
		m_luminoPackageToolsDir = ln::Path(m_luminoPackageRootDir, u"Tools");
		m_projectTemplatesDirPath = ln::Path(m_luminoPackageToolsDir, u"Templates");

		CLI::info(u"luminoPackageRootDir: " + m_luminoPackageRootDir);
		CLI::info(u"luminoPackageToolsDir: " + m_luminoPackageToolsDir);
		CLI::info(u"projectTemplatesDirPath: " + m_projectTemplatesDirPath);
	}

	// LUMINO_PATH
	{
		auto v = ln::Environment::getEnvironmentVariable(u"LUMINO_PATH");
		if (v.hasValue()) {
			CLI::info(ln::String::format(u"LUMINO_PATH: {0}", v.value()));
		}
		else {
			CLI::warning(ln::String::format(u"LUMINO_PATH is unset. Use local package instead."));
			ln::Environment::setEnvironmentVariable(u"LUMINO_PATH", m_luminoPackageRootDir);
			CLI::info(ln::String::format(u"LUMINO_PATH: {0}", m_luminoPackageRootDir));
		}
	}
}

void BuildEnvironment::setupPathesFromRepositoryRoot(const ln::Path& repoRoot)
{
	m_projectTemplatesDirPath = ln::Path::combine(repoRoot, u"tools", u"ProjectTemplates");
}

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


} // namespace lna
