
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

ln::Path EnvironmentSettings::projectTemplatesDirPath() const
{
	return _T("D:/Proj/LN/Lumino/tools/LuminoCLI/ProjectTemplates");
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
