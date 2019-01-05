
#include "EnvironmentSettings.hpp"
#include "Project.hpp"
#include "Workspace.hpp"

Workspace::Workspace()
	: m_devTools(ln::makeRef<BuildEnvironment>())
{
	m_devTools->setupPathes();
}

Workspace::~Workspace()
{
}

Result Workspace::openProject(const ln::Path& dir)
{
	m_project = ln::makeRef<Project>(this);
	return m_project->openProject(dir);
}

Result Workspace::buildProject(const ln::String& target)
{
	// Windows
	if (ln::String::compare(target, u"Windows", ln::CaseSensitivity::CaseInsensitive) == 0)
	{
		auto file = ln::FileSystem::getFile(m_project->rootDirPath(), u"*.sln");
		if (file.isEmpty()) {
			CLI::error(".sln file not found.");
			return Result::Fail;
		}

		if (ln::Process::execute(m_devTools->msbuild(), { file.str(), u"/t:build", u"/p:Configuration=Debug;Platform=\"x86\"" }) != 0) {
			CLI::error("Failed MSBuild.");
			return Result::Fail;
		}
	}
#if 0
	// Android
	else if (ln::String::compare(target, u"Android", ln::CaseSensitivity::CaseInsensitive) == 0)
	{
		putenv("JAVA_HOME=\"D:\\Program Files\\Android\\Android Studio\\jre\"");

		ln::Process::execute(u"gradlew.bat", { u"assemble" });	// Debug, Release 両方ビルド

		// https://qiita.com/tkc_tsuchiya/items/6485714615ace9e19918

#if 0
		ln::String abi = u"x86_64";
		ln::String platform = "android-26";
		ln::String buildType = "Release";
		ln::String targetName = u"Android-" + abi;
		ln::Path outputDir = ln::Path(m_project->androidProjectDir(), u"app/build/intermediates/cmake/release/obj/" + abi);
		ln::Path luminoPackageDir = ln::Path(m_devTools->luminoPackageRootDir(), u"Engine/Cpp/Android-" + abi);
		ln::Path cmakeHomeDir = ln::Path(m_project->androidProjectDir(), u"app");
		ln::Path buildDir = ln::Path::combine(m_project->buildDir(), targetName);

		ln::List<ln::String> args = {
			u"-H" + cmakeHomeDir,
			u"-B" + buildDir,
			u"-DLN_TARGET_ARCH_NAME=" + targetName,
			u"-DANDROID_ABI=" + abi,
			u"-DANDROID_PLATFORM=" + platform,
			u"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=" + outputDir.str().replace("\\", "/"),
			u"-DCMAKE_BUILD_TYPE=" + buildType,
			u"-DANDROID_NDK=" + m_environmentSettings->androidNdkRootDir(),
			u"-DCMAKE_CXX_FLAGS=-std=c++14",
			u"-DCMAKE_TOOLCHAIN_FILE=" + m_environmentSettings->androidCMakeToolchain(),
			u"-DCMAKE_MAKE_PROGRAM=" + m_environmentSettings->androidSdkNinja(),

			u"-DANDROID_STL=c++_shared",
			u"-DLumino_DIR=" + luminoPackageDir.str().replace("\\", "/"),

			u"-G\"Android Gradle - Ninja\"",
		};

		ln::Process::execute(m_environmentSettings->androidSdkCMake(), args);

		ln::Process::execute(m_environmentSettings->androidSdkCMake(), {u"--build", buildDir });
#endif
	}
#endif
	// Web
	else if (ln::String::compare(target, u"Web", ln::CaseSensitivity::CaseInsensitive) == 0)
	{
		// emsdk がなければインストールする
		m_devTools->prepareEmscriptenSdk();

		auto buildDir = ln::Path::combine(m_project->buildDir(), u"Web").canonicalize();
		auto installDir = ln::Path::combine(buildDir, u"Release");
		auto cmakeSourceDir = m_project->emscriptenProjectDir();
		auto script = ln::Path::combine(buildDir, u"build.bat");

		ln::FileSystem::createDirectory(buildDir);

		{
			ln::List<ln::String> emcmakeArgs = {
				u"-DCMAKE_BUILD_TYPE=Release",
				u"-DCMAKE_INSTALL_PREFIX=" + installDir,
				u"-DLUMINO_ENGINE_ROOT=\"" + ln::Path(m_project->engineDirPath(), u"Native").str().replace("\\", "/") + u"\"",
				u"-DLN_TARGET_ARCH=Emscripten",
				u"-G \"MinGW Makefiles\"",
				cmakeSourceDir,
			};

			ln::StreamWriter sw(script);
			sw.writeLineFormat(u"cd /d \"{0}\"", m_devTools->emsdkDirPath());
			sw.writeLineFormat(u"call emsdk activate " + m_devTools->emsdkName());
			sw.writeLineFormat(u"call emsdk_env.bat");
			sw.writeLineFormat(u"cd /d \"{0}\"", buildDir);
			sw.writeLineFormat(u"call emcmake cmake " + ln::String::join(emcmakeArgs, u" "));
			sw.writeLineFormat(u"call cmake --build .");
		}

		ln::Process::execute(script);
	}
	else
	{
		CLI::error(ln::String::format(u"{0} is invalid target.", target));
		return Result::Fail;
	}

	return Result::Success;
}

Result Workspace::runProject(const ln::String& target)
{
	// Windows
	if (ln::String::compare(target, u"Windows", ln::CaseSensitivity::CaseInsensitive) == 0)
	{
		auto exe = ln::FileSystem::getFile(ln::Path(m_project->windowsProjectDir(), u"bin/Debug"), u"*.exe");
		ln::Process::execute(exe);
	}
	// Web
	else if (ln::String::compare(target, u"Web", ln::CaseSensitivity::CaseInsensitive) == 0)
	{
		auto buildDir = ln::Path::combine(m_project->buildDir(), u"Web").canonicalize();

		ln::Process proc;
		proc.setProgram(m_devTools->python2());
		proc.setArguments({u"-m", u"SimpleHTTPServer", u"8000"});
		proc.setWorkingDirectory(buildDir);
		proc.setUseShellExecute(false);
		proc.start();

		{
			auto files = ln::FileSystem::getFiles(buildDir, u"*.html");
			if (files.isEmpty()) {
				CLI::error("Not found *.html file.");
				return Result::Fail;
			}

			ln::Process proc2;
			proc2.setProgram(u"http://localhost:8000/" + (*files.begin()).fileName());
			proc2.start();
		}

		proc.wait();
	}
	else
	{
		CLI::error(ln::String::format(u"{0} is invalid target.", target));
		return Result::Fail;
	}

	return Result::Success;
}

Result Workspace::restoreProject()
{
	m_project->restore();
	return Result::Success;
}

Result Workspace::dev_installTools() const
{
	m_devTools->prepareEmscriptenSdk();


	
	return Result::Success;
}

void Workspace::dev_openIde(const ln::String& target) const
{
#if defined(_WIN32)
	if (ln::String::compare(target, u"Android", ln::CaseSensitivity::CaseInsensitive) == 0)
	{
		HKEY hKey = NULL;
		LONG lRet = RegOpenKeyExW(HKEY_LOCAL_MACHINE,
			L"SOFTWARE\\Android Studio",
			NULL,
			KEY_READ | KEY_WOW64_64KEY,	// https://stackoverflow.com/questions/252297/why-is-regopenkeyex-returning-error-code-2-on-vista-64bit
			&hKey);
		if (lRet != ERROR_SUCCESS) {
			LN_LOG_ERROR << u"Android Studio not installed.";
			return;
		}

		DWORD type, size;
		WCHAR path[MAX_PATH];
		RegQueryValueExW(hKey, L"Path", NULL, &type, (LPBYTE)path, &size);

		ln::Environment::setEnvironmentVariable(u"LUMINO", buildEnvironment()->luminoPackageRootDir());

		ln::Process proc;
		proc.setProgram(ln::Path::combine(ln::String::fromCString(path), u"bin", u"studio"));
		proc.setArguments({ m_project->androidProjectDir() });
		proc.start();
	}
	else
	{
		auto files = ln::FileSystem::getFiles(ln::Environment::currentDirectory(), u"*.sln");
		if (files.isEmpty()) {
			CLI::error("Not found *.sln file.");
			return;
		}

		ln::Environment::setEnvironmentVariable(u"LUMINO", buildEnvironment()->luminoPackageRootDir());

		ln::Process proc;
		proc.setProgram(*files.begin());
		proc.start();
	}
#elif defined(__APPLE__)
	ln::Process proc;
	proc.setProgram(u"/usr/bin/open");
	proc.setArguments({ u"/Applications/Xcode.app/", ln::Path(m_project->macOSProjectDir(), u"LuminoApp.macOS.xcodeproj") });
	proc.start();
#else
	LN_NOTIMPLEMENTED();	// TODO: putenv は書き込み可能なポインタを渡さないとならないみたい？
#endif
}
