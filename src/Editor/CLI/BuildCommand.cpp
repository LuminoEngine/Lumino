
#include <EnvironmentSettings.hpp>
#include <Workspace.hpp>
#include <Project/Project.hpp>
#include <Project/LanguageContext.hpp>
#include "BuildCommand.hpp"
#include "FxcCommand.hpp"

int BuildCommand::execute(lna::Workspace* workspace, lna::Project* project)
{
	m_project = project;

	if (target.isEmpty()) {
		target = selectDefaultTarget();
	}

	if (!m_project->languageContext()->build(target)) {
		return 1;
	}

#if 0

	if (ln::String::compare(target, u"Windows", ln::CaseSensitivity::CaseInsensitive) == 0) {
		if (!buildAssets()) {
			return 1;
		}
		if (!package) {
			if (!buildWindowsTarget(workspace, true)) {
				return 1;
			}
		}
		else {
			if (!buildWindowsTarget(workspace, false)) {
				return 1;
			}
			if (!buildWindowsPackage(project)) {
				return 1;
			}
		}
	}
	else if (ln::String::compare(target, u"Web", ln::CaseSensitivity::CaseInsensitive) == 0) {
		if (!buildAssets()) {
			return 1;
		}
		if (!buildWebTarget(workspace)) {
			return 1;
		}
	}
	else if (ln::String::compare(target, u"Assets", ln::CaseSensitivity::CaseInsensitive) == 0) {
		if (!buildAssets()) {
			return 1;
		}
	}
	else {
		CLI::error(ln::String::format(u"{0} is invalid target.", target));
		return 1;
	}
#endif

    return 0;
}

ln::String BuildCommand::selectDefaultTarget() const
{
#if defined(LN_OS_WIN32)
	return u"Windows";
#else
	return ln::String::Empty;
#endif
}

ln::Result BuildCommand::buildWindowsTarget(lna::Workspace* workspace, bool debug)
{
	auto file = ln::FileSystem::getFile(m_project->rootDirPath(), u"*.sln");
	if (file.isEmpty()) {
		CLI::error(".sln file not found.");
		return false;
	}

	if (debug) {
		if (ln::Process::execute(workspace->buildEnvironment()->msbuild(), { file.str(), u"/t:build", u"/p:Configuration=Debug;Platform=\"x86\"" }) != 0) {
			CLI::error("Failed MSBuild.");
			return false;
		}
	}
	else {
		if (ln::Process::execute(workspace->buildEnvironment()->msbuild(), { file.str(), u"/t:build", u"/p:Configuration=Release;Platform=\"x86\"" }) != 0) {
			CLI::error("Failed MSBuild.");
			return false;
		}
	}

	return true;
}

ln::Result BuildCommand::buildWindowsPackage(lna::Project* project)
{
	auto dstDir = ln::Path::combine(project->releaseDir(), u"Windows");
	ln::FileSystem::createDirectory(dstDir);

	ln::FileSystem::copyFile(
		ln::Path::combine(project->windowsProjectDir(), u"bin", u"Release", u"LuminoApp.exe"),
		ln::Path::combine(dstDir, u"LuminoApp.exe"));

	ln::FileSystem::copyFile(
		ln::Path::combine(project->windowsProjectDir(), u"Assets.lca"),
		ln::Path::combine(dstDir, u"Assets.lca"));

	return true;
}



ln::Result BuildCommand::buildAndroidTarget()
{
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

	ln::Process::execute(m_environmentSettings->androidSdkCMake(), { u"--build", buildDir });
#endif
	}
#endif
	return true;
}
