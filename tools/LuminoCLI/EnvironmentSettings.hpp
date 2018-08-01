#pragma once

class EnvironmentSettings
	: public ln::RefObject
{
public:
	void updatePathes();

	ln::Path projectTemplatesDirPath() const;

	// リリースパッケージのルートパス。この下に include フォルダなどがある
	ln::Path luminoPackageRootPath() const;

	ln::Path appDataDirPath() const;

	ln::Path emscriptenPythonPath() const;
	ln::Path emcmakePath() const;
	ln::Path emscriptenRootPath() const;


	const ln::Path& androidSdkRootDir() const { return m_androidSdkRootDir; }
	const ln::Path& androidSdkCMake() const { return m_androidSdkCMake; }
	const ln::Path& androidSdkNinja() const { return m_androidSdkNinja; }
	const ln::Path& androidNdkRootDir() const { return m_androidNdkRootDir; }
	const ln::Path& androidCMakeToolchain() const { return m_androidCMakeToolchain; }

private:
	ln::Path m_androidSdkRootDir;
	ln::Path m_androidSdkCMake;
	ln::Path m_androidSdkNinja;
	ln::Path m_androidNdkRootDir;
	ln::Path m_androidCMakeToolchain;
};

class DevTools
	: public ln::RefObject
{
public:
	DevTools();
	void setupPathes(EnvironmentSettings* env);
	void install();
	void verify();

private:
	ln::Path m_toolsDir;
	ln::String m_emsdkVer;
	ln::Path m_emsdkRootDir;
	ln::Path m_emscriptenRootDir;
};
