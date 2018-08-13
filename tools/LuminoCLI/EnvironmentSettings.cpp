
#include "EnvironmentSettings.hpp"

void EnvironmentSettings::updatePathes()
{
#ifdef _WIN32
	PWSTR path = NULL;
	HRESULT hr = ::SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &path);
	if (SUCCEEDED(hr))
	{
		wprintf(L"FOLDERID_Public: %s\n", path);

		m_androidSdkRootDir = ln::Path::combine(ln::String::fromCString(path), u"Android", u"Sdk");
		m_androidSdkCMake = ln::Path::combine(m_androidSdkRootDir, u"cmake/3.6.4111459/bin/cmake.exe");
		m_androidSdkNinja = ln::Path::combine(m_androidSdkRootDir, u"cmake/3.6.4111459/bin/ninja.exe");
		m_androidNdkRootDir = ln::Path::combine(m_androidSdkRootDir, u"ndk-bundle");
		m_androidCMakeToolchain = ln::Path::combine(m_androidNdkRootDir, u"build/cmake/android.toolchain.cmake");

		CoTaskMemFree(path);
	}
#endif
}

ln::Path EnvironmentSettings::appDataDirPath() const
{
	return ln::Path::combine(ln::Environment::specialFolderPath(ln::SpecialFolder::ApplicationData), u"Lumino");
}

ln::Path EnvironmentSettings::luminoPackageRootPath() const
{
	return _T("D:/Proj/LN/Lumino/build/CMakeInstallTemp");
}

ln::Path EnvironmentSettings::emscriptenPythonPath() const
{
	return _T("D:\\ProgramFiles\\Emscripten\\python\\2.7.5.3_64bit\\python.exe");
}

ln::Path EnvironmentSettings::emcmakePath() const
{
	return _T("D:\\ProgramFiles\\Emscripten\\emscripten\\1.35.0\\emcmake");
}

ln::Path EnvironmentSettings::emscriptenRootPath() const
{
	return _T("D:\\ProgramFiles\\Emscripten\\emscripten\\1.35.0");
}

//==============================================================================
// BuildEnvironment

BuildEnvironment::BuildEnvironment()
	: m_toolsDir()
	, m_emsdkVer()
	, m_emsdkRootDir()
	, m_emscriptenRootDir()
{
}

void BuildEnvironment::setupPathes(EnvironmentSettings* env)
{
	if (LN_REQUIRE(env)) return;
	m_toolsDir = (ln::Path::combine(env->appDataDirPath(), u"BuildTools"));
	m_emsdkVer = (u"1.38.10");
	m_emsdkName = u"sdk-1.38.10-64bit";
	m_emsdkRootDir = (ln::Path::combine(m_toolsDir, u"emsdk"));
	m_emscriptenRootDir = (ln::Path::combine(m_emsdkRootDir, u"emscripten", m_emsdkVer));

	ln::Path path = ln::Environment::executablePath();
	while (!path.isRoot())
	{
		if (ln::FileSystem::existsDirectory(ln::Path(path, u".git"))) {
			m_luminoRepoRoot = path;
			break;
		}
		path = path.parent();
	}
	if (LN_ENSURE(!m_luminoRepoRoot.isEmpty())) return;
	LN_LOG_DEBUG << m_luminoRepoRoot;

	m_luminoEmscriptenSdkDirPath = ln::Path(m_luminoRepoRoot, u"build/CMakeInstallTemp/Emscripten");
	m_projectTemplatesDirPath = ln::Path(m_luminoRepoRoot, u"tools/LuminoCLI/ProjectTemplates");
}

void BuildEnvironment::verifyAndInstall()
{
	ln::FileSystem::createDirectory(m_toolsDir);

	// Emscripten
	{
		if (!ln::FileSystem::existsDirectory(m_emsdkRootDir))
		{
			ln::Process proc1;
			proc1.setProgram(u"git");
			proc1.setArguments({ u"clone", u"https://github.com/juj/emsdk.git" });
			proc1.setWorkingDirectory(m_toolsDir);
			proc1.start();
			proc1.wait();
		}

		if (!ln::FileSystem::existsDirectory(m_emscriptenRootDir))
		{
			ln::Process proc1;
			proc1.setProgram(u"emsdk");
			proc1.setArguments({ u"install", m_emsdkVer });
			proc1.setWorkingDirectory(m_emsdkRootDir);
			proc1.start();
			proc1.wait();
		}

		auto file = ln::Path::combine(m_emscriptenRootDir, u"system", u"include", u"LuminoEngine.hpp");
		if (!ln::FileSystem::existsFile(file))
		{
			// 必須ファイルが1つ無かったので、とりあえず全部インストールしなおす

			auto srcIncludeDir = ln::Path::combine(m_luminoEmscriptenSdkDirPath, u"include");
			auto dstIncludeDir = ln::Path::combine(m_emscriptenRootDir, u"system", u"include");
			ln::FileSystem::copyDirectory(srcIncludeDir, dstIncludeDir, true, true);

			auto srcLibDir = ln::Path::combine(m_luminoEmscriptenSdkDirPath, u"lib");
			auto dstLibDir = ln::Path::combine(m_emscriptenRootDir, u"system", u"lib");
			ln::FileSystem::copyDirectory(srcLibDir, dstLibDir, true, true);

		}
		

	}

}

