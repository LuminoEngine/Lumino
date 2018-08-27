#pragma once

class EnvironmentSettings
	: public ln::RefObject
{
public:
	void updatePathes();


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

class BuildEnvironment
	: public ln::RefObject
{
public:
	BuildEnvironment();
	void setupPathes(EnvironmentSettings* env);
	void verifyAndInstall();

	const ln::Path& luminoEmscriptenSdkDirPath() const { return m_luminoEmscriptenSdkDirPath; }
	
	ln::Path projectTemplatesDirPath() const { return m_projectTemplatesDirPath; }

	const ln::Path& luminoPackageRootDir() const { return m_luminoPackageRootDir; }
	

	const ln::String& emsdkName() const { return m_emsdkName; }
	const ln::Path& emsdkDirPath() const { return m_emsdkRootDir; }
	const ln::Path& emscriptenDirPath() const { return m_emscriptenRootDir; }

private:
	ln::Path m_luminoRepoRoot;
	ln::Path m_luminoPackageRootDir;
	ln::Path m_luminoEmscriptenSdkDirPath;
	ln::Path m_projectTemplatesDirPath;
	ln::Path m_toolsDir;
	ln::String m_emsdkVer;
	ln::String m_emsdkName;
	ln::Path m_emsdkRootDir;
	ln::Path m_emscriptenRootDir;
};
