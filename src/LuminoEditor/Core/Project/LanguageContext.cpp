
#include "../../LuminoEngine/src/Asset/AssetArchive.hpp"
#include "../../LuminoEngine/src/Shader/UnifiedShader.hpp"
#include "EnvironmentSettings.hpp"
#include "Workspace.hpp"
#include "Project.hpp"
#include "BuildAssetHelper.hpp"
#include "ProjectTemplateManager.hpp"
#include "LanguageContext.hpp"

namespace lna {

//==============================================================================
// LanguageContext

LanguageContext::LanguageContext(Project* project)
	: m_project(project)
{
}

LanguageContext::~LanguageContext()
{
}

ln::Result LanguageContext::build(const ln::String& target)
{
	if (!buildAssets()) {
		return false;
	}

	return true;
}

ln::Result LanguageContext::buildAssets() const
{
	ln::detail::CryptedAssetArchiveWriter writer;
	auto outputFilePath = ln::Path(m_project->buildDir(), u"Assets.lca");
	writer.open(outputFilePath, ln::detail::CryptedArchiveHelper::DefaultPassword);

	for (auto& file : ln::FileSystem::getFiles(m_project->assetsDir(), ln::StringRef(), ln::SearchOption::Recursive)) {
		if (file.hasExtension(".fx")) {

			ln::Path outputFile;
			if (!BuildAssetHelper::buildShaderFromAutoBuild(m_project, file, &outputFile)) {
				return false;
			}

			writer.addFile(outputFile, m_project->assetsDir().makeRelative(file).replaceExtension(ln::detail::UnifiedShader::FileExt));
			CLI::info(file);
		}
		else {
			writer.addFile(file, m_project->assetsDir().makeRelative(file));
			CLI::info(file);
		}
	}

	writer.close();

	// Android
	{
		if (ln::FileSystem::existsDirectory(m_project->androidProjectDir())) {
			auto dst = ln::Path::combine(m_project->androidProjectDir(), u"app", u"src", u"main", u"assets", u"Assets.lca");
			ln::FileSystem::createDirectory(dst.parent());
			ln::FileSystem::copyFile(outputFilePath, dst, ln::FileCopyOption::Overwrite);
			CLI::info(u"Copy to " + dst);
		}
	}

	// macOS
	{
		if (ln::FileSystem::existsDirectory(m_project->macOSProjectDir())) {
			auto dst = ln::Path::combine(m_project->macOSProjectDir(), u"LuminoApp.macOS", u"Assets.lca");
			ln::FileSystem::copyFile(outputFilePath, dst, ln::FileCopyOption::Overwrite);
			CLI::info(u"Copy to " + dst);
		}
	}

	// iOS
	{
		if (ln::FileSystem::existsDirectory(m_project->iOSProjectDir())) {
			auto dst = ln::Path::combine(m_project->iOSProjectDir(), u"LuminoApp.iOS", u"Assets.lca");
			ln::FileSystem::copyFile(outputFilePath, dst, ln::FileCopyOption::Overwrite);
			CLI::info(u"Copy to " + dst);
		}
	}

	// Windows
	{
		if (ln::FileSystem::existsDirectory(m_project->windowsProjectDir())) {
			auto dst = ln::Path::combine(m_project->windowsProjectDir(), u"Assets.lca");
			ln::FileSystem::copyFile(outputFilePath, dst, ln::FileCopyOption::Overwrite);
			CLI::info(u"Copy to " + dst);
		}
	}

	// Web
	{
		if (ln::FileSystem::existsDirectory(m_project->buildDir())) {
			auto dstDir = ln::Path::combine(m_project->buildDir(), u"Web");
			ln::FileSystem::createDirectory(dstDir);

			auto dst = ln::Path::combine(dstDir, u"Assets.lca");
			ln::FileSystem::copyFile(outputFilePath, dst, ln::FileCopyOption::Overwrite);
			CLI::info(u"Copy to " + dst);
		}
	}

	CLI::info(u"Compilation succeeded.");

	return true;
}



//==============================================================================
// CppLanguageContext

CppLanguageContext::CppLanguageContext(Project* project)
	: LanguageContext(project)
{
}

CppLanguageContext::~CppLanguageContext()
{
}

ln::Result CppLanguageContext::applyTemplates(const ln::String& templateName)
{
	auto templateProject = Workspace::instance()->projectTemplateManager()->findTemplate(u"cpp", templateName);
	if (!templateProject) {
		CLI::error(u"Invalid project template.");
		return false;
	}

	auto projectTemplatesDir = project()->workspace()->buildEnvironment()->projectTemplatesDirPath();
	auto dstRoot = project()->rootDirPath();
	auto srcRoot = templateProject->directoryPath;

    CLI::info(u"Template: " + templateName);

    // 先にフォルダを作っておく
    for (auto dir : ln::FileSystem::getDirectories(srcRoot, ln::StringRef(), ln::SearchOption::Recursive)) {
        auto rel = srcRoot.makeRelative(dir);
        ln::FileSystem::createDirectory(ln::Path(dstRoot, rel));
    }

    // ファイルをコピー
    for (auto file : ln::FileSystem::getFiles(srcRoot, ln::StringRef(), ln::SearchOption::Recursive)) {
        auto rel = srcRoot.makeRelative(file);
        ln::FileSystem::copyFile(file, ln::Path(dstRoot, rel));
    }

	CLI::info("Copied template.");
	return true;
}

void CppLanguageContext::restore()
{
}

ln::Result CppLanguageContext::build(const ln::String& target)
{
	if (!LanguageContext::build(target)) {
		return false;
	}

	if (ln::String::compare(target, u"Windows", ln::CaseSensitivity::CaseInsensitive) == 0) {
		return build_NativeCMakeTarget();
	}
	else if (ln::String::compare(target, u"Web", ln::CaseSensitivity::CaseInsensitive) == 0) {
		return build_WebTarget();
	}

	return true;
}

ln::Result CppLanguageContext::build_NativeCMakeTarget() const
{
	//ln::String arch = u"MSVC2019-x64-MT";
	ln::String arch = u"MSVC2017-x64-MT";

	ln::List<ln::String> args = {
		project()->rootDirPath(),
		u"-G \"Visual Studio 15 Win64\"",
		u"-DLN_TARGET_ARCH=" + arch,
		u"-DLN_MSVC_STATIC_RUNTIME=ON",
		//u"-DLUMINO_ENGINE_ROOT=\"" + ln::Path(m_project->engineDirPath(), u"Native").str().replace("\\", "/") + u"\"",
		//u"-DLN_TARGET_ARCH=Emscripten",
		//cmakeSourceDir,
	};

	// for tool development and debuging.
	auto& envSettings = project()->workspace()->buildEnvironment();
	if (lna::Workspace::developMode) {
		args.add(ln::String::format(u"-DLUMINO_REPO_ROOT=\"{0}\"", envSettings->luminoPackageRootDir()));
	}

	auto buildDir = ln::Path(project()->buildDir(), arch);
	ln::FileSystem::createDirectory(buildDir);

	ln::Process cmake;
	cmake.setProgram(u"cmake");
	cmake.setArguments(args);
	cmake.setWorkingDirectory(buildDir);
	cmake.start();
	cmake.wait();
	if (cmake.exitCode() != 0) {
		CLI::error(u"Failed cmake.");
		return false;
	}

	return true;
}

ln::Result CppLanguageContext::build_WebTarget() const
{
	Workspace* workspace = project()->workspace();

	// emsdk がなければインストールする
	workspace->buildEnvironment()->prepareEmscriptenSdk();

	auto buildDir = ln::Path::combine(project()->buildDir(), u"Web").canonicalize();
	auto installDir = ln::Path::combine(buildDir, u"Release");
	auto cmakeSourceDir = project()->emscriptenProjectDir();
	auto script = ln::Path::combine(buildDir, u"build.bat");
	auto engineRoot = ln::Path(workspace->buildEnvironment()->emscriptenSysRootLocalDir(), u"LuminoEngine");

	ln::FileSystem::createDirectory(buildDir);

	{
		ln::List<ln::String> emcmakeArgs = {
			u"-DCMAKE_BUILD_TYPE=Release",
			u"-DCMAKE_INSTALL_PREFIX=" + installDir,
			u"-DLUMINO_ENGINE_ROOT=\"" + engineRoot.str().replace("\\", "/") + u"\"",
			u"-DLN_TARGET_ARCH=Emscripten",
			u"-G \"MinGW Makefiles\"",
			cmakeSourceDir,
		};

		ln::StreamWriter sw(script);
		sw.writeLineFormat(u"cd /d \"{0}\"", workspace->buildEnvironment()->emsdkDirPath());
		sw.writeLineFormat(u"call emsdk activate " + workspace->buildEnvironment()->emsdkName());
		sw.writeLineFormat(u"call emsdk_env.bat");
		sw.writeLineFormat(u"cd /d \"{0}\"", buildDir);
		sw.writeLineFormat(u"call emcmake cmake " + ln::String::join(emcmakeArgs, u" "));
		sw.writeLineFormat(u"call cmake --build .");
	}

	ln::Process::execute(script);
	return true;
}

} // namespace lna
