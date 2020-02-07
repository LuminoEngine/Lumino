
#include "../../LuminoEngine/src/Asset/AssetArchive.hpp"
#include "../../LuminoEngine/src/Shader/UnifiedShader.hpp"
#include "EnvironmentSettings.hpp"
#include "Workspace.hpp"
#include "Project.hpp"
#include "BuildAssetHelper.hpp"
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
    auto projectTemplatesDir = project()->workspace()->buildEnvironment()->projectTemplatesDirPath();
	auto dstRoot = project()->rootDirPath();

    ln::Path srcRoot;
    if (!templateName.isEmpty()) {
        srcRoot = ln::Path(projectTemplatesDir, templateName);
    }
    else {
        srcRoot = ln::Path(projectTemplatesDir, u"NativeProject");
    }

    if (!ln::FileSystem::existsDirectory(srcRoot)) {
        // TODO: 事前検証
        CLI::error(u"Invalid project template.");
        return false;
    }

	CLI::info(u"Copying template...");

    //if (templateName.isEmpty())
    //{
    //    CLI::info(u"Template: Default");
    //    

    //    // TODO: v0.7.0 時点の動作。else 側に統一したい

    //    // Common
    //    {
    //        ln::String files[] = {
    //            u".gitignore",
    //            u"Sources/Application.cpp",
    //            u"Sources/Application.h",
    //        };

    //        for (auto& file : files)
    //        {
    //            auto filePath = ln::Path(dstRoot, file);
    //            auto dirPath = filePath.parent();
    //            if (!ln::FileSystem::existsDirectory(dirPath)) {
    //                ln::FileSystem::createDirectory(dirPath);
    //            }

    //            ln::FileSystem::copyFile(ln::Path(srcRoot, file), filePath, ln::FileCopyOption::Overwrite);
    //        }

    //        // Assets
    //        ln::FileSystem::copyDirectory(ln::Path(srcRoot, u"Assets"), ln::Path(dstRoot, u"Assets"), true, true);
    //    }

    //    // Windows
    //    {
    //        ln::FileSystem::copyFile(
    //            ln::Path(srcRoot, u"LuminoApp.sln"),
    //            ln::Path(dstRoot, u"LuminoApp.sln"), ln::FileCopyOption::Overwrite);

    //        ln::FileSystem::copyDirectory(
    //            ln::Path(srcRoot, u"Projects/LuminoApp.Windows"),
    //            ln::Path(dstRoot, u"Projects/LuminoApp.Windows"),
    //            true, true);
    //    }

    //    // macOS
    //    {
    //        ln::FileSystem::copyDirectory(
    //            ln::Path(srcRoot, u"Projects/LuminoApp.macOS"),
    //            ln::Path(dstRoot, u"Projects/LuminoApp.macOS"),
    //            true, true);

    //        ln::FileSystem::writeAllText(
    //            ln::Path(dstRoot, u"Projects/LuminoApp.macOS/LuminoApp.macOS/Config-generated.xcconfig"),
    //            ln::String::format(u"LUMINO_PATH=" + project()->workspace()->buildEnvironment()->luminoPackageRootDir().unify()));
    //    }

    //    // Web
    //    {
    //        ln::FileSystem::copyDirectory(
    //            ln::Path(srcRoot, u"Projects/LuminoApp.Web"),
    //            ln::Path(dstRoot, u"Projects/LuminoApp.Web"),
    //            true, true);
    //    }

    //    // iOS
    //    {
    //        ln::FileSystem::copyDirectory(
    //            ln::Path(srcRoot, u"Projects/LuminoApp.iOS"),
    //            ln::Path(dstRoot, u"Projects/LuminoApp.iOS"),
    //            true, true);

    //        ln::FileSystem::writeAllText(
    //            ln::Path(dstRoot, u"Projects/LuminoApp.iOS/LuminoApp.iOS/Config-generated.xcconfig"),
    //            ln::String::format(u"LUMINO_PATH=" + project()->workspace()->buildEnvironment()->luminoPackageRootDir().unify()));
    //    }

    //    // Android
    //    {
    //        ln::FileSystem::copyDirectory(
    //            ln::Path(srcRoot, u"Projects/LuminoApp.Android"),
    //            ln::Path(dstRoot, u"Projects/LuminoApp.Android"),
    //            true, true);
    //    }
    //}
    //else
    {
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
    }

	CLI::info("Copied template.");
	return true;
}

ln::Result CppLanguageContext::applyEngine()
{
    if (project()->properties()->engine.indexOf(u"local") == 0)
    {
        if (!ln::FileSystem::getFile(project()->engineDirPath()).isEmpty()) {
            CLI::warning(u"File exists in the engine folder.");
        }
        else {
            CLI::info("Copying Engine...");

            ln::FileSystem::copyDirectory(
                ln::Path::combine(project()->workspace()->buildEnvironment()->luminoPackageRootDir(), u"Engine", u"Native"),
                ln::Path::combine(project()->engineDirPath(), u"Native"),
                true, true);

            CLI::info("Copied Engine.");
        }
    }
    // リポジトリ clone
    else if (project()->properties()->engine.indexOf(u"repo:") == 0)
    {
        auto engineRoot = ln::Path::combine(project()->engineDirPath(), u"Native");
        auto branch = project()->properties()->engine.substr(5);
        int result = 0;

        result = ln::Process::execute(
            u"git",
            {
                u"clone",
                u"--progress",
                u"-b", branch,
                u"https://github.com/lriki/Lumino.git",
                engineRoot,
            });
        if (result != 0) {
            return false;
        }

        result = ln::Process::execute(u"dotnet", { u"run", u"--project", ln::Path(engineRoot, u"build.csproj"), u"--", u"BuildForCI_1" });
        if (result != 0) {
            return false;
        }

        result = ln::Process::execute(u"dotnet", { u"run", u"--project", ln::Path(engineRoot, u"build.csproj"), u"--", u"BuildForCI_2" });
        if (result != 0) {
            return false;
        }

        result = ln::Process::execute(u"dotnet", { u"run", u"--project", ln::Path(engineRoot, u"build.csproj"), u"--", u"CopyEngineLibsToRepoRoot" });
        if (result != 0) {
            return false;
        }
    }
    else // system
    {
        // 各プロジェクトが環境変数でインストール済みパッケージを参照する
    }

    return true;
}

void CppLanguageContext::restore()
{
    applyEngine();
}

ln::Result CppLanguageContext::build(const ln::String& target)
{
	if (!LanguageContext::build(target)) {
		return false;
	}

	if (ln::String::compare(target, u"Windows", ln::CaseSensitivity::CaseInsensitive) == 0) {
		return build_WindowsTarget();
	}
	else if (ln::String::compare(target, u"Web", ln::CaseSensitivity::CaseInsensitive) == 0) {
		return build_WebTarget();
	}

	return true;
}

ln::Result CppLanguageContext::build_WindowsTarget() const
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
	if (envSettings->actualPathBase() == EnvironmentPathBase::Repository) {
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

	ln::FileSystem::createDirectory(buildDir);

	{
		ln::List<ln::String> emcmakeArgs = {
			u"-DCMAKE_BUILD_TYPE=Release",
			u"-DCMAKE_INSTALL_PREFIX=" + installDir,
			u"-DLUMINO_ENGINE_ROOT=\"" + ln::Path(project()->engineDirPath(), u"Native").str().replace("\\", "/") + u"\"",
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
