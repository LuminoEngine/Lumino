
#include "EnvironmentSettings.hpp"
#include "Project.hpp"
#include "Workspace.hpp"

Workspace::Workspace()
	: m_environmentSettings(ln::makeRef<EnvironmentSettings>())
	, m_devTools(ln::makeRef<BuildEnvironment>())
{
	m_environmentSettings->updatePathes();
	m_devTools->setupPathes(m_environmentSettings);
}

Workspace::~Workspace()
{
}

Result Workspace::newProject(const ln::Path& dir, const ln::String& projectName)
{
	m_project = ln::makeRef<Project>(this);
	return m_project->newProject(dir, projectName);
}

Result Workspace::openProject(const ln::Path& dir)
{
	m_project = ln::makeRef<Project>(this);
	return m_project->openProject(dir);
}

Result Workspace::buildProject(const ln::String& target)
{

	// Android
	if (ln::String::compare(target, u"Android", ln::CaseSensitivity::CaseInsensitive) == 0)
	{
		//::SetCurrentDirectoryW(L"D:\\Documents\\LuminoProjects\\HelloLumino\\NativeProjects\\LuminoApp.Android");

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

	// Emscripten
	if (ln::String::compare(target, u"Emscripten", ln::CaseSensitivity::CaseInsensitive) == 0)
	{
		auto buildDir = ln::Path::combine(m_project->buildDir(), u"Emscripten").canonicalize();
		auto installDir = ln::Path::combine(buildDir, u"Release");
		auto cmakeSourceDir = m_project->rootDirPath();
		auto script = ln::Path::combine(buildDir, u"build.bat");

		ln::FileSystem::createDirectory(buildDir);

		{
			ln::List<ln::String> emcmakeArgs = {
				u"-DCMAKE_BUILD_TYPE=Release",
				u"-DCMAKE_INSTALL_PREFIX=" + installDir,
				u"-G \"MinGW Makefiles\"",
				cmakeSourceDir,
			};

			ln::StreamWriter sw(script);
			sw.writeLineFormat(u"cd /d \"{0}\"", m_devTools->emsdkDirPath());
			sw.writeLineFormat(u"call emsdk_env.bat " + m_devTools->emsdkName());
			sw.writeLineFormat(u"call emsdk_env.bat");
			sw.writeLineFormat(u"cd /d \"{0}\"", buildDir);
			sw.writeLineFormat(u"call emcmake cmake " + ln::String::join(emcmakeArgs, u" "));
			sw.writeLineFormat(u"call cmake --build .");
		}

		ln::Process::execute(script);
	}

	return Result::OK;
}

Result Workspace::dev_installTools() const
{
	m_devTools->verifyAndInstall();


	
	return Result::OK;
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
		putenv((u"LUMINO_PATH=" + buildEnvironment()->luminoPackageRootDir()).toStdString().c_str());

		ln::Process proc;
		proc.setProgram("LuminoCppTemplate.sln");
		proc.start();
	}
#else
	LN_NOTIMPLEMENTED();	// TODO: putenv は書き込み可能なポインタを渡さないとならないみたい？
#endif
}
