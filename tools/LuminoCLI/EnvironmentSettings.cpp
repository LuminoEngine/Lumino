
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
	m_luminoPackageRootDir = ln::Path(luminoRepoRoot, u"Lumino-0.6.0-Windows");
#elif defined(LN_OS_MAC)
	m_luminoPackageRootDir = ln::Path(luminoRepoRoot, u"Lumino-0.6.0-macOS");
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

Result BuildEnvironment::prepareEmscriptenSdk()
{
	ln::FileSystem::createDirectory(m_toolsDir);

	// Emscripten
	{
		if (!ln::FileSystem::existsDirectory(m_emsdkRootDir))
		{
			if (!callProcess(u"git", { u"clone", u"https://github.com/juj/emsdk.git" }, m_toolsDir)) {
				return Result::Fail;
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
				return Result::Fail;
			}
			if (!callProcess(emsdk, { u"update-tags" }, m_emsdkRootDir)) {
				return Result::Fail;
			}
			if (!callProcess(emsdk, { u"install", m_emsdkName }, m_emsdkRootDir)) {
				return Result::Fail;
			}
		}
	}
}

Result BuildEnvironment::callProcess(const ln::String& program, const ln::List<ln::String>& arguments, const ln::Path& workingDir)
{
	ln::Process proc1;
	proc1.setUseShellExecute(false);
	proc1.setProgram(program);
	proc1.setArguments(arguments);
	proc1.setWorkingDirectory(workingDir);
	proc1.start();
	proc1.wait();
	if (proc1.exitCode() == 0) {
		return Result::Success;
	}
	else {
		return Result::Fail;
	}
}

