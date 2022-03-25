
#include "../../../lumino/Runtime/src/Asset/AssetArchive.hpp"
#include <LuminoShaderCompiler/detail/UnifiedShader.hpp>
#include "EnvironmentSettings.hpp"
#include "Workspace.hpp"
#include "Project.hpp"
#include "BuildAssetHelper.hpp"
#include "LanguageContext.hpp"

namespace lna {

//==============================================================================
// LanguageContext

LanguageContext::LanguageContext(Project* project)
    : m_project(project) {
}

LanguageContext::~LanguageContext() {
}

ln::Result LanguageContext::build(const ln::String& target) {
    if (!buildAssets()) {
        return ln::err();
    }

    return ln::ok();
}

ln::Result LanguageContext::buildAssets(const ln::Path& intermediateDir, const ln::Path& inputDir, const ln::Path& outputFile) {
    ln::FileSystem::createDirectory(intermediateDir);

    ln::detail::CryptedAssetArchiveWriter writer;
    writer.open(outputFile, ln::detail::CryptedArchiveHelper::DefaultPassword);

    for (auto& file : ln::FileSystem::getFiles(inputDir, ln::StringView(), ln::SearchOption::Recursive)) {
        if (file.hasExtension(_TT(".fx")) || file.hasExtension(_TT(".hlsl"))) {

            ln::Path outputFile;
            if (!BuildAssetHelper::buildShaderFromAutoBuild(intermediateDir, file, &outputFile)) {
                return ln::err();
            }

            const auto relPath = inputDir.makeRelative(file).replaceExtension(ln::detail::UnifiedShader::FileExt);
            CLI::info(ln::format(U"{} > {}", file, relPath));
            writer.addFile(outputFile, relPath);
            CLI::info(file);
        }
        else {
            const auto relPath = inputDir.makeRelative(file);
            CLI::info(ln::format(U"{} > {}", file, relPath));
            writer.addFile(file, relPath);
            CLI::info(file);
        }
    }

    writer.close();
    return ln::ok();
}

ln::Result LanguageContext::buildAssets() const {
    auto outputFilePath = ln::Path(m_project->acquireBuildDir(), _TT("Assets.lca"));
    LN_TRY(buildAssets(m_project->intermediateAssetsDir(), m_project->assetsDir(), outputFilePath));

    // Android
    {
        if (ln::FileSystem::existsDirectory(m_project->androidProjectDir())) {
            auto dst = ln::Path::combine(m_project->androidProjectDir(), _TT("app"), _TT("src"), _TT("main"), _TT("assets"), _TT("Assets.lca"));
            ln::FileSystem::createDirectory(dst.parent());
            ln::FileSystem::copyFile(outputFilePath, dst, ln::FileCopyOption::Overwrite);
            CLI::info(_TT("Copy to ") + dst);
        }
    }

    // macOS
    {
        if (ln::FileSystem::existsDirectory(m_project->macOSProjectDir())) {
            auto dst = ln::Path::combine(m_project->macOSProjectDir(), _TT("LuminoApp.macOS"), _TT("Assets.lca"));
            ln::FileSystem::copyFile(outputFilePath, dst, ln::FileCopyOption::Overwrite);
            CLI::info(_TT("Copy to ") + dst);
        }
    }

    // iOS
    {
        if (ln::FileSystem::existsDirectory(m_project->iOSProjectDir())) {
            auto dst = ln::Path::combine(m_project->iOSProjectDir(), _TT("LuminoApp.iOS"), _TT("Assets.lca"));
            ln::FileSystem::copyFile(outputFilePath, dst, ln::FileCopyOption::Overwrite);
            CLI::info(_TT("Copy to ") + dst);
        }
    }

    // Windows
    {
        if (ln::FileSystem::existsDirectory(m_project->windowsProjectDir())) {
            auto dst = ln::Path::combine(m_project->windowsProjectDir(), _TT("Assets.lca"));
            ln::FileSystem::copyFile(outputFilePath, dst, ln::FileCopyOption::Overwrite);
            CLI::info(_TT("Copy to ") + dst);
        }
    }

    // Web
    {
        auto dstDir = ln::Path::combine(m_project->acquireBuildDir(), _TT("Web"));
        ln::FileSystem::createDirectory(dstDir);

        auto dst = ln::Path::combine(dstDir, _TT("Assets.lca"));
        ln::FileSystem::copyFile(outputFilePath, dst, ln::FileCopyOption::Overwrite);
        CLI::info(_TT("Copy to ") + dst);
    }

    CLI::info(_TT("Compilation succeeded."));

    return ln::ok();
}

//==============================================================================
// CppLanguageContext

CppLanguageContext::CppLanguageContext(Project* project)
    : LanguageContext(project) {
}

CppLanguageContext::~CppLanguageContext() {
}

void CppLanguageContext::restore() {
}

ln::Result CppLanguageContext::build(const ln::String& target) {
    if (!LanguageContext::build(target)) {
        return ln::err();
    }

    if (ln::String::compare(target, _TT("Windows"), ln::CaseSensitivity::CaseInsensitive) == 0) {
        return build_NativeCMakeTarget();
    }
    else if (ln::String::compare(target, _TT("Web"), ln::CaseSensitivity::CaseInsensitive) == 0) {
        return build_WebTarget();
    }

    return ln::ok();
}

ln::Result CppLanguageContext::build_NativeCMakeTarget() const {
    ln::String arch = U"x64-windows";

    ln::List<ln::String> args = {
        project()->rootDirPath(),
        U"-G \"Visual Studio 16 2019\"",
        U"-A x64",
        U"-DCMAKE_TOOLCHAIN_FILE=\"" + ln::Path(APP->buildEnvironment()->vcpkgDir(), U"scripts/buildsystems/vcpkg.cmake").unify() + U"\"",
        U"-DX_VCPKG_APPLOCAL_DEPS_INSTALL=ON",
        U"-DLN_TARGET_ARCH=" + arch,
        U"-DLN_MSVC_STATIC_RUNTIME=ON",
        U"-DCMAKE_PREFIX_PATH=\"" + ln::Path(APP->buildEnvironment()->cmakePackagesDir(), arch).unify() + U"\"",
        //_TT("-DLUMINO_ENGINE_ROOT=\"") + ln::Path(m_project->engineDirPath(), _TT("Native")).str().replace("\\", "/") + _TT("\""),
        //_TT("-DLN_TARGET_ARCH=Emscripten"),
        // cmakeSourceDir,
    };

    // for tool development and debuging.
    //auto& envSettings = project()->workspace()->buildEnvironment();
    //if (lna::Workspace::developMode) {
    //    args.add(ln::format(_TT("-DLUMINO_REPO_ROOT=\"{0}\""), envSettings->engineDevelopmentRepoRootDir().str()));
    //}

    auto buildDir = ln::Path(project()->acquireBuildDir(), arch);
    ln::FileSystem::createDirectory(buildDir);

    ln::Process cmake;
    cmake.setProgram(_TT("cmake"));
    cmake.setArguments(args);
    cmake.setWorkingDirectory(buildDir);
    cmake.start();
    cmake.wait();
    if (cmake.exitCode() != 0) {
        CLI::error(_TT("Failed cmake."));
        return ln::err();
    }

    return ln::ok();
}

ln::Result CppLanguageContext::build_WebTarget() const {
    Workspace* workspace = project()->workspace();

    // emsdk がなければインストールする
    workspace->buildEnvironment()->prepareEmscriptenSdk();

    auto buildDir = ln::Path::combine(project()->acquireBuildDir(), _TT("Web")).canonicalize();
    auto installDir = ln::Path::combine(buildDir, _TT("Release"));
    auto cmakeSourceDir = project()->emscriptenProjectDir();
    auto script = ln::Path::combine(buildDir, _TT("build.bat"));
    auto engineRootPath = ln::Path(workspace->buildEnvironment()->emscriptenSysRootLocalDir(), _TT("LuminoEngine"));

    ln::FileSystem::createDirectory(buildDir);

    {
        auto engineRoot = engineRootPath.str().replace(_TT("\\"), _TT("/"));
        ln::List<ln::String> emcmakeArgs = {
            _TT("-DCMAKE_BUILD_TYPE=Release"),
            _TT("-DCMAKE_INSTALL_PREFIX=") + installDir,
            //_TT("-DCMAKE_PREFIX_PATH=\"") + ln::String::concat(engineRoot, _TT("/"), _TT("Emscripten")) + _TT("\""),
            _TT("-DCMAKE_PREFIX_PATH=\"") + engineRoot + _TT("\""),
            _TT("-DLUMINO_ENGINE_ROOT=\"") + engineRoot + _TT("\""),
            _TT("-DLN_TARGET_ARCH=Emscripten"),
            _TT("-G \"MinGW Makefiles\""),
            cmakeSourceDir,
        };

        ln::StreamWriter sw(script);
        sw.writeLineFormat(_TT("cd /d \"{0}\""), workspace->buildEnvironment()->emsdkDirPath().str());
        sw.writeLineFormat(_TT("call emsdk activate ") + workspace->buildEnvironment()->emsdkName());
        sw.writeLineFormat(_TT("call emsdk_env.bat"));
        sw.writeLineFormat(_TT("cd /d \"{0}\""), buildDir.str());
        sw.writeLineFormat(_TT("call emcmake cmake ") + ln::String::join(emcmakeArgs, _TT(" ")));
        sw.writeLineFormat(_TT("call cmake --build ."));
    }

    ln::Process::execute(script);
    return ln::ok();
}

} // namespace lna
