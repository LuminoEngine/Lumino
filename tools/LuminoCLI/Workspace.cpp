
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

		ln::String luminoPackageDir = u"D:\\Proj\\GitHub\\Lumino\\build\\CMakeInstallTemp\\Android-x86_64";// \\lib\\cmake";//ln::Environment::getEnvironmentVariable();

		ln::String abi = u"x86_64";
		ln::String platform = "android-26";
		ln::String buildType = "Release";
		ln::String targetName = u"Android-" + abi;
		ln::String outputDir = m_project->rootDirPath().str().replace("\\", "/") + u"Projects/Android/app/build/intermediates/cmake/release/obj/x86_64";
		ln::String buildDir = ln::Path::combine(m_project->buildDir(), targetName);

		ln::List<ln::String> args = {
			u"-H" + m_project->rootDirPath().str(),
			u"-B" + buildDir,
			u"-DLN_TARGET_ARCH_NAME=" + targetName,
			u"-DANDROID_ABI=" + abi,
			u"-DANDROID_PLATFORM=" + platform,
			u"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=" + outputDir,
			u"-DCMAKE_BUILD_TYPE=" + buildType,
			u"-DANDROID_NDK=" + m_environmentSettings->androidNdkRootDir(),
			u"-DCMAKE_CXX_FLAGS=-std=c++14",
			u"-DCMAKE_TOOLCHAIN_FILE=" + m_environmentSettings->androidCMakeToolchain(),
			u"-DCMAKE_MAKE_PROGRAM=" + m_environmentSettings->androidSdkNinja(),

			u"-DLumino_DIR=" + luminoPackageDir.replace("\\", "/"),

			u"-G\"Android Gradle - Ninja\"",
		};

		ln::Process::execute(m_environmentSettings->androidSdkCMake(), args);

		ln::Process::execute(m_environmentSettings->androidSdkCMake(), {u"--build", buildDir });
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
