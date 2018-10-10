
#include "EnvironmentSettings.hpp"

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
}

void BuildEnvironment::setupPathes()
{
	m_appDataDirPath = ln::Path::combine(ln::Environment::specialFolderPath(ln::SpecialFolder::ApplicationData), u"Lumino");
	m_toolsDir = (ln::Path::combine(m_appDataDirPath, u"BuildTools"));
	m_emsdkVer = u"1.38.12";
	m_emsdkName = u"sdk-1.38.12-64bit";
	m_emsdkRootDir = (ln::Path::combine(m_toolsDir, u"emsdk"));
	m_emscriptenRootDir = (ln::Path::combine(m_emsdkRootDir, u"emscripten", m_emsdkVer));
	m_python2 = ln::Path::combine(m_emsdkRootDir, u"python", u"2.7.13.1_64bit", u"python-2.7.13.amd64", u"python");

#ifdef  LN_DEBUG
	// デバッグ用。実行ファイルの位置からさかのぼっていって、.git が見つかればそこから必要なパスを作ってみる
	ln::Path path = ln::Environment::executablePath();
	ln::Path luminoRepoRoot;
	while (!path.isRoot())
	{
		if (ln::FileSystem::existsDirectory(ln::Path(path, u".git"))) {
			luminoRepoRoot = path;
			break;
		}
		path = path.parent();
	}
	if (LN_ENSURE(!luminoRepoRoot.isEmpty())) return;
	CLI::info("Using debug mode build environment pathes.");

	LN_LOG_DEBUG << luminoRepoRoot;

	m_luminoEmscriptenSdkDirPath = ln::Path(luminoRepoRoot, u"build/CMakeInstallTemp/Emscripten");
	m_projectTemplatesDirPath = ln::Path(luminoRepoRoot, u"tools/LuminoCLI/Templates");

#if defined(LN_OS_WIN32)
	m_luminoPackageRootDir = ln::Path(luminoRepoRoot, u"ReleasePackage.Win32");
#elif defined(LN_OS_MAC)
	m_luminoPackageRootDir = ln::Path(luminoRepoRoot, u"ReleasePackage.macOS");
#endif
#else
	ln::Path executablePath = ln::Environment::executablePath();
	ln::Path toolsDir = executablePath.parent();
	m_luminoPackageRootDir = toolsDir.parent();
	m_projectTemplatesDirPath = ln::Path(toolsDir, u"Templates");

#endif

	m_luminoPackageToolsDir = ln::Path(m_luminoPackageRootDir, u"Tools");

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

	// Find msbuild
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
}

void BuildEnvironment::prepareEmscriptenSdk()
{
	ln::FileSystem::createDirectory(m_toolsDir);

	// Emscripten
	{
		if (!ln::FileSystem::existsDirectory(m_emsdkRootDir))
		{
			ln::Process proc1;
			proc1.setUseShellExecute(false);
			proc1.setProgram(u"git");
			proc1.setArguments({ u"clone", u"https://github.com/juj/emsdk.git" });
			proc1.setWorkingDirectory(m_toolsDir);
			proc1.start();
			proc1.wait();
		}

		if (!ln::FileSystem::existsDirectory(m_emscriptenRootDir))
		{
			ln::Process proc1;
			proc1.setUseShellExecute(false);
#ifdef LN_OS_WIN32
			proc1.setProgram(u"emsdk");
#else
			proc1.setProgram(u"emsdk");
#endif
			proc1.setArguments({ u"install", m_emsdkName });
			proc1.setWorkingDirectory(m_emsdkRootDir);
			proc1.start();
			proc1.wait();
		}

		//auto file = ln::Path::combine(m_emscriptenRootDir, u"system", u"include", u"LuminoEngine.hpp");
		//if (!ln::FileSystem::existsFile(file))
		//{
		//	// 必須ファイルが1つ無かったので、とりあえず全部インストールしなおす

		//	auto srcIncludeDir = ln::Path::combine(m_luminoEmscriptenSdkDirPath, u"include");
		//	auto dstIncludeDir = ln::Path::combine(m_emscriptenRootDir, u"system", u"include");
		//	ln::FileSystem::copyDirectory(srcIncludeDir, dstIncludeDir, true, true);

		//	auto srcLibDir = ln::Path::combine(m_luminoEmscriptenSdkDirPath, u"lib");
		//	auto dstLibDir = ln::Path::combine(m_emscriptenRootDir, u"system", u"lib");
		//	ln::FileSystem::copyDirectory(srcLibDir, dstLibDir, true, true);

		//}
		

	}

}

