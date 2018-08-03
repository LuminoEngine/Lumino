
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
	m_emsdkRootDir = (ln::Path::combine(m_toolsDir, u"emsdk"));
	m_emscriptenRootDir = (ln::Path::combine(m_emsdkRootDir, m_emsdkVer));

	//ln::Path sp = ln::Environment::executablePath();
	m_projectTemplatesDirPath = _T("C:/Proj/GitHub/Lumino/tools/LuminoCLI/ProjectTemplates");
}

void BuildEnvironment::install()
{
	ln::FileSystem::createDirectory(m_toolsDir);

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
}

void BuildEnvironment::verify()
{

}
