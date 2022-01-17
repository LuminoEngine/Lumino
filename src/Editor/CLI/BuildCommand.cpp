
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

	if (ln::String::compare(target, _TT("Windows"), ln::CaseSensitivity::CaseInsensitive) == 0) {
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
	else if (ln::String::compare(target, _TT("Web"), ln::CaseSensitivity::CaseInsensitive) == 0) {
		if (!buildAssets()) {
			return 1;
		}
		if (!buildWebTarget(workspace)) {
			return 1;
		}
	}
	else if (ln::String::compare(target, _TT("Assets"), ln::CaseSensitivity::CaseInsensitive) == 0) {
		if (!buildAssets()) {
			return 1;
		}
	}
	else {
		CLI::error(ln::String::format(_TT("{0} is invalid target."), target));
		return 1;
	}
#endif

    return 0;
}

ln::String BuildCommand::selectDefaultTarget() const
{
#if defined(LN_OS_WIN32)
	return _TT("Windows");
#else
	return ln::String::Empty;
#endif
}

ln::Result BuildCommand::buildWindowsTarget(lna::Workspace* workspace, bool debug)
{
	auto file = ln::FileSystem::getFile(m_project->rootDirPath(), _TT("*.sln"));
	if (file.isEmpty()) {
		CLI::error(_TT(".sln file not found."));
		return ln::err();
	}

	if (debug) {
		if (ln::Process::execute(workspace->buildEnvironment()->msbuild(), { file.str(), _TT("/t:build"), _TT("/p:Configuration=Debug;Platform=\"x86\"") }) != 0) {
			CLI::error(_TT("Failed MSBuild."));
			return ln::err();
		}
	}
	else {
		if (ln::Process::execute(workspace->buildEnvironment()->msbuild(), { file.str(), _TT("/t:build"), _TT("/p:Configuration=Release;Platform=\"x86\"") }) != 0) {
			CLI::error(_TT("Failed MSBuild."));
			return ln::err();
		}
	}

	return ln::ok();
}

ln::Result BuildCommand::buildWindowsPackage(lna::Project* project)
{
	auto dstDir = ln::Path::combine(project->releaseDir(), _TT("Windows"));
	ln::FileSystem::createDirectory(dstDir);

	ln::FileSystem::copyFile(
		ln::Path::combine(project->windowsProjectDir(), _TT("bin"), _TT("Release"), _TT("LuminoApp.exe")),
		ln::Path::combine(dstDir, _TT("LuminoApp.exe")));

	ln::FileSystem::copyFile(
		ln::Path::combine(project->windowsProjectDir(), _TT("Assets.lca")),
		ln::Path::combine(dstDir, _TT("Assets.lca")));

	return ln::ok();
}



ln::Result BuildCommand::buildAndroidTarget()
{
#if 0
	// Android
	else if (ln::String::compare(target, _TT("Android", ln::CaseSensitivity::CaseInsensitive) == 0)
	{
	putenv("JAVA_HOME=\"D:\\Program Files\\Android\\Android Studio\\jre\"");

	ln::Process::execute(_TT("gradlew.bat", { _TT("assemble" });	// Debug, Release 両方ビルド

	// https://qiita.com/tkc_tsuchiya/items/6485714615ace9e19918

#if 0
	ln::String abi = _TT("x86_64";
	ln::String platform = "android-26";
	ln::String buildType = "Release";
	ln::String targetName = _TT("Android-" + abi;
	ln::Path outputDir = ln::Path(m_project->androidProjectDir(), _TT("app/build/intermediates/cmake/release/obj/" + abi);
	ln::Path luminoPackageDir = ln::Path(m_devTools->luminoPackageRootDir(), _TT("Engine/Cpp/Android-" + abi);
	ln::Path cmakeHomeDir = ln::Path(m_project->androidProjectDir(), _TT("app");
	ln::Path buildDir = ln::Path::combine(m_project->buildDir(), targetName);

	ln::List<ln::String> args = {
		_TT("-H" + cmakeHomeDir,
		_TT("-B" + buildDir,
		_TT("-DLN_TARGET_ARCH_NAME=" + targetName,
		_TT("-DANDROID_ABI=" + abi,
		_TT("-DANDROID_PLATFORM=" + platform,
		_TT("-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=" + outputDir.str().replace("\\", "/"),
		_TT("-DCMAKE_BUILD_TYPE=" + buildType,
		_TT("-DANDROID_NDK=" + m_environmentSettings->androidNdkRootDir(),
		_TT("-DCMAKE_CXX_FLAGS=-std=c++14",
		_TT("-DCMAKE_TOOLCHAIN_FILE=" + m_environmentSettings->androidCMakeToolchain(),
		_TT("-DCMAKE_MAKE_PROGRAM=" + m_environmentSettings->androidSdkNinja(),

		_TT("-DANDROID_STL=c++_shared",
		_TT("-DLumino_DIR=" + luminoPackageDir.str().replace("\\", "/"),

		_TT("-G\"Android Gradle - Ninja\"",
	};

	ln::Process::execute(m_environmentSettings->androidSdkCMake(), args);

	ln::Process::execute(m_environmentSettings->androidSdkCMake(), { _TT("--build", buildDir });
#endif
	}
#endif
    return ln::ok();
}
