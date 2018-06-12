
#include "EnvironmentSettings.hpp"
#include "Project.hpp"
#include "Workspace.hpp"

Workspace::Workspace()
	: m_environmentSettings(ln::makeRef<EnvironmentSettings>())
{
	m_environmentSettings->updatePathes();
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

Result Workspace::buildProject()
{
	ln::String luminoPackageDir = u"D:\\Proj\\LN\\Lumino\\build\\Package";//ln::Environment::getEnvironmentVariable();

	// Android
	{
		ln::String abi = u"x86_64";
		ln::String platform = "android-26";
		ln::String buildType = "Release";
		ln::String targetName = u"Android-" + abi;
		ln::String outputDir = u"D:/Documents/LuminoProjects/Hello/Projects/LuminoApplication/app/build/intermediates/cmake/release/obj/x86_64";
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

			u"-DLumino_DIR=" + luminoPackageDir,

			u"-G\"Android Gradle - Ninja\"",
		};

		ln::Process::execute(m_environmentSettings->androidSdkCMake(), args);

		ln::Process::execute(m_environmentSettings->androidSdkCMake(), {u"--build", buildDir });
	}

	return Result::OK;
}

Result Workspace::dev_installTools() const
{
	// Emscripten
	{
		auto srcIncludeDir = ln::Path::combine(m_environmentSettings->luminoPackageRootPath(), u"Emscripten", u"include");
		auto dstIncludeDir = ln::Path::combine(m_environmentSettings->emscriptenRootPath(), u"system", u"include");
		ln::FileSystem::copyDirectory(srcIncludeDir, dstIncludeDir, true, true);

		auto srcLibDir = ln::Path::combine(m_environmentSettings->luminoPackageRootPath(), u"Emscripten", u"lib");
		auto dstLibDir = ln::Path::combine(m_environmentSettings->emscriptenRootPath(), u"system", u"lib");
		ln::FileSystem::copyDirectory(srcLibDir, dstLibDir, true, true);
	}
	
	return Result::OK;
}
