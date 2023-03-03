
#include "Workspace.hpp"
#include "EnvironmentSettings.hpp"
#include "../../../lumino/LuminoEngine/src/Engine/EngineDomain.hpp"
#include "../../../lumino/LuminoEngine/src/Engine/EngineManager.hpp"

namespace lna {

//==============================================================================
// BuildEnvironment

BuildEnvironment::BuildEnvironment()
    : m_toolsDir()
    , m_emsdkVer()
    , m_emsdkRootDir()
    , m_emscriptenRootDir()
    , m_engineDevelopmentRepoRootDir() {
#if defined(LN_OS_WIN32)
    m_defaultTargetName = _TT("Windows");
#elif defined(LN_OS_MAC)
    m_defaultTargetName = _TT("macOS");
#else
    static_assert("Target not supported.");
#endif

    m_defaultLanguage = Language::Cpp;
    if (ln::Environment::executablePath().str().endsWith(_TT("-rb"))) {
    	m_defaultLanguage = Language::Ruby;
    }
}

void BuildEnvironment::setupPathes(bool developMode) {
    // Make package paths.
    {
        if (developMode) {
            // Visual Studio など IDE 上からデバッグする用。
            // 先に build.csproj を実行して NativePackage を作っておく必要がある。
            m_engineDevelopmentRepoRootDir = findRepositoryRootDir();
            if (!m_engineDevelopmentRepoRootDir.isEmpty()) {
                // TODO: 仮
                m_luminoPackageRootDir = ln::Path::combine(m_engineDevelopmentRepoRootDir, U"build", U"installed");
                m_projectTemplatesDirPath = ln::Path::combine(m_engineDevelopmentRepoRootDir, U"tools", U"ProjectTemplates");
            
                m_cmakePackagesDir = ln::Path::combine(m_engineDevelopmentRepoRootDir, U"build", U"installed");
                m_vcpkgDir = ln::Path::combine(m_engineDevelopmentRepoRootDir, U"build", U"tools", U"vcpkg");
                m_ninja = ln::Path::combine(m_engineDevelopmentRepoRootDir, U"build", U"tools", U"ninja-win", U"ninja.exe");
            }
        }

        // 実行ファイルからディレクトリを親へ辿って、Package の Root を見つける。
        if (m_luminoPackageRootDir.isEmpty()) {
            m_luminoPackageRootDir = findNativePackageRootDir();
        }

        if (m_projectTemplatesDirPath.isEmpty()) {
            m_projectTemplatesDirPath = ln::Path::combine(m_luminoPackageRootDir, _TT("Tools"), _TT("ProjectTemplates"));
        }

        m_luminoPackageEngineDir = ln::Path::combine(m_luminoPackageRootDir, _TT("Engine"));

        // Validation
        {
            if (m_luminoPackageRootDir.isEmpty()) {
                CLI::fatal(_TT("Not found lumino package root directory."));
                return;
            }
            if (!ln::FileSystem::existsDirectory(m_projectTemplatesDirPath)) {
                CLI::fatal(_TT("Not found 'ProjectTemplatesDir' directory."));
                return;
            }
        }

        CLI::verbose(ln::format(_TT("PackageRootDir: {0}"), m_luminoPackageRootDir.str()));
        CLI::verbose(ln::format(_TT("ProjectTemplatesDir: {0}"), m_projectTemplatesDirPath.str()));
    }

#if 0
    // Find package root
    {
        m_luminoPackageRootDir.clear();

		switch (pathBase)
		{
			//case EnvironmentPathBase::EnvironmentVariable:
			//{
			//	auto path = ln::Environment::getEnvironmentVariable(_TT("LUMINO_PATH");
			//	if (path) {
			//		m_luminoPackageRootDir = *path;
			//		setupPathesFromPackageRoot(m_luminoPackageRootDir);
			//	}
			//	m_actualPathBase = EnvironmentPathBase::EnvironmentVariable;
			//	break;
			//}
			case EnvironmentPathBase::LocalPackage:
			{
				//m_luminoPackageRootDir = findNativePackageRootDir();

#ifdef LN_DEBUG
				if (m_luminoPackageRootDir.isEmpty()) {
					m_luminoPackageRootDir = findLocalPackageForTesting();
				}
#endif
				setupPathesFromPackageRoot(m_luminoPackageRootDir);
				m_actualPathBase = EnvironmentPathBase::LocalPackage;
				break;
			}
			case EnvironmentPathBase::Repository:
			{
				m_luminoPackageRootDir = ln::detail::EngineManager::findRepositoryRootForTesting();
				m_actualPathBase = EnvironmentPathBase::Repository;
				break;
			}
			default:
				LN_UNREACHABLE();
				break;
		}

        // ここまでで見つからなかったらエラー
        if (m_luminoPackageRootDir.isEmpty())
        {
            CLI::fatal(_TT("Not found lumino package root directory.");
			return;
        }

		setupPathesFromRepositoryRoot(m_luminoPackageRootDir);

		// Validation
		{
			if (!ln::FileSystem::existsDirectory(m_projectTemplatesDirPath)) {
				CLI::fatal(_TT("Not found 'projectTemplatesDirPath' directory.");
				return;
			}
		}

		CLI::verbose(ln::String::format(_TT("PackageRootDir: {0}", m_luminoPackageRootDir));
		CLI::verbose(ln::String::format(_TT("ProjectTemplatesDir: {0}", m_projectTemplatesDirPath));
    }

#endif

    m_appDataDirPath = ln::Path::combine(ln::Environment::specialFolderPath(ln::SpecialFolder::ApplicationData), _TT("Lumino"));
    m_toolsDir = (ln::Path::combine(m_appDataDirPath, _TT("BuildTools")));

    // Emscripten
    {
        m_emsdkVer = _TT("3.1.9");
        m_emsdkName = _TT("3.1.9");
    }

    // Android
    {
#ifdef _WIN32
        PWSTR path = NULL;
        HRESULT hr = ::SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &path);
        if (SUCCEEDED(hr)) {
            m_androidSdkRootDir = ln::Path::combine(ln::String::fromCString(path), _TT("Android"), _TT("Sdk"));
            m_androidSdkCMake = ln::Path::combine(m_androidSdkRootDir, _TT("cmake/3.6.4111459/bin/cmake.exe"));
            m_androidSdkNinja = ln::Path::combine(m_androidSdkRootDir, _TT("cmake/3.6.4111459/bin/ninja.exe"));
            m_androidNdkRootDir = ln::Path::combine(m_androidSdkRootDir, _TT("ndk-bundle"));
            m_androidCMakeToolchain = ln::Path::combine(m_androidNdkRootDir, _TT("build/cmake/android.toolchain.cmake"));

            CoTaskMemFree(path);
        }
#endif
    }

#if 0 // Find msbuild
	{
#ifdef LN_OS_WIN32
		ln::String result;
		if (ln::Process::execute(ln::Path(luminoPackageToolsDir(), _TT("vswhere"), {_TT("-format", _TT("json"}, &result) != 0) {
			CLI::error("Failed vswhere.");
			return;
		}

		ln::JsonDocument json;
		json.parse(result);
		auto* obj = static_cast<ln::JsonObject*>(static_cast<ln::JsonArray*>(json.rootElement())->element(0));
		auto* value = static_cast<ln::JsonValue*>(obj->find(_TT("installationPath"));

		auto path = ln::Path(value->stringValue(), _TT("MSBuild\\15.0\\Bin\\MSBuild.exe");
		if (ln::FileSystem::existsFile(path)) {
			m_msbuild = path;
		}
		else {
			CLI::warning("MSBuild not found.");
		}
#endif
	}
#endif
}

ln::Result<> BuildEnvironment::prepareEmscriptenSdk() {

    if (engineDevelopmentMode()) {
        m_emsdkRootDir = ln::Path::combine(m_engineDevelopmentRepoRootDir, _TT("build"), _TT("tools"), _TT("emsdk"));
        m_emscriptenRootDir = ln::Path::combine(m_emsdkRootDir, _TT("upstream"), _TT("emscripten"));
        m_emscriptenSysRoot = ln::Path::combine(m_emscriptenRootDir, U"cache", _TT("sysroot"));

        //auto dst = ln::Path::combine(m_emscriptenSysRoot);
        auto dst = ln::Path::combine(m_vcpkgDir, U"installed", U"wasm32-emscripten");
        //if (ln::FileSystem::existsDirectory(engineRoot)) {
        auto src = ln::Path::combine(m_cmakePackagesDir, _TT("wasm32-emscripten"));
        CLI::info(ln::format(_TT("Copy Engine '{0}' to '{1}'"), src.str(), dst.str()));
        ln::FileSystem::copyDirectory(src, dst, true, true);
        //}
    }
    else {

        if (m_emsdkRootDir.isEmpty()) {
            m_emsdkRootDir = ln::Path::combine(m_toolsDir, _TT("emsdk"));
        }

        ln::FileSystem::createDirectory(m_emsdkRootDir);

        m_emscriptenRootDir = ln::Path::combine(m_emsdkRootDir, _TT("upstream"), _TT("emscripten"));
        //m_emscriptenSysRootLocal = ln::Path::combine(m_emscriptenRootDir, _TT("system"), _TT("local"));
        // m_python = ln::Path::combine(m_emsdkRootDir, _TT("python"), _TT("3.7.4_64bit"), _TT("python"));

        if (!ln::FileSystem::existsDirectory(m_emsdkRootDir)) {
            if (!callProcess(_TT("git"), { _TT("clone"), _TT("https://github.com/juj/emsdk.git") }, m_toolsDir)) {
                return ln::err();
            }
        }

        if (!ln::FileSystem::existsDirectory(m_emscriptenRootDir)) {
            CLI::info(ln::format(_TT("Setup emscripten : {0}"), m_emsdkName));

#ifdef LN_OS_WIN32
            auto emsdk = ln::Path(m_emsdkRootDir, _TT("emsdk.bat")); // TODO: process クラス内で path 結合
#else
            auto emsdk = _TT("emsdk");
#endif
            if (!callProcess(_TT("git"), { _TT("pull") }, m_emsdkRootDir)) {
                return ln::err();
            }
            if (!callProcess(emsdk, { _TT("update-tags") }, m_emsdkRootDir)) {
                return ln::err();
            }
            if (!callProcess(emsdk, { _TT("install"), m_emsdkName }, m_emsdkRootDir)) {
                return ln::err();
            }
        }


        //CLI::verbose(ln::format(_TT("EmscriptenSysLocal: {0}"), m_emscriptenSysRootLocal.str()));

        //if (!ln::FileSystem::existsDirectory(m_emscriptenSysRootLocal)) {
        //    CLI::fatal(_TT("Not found 'EmscriptenSysLocal' directory."));
        //    return ln::err();
        //}
    }

    // Find python
    {
        auto files = ln::FileSystem::getFiles(ln::Path(m_emsdkRootDir, U"python"), U"python.exe", ln::SearchOption::Recursive);
        if (!files.isEmpty()) {
            m_python = *files.begin();
        }
        else {
            LN_ERROR(U"Not found python executable in emsdk.");
            return ln::err();
        }
    }

    CLI::verbose(ln::format(_TT("EmsdkRootDir: {0}"), m_emsdkRootDir.str()));
    CLI::verbose(ln::format(_TT("EmscriptenRootDir: {0}"), m_emscriptenRootDir.str()));

    {
        auto cd = ScopedCurrentDir::enter(m_emsdkRootDir);
        ln::String logs;
        ln::Process2::exec(U"cmd /c emsdk activate " + m_emsdkName, nullptr, &logs);
        ln::StringReader reader(logs);

        ln::Array<ln::String> var_PATH;
        ln::String var_EMSDK;
        ln::String var_EM_CONFIG;
        ln::String var_EMSDK_NODE;
        ln::String var_EMSDK_PYTHON;
        ln::String var_JAVA_HOME;
        ln::String line;
        while (reader.readLine(&line)) {
            if (line.startsWith(U"PATH +=")) {
                var_PATH.push(ln::String(line.substr(7)).trim());
            }
            else if (line.startsWith(U"EMSDK =")) {
                var_EMSDK = ln::String(line.substr(7)).trim();  // TODO: StringView::trim()
            }
            else if (line.startsWith(U"EM_CONFIG =")) {
                var_EM_CONFIG = ln::String(line.substr(11)).trim();
            }
            else if (line.startsWith(U"EMSDK_NODE =")) {
                var_EMSDK_NODE = ln::String(line.substr(12)).trim();
            }
            else if (line.startsWith(U"EMSDK_PYTHON")) {
                int i = line.indexOf(U"=");
                var_EMSDK_PYTHON = ln::String(line.substr(i + 1)).trim();
            }
            else if (line.startsWith(U"JAVA_HOME =")) {
                var_JAVA_HOME = ln::String(line.substr(11)).trim();
            }
        }

        auto path = ln::Environment::getEnvironmentVariable(U"PATH").value();
        path = ln::String::join(var_PATH, U";") + U";" + path;
        ln::Environment::setEnvironmentVariable(U"PATH", path);
        ln::Environment::setEnvironmentVariable(U"EMSDK", var_EMSDK);
        ln::Environment::setEnvironmentVariable(U"EM_CONFIG", var_EM_CONFIG);
        ln::Environment::setEnvironmentVariable(U"EMSDK_NODE", var_EMSDK_NODE);
        ln::Environment::setEnvironmentVariable(U"EMSDK_PYTHON", var_EMSDK_PYTHON);
        ln::Environment::setEnvironmentVariable(U"JAVA_HOME", var_JAVA_HOME);
    }

    

    return ln::ok();
}

ln::Path BuildEnvironment::findLocalPackageForTesting() {
    ln::Path result;

    // デバッグ用。実行ファイルの位置からさかのぼっていって、build.csproj が見つかればそこから必要なパスを作ってみる
    {
        CLI::info(_TT("Using debug mode build environment pathes."));

        auto luminoRepoRoot = ln::detail::EngineManager::findRepositoryRootForTesting();
        LN_LOG_DEBUG(luminoRepoRoot);

        result = ln::Path::combine(luminoRepoRoot, _TT("build"), _TT("NativePackage"));
    }

    return result;
}

//void BuildEnvironment::setupPathesFromPackageRoot(const ln::Path& packageRoot)
//{
//	// Setup Basic paths
//	{
//		m_luminoPackageToolsDir = ln::Path(m_luminoPackageRootDir, _TT("Tools");
//		m_projectTemplatesDirPath = ln::Path(m_luminoPackageToolsDir, _TT("Templates");
//
//		CLI::info(_TT("luminoPackageRootDir: " + m_luminoPackageRootDir);
//		CLI::info(_TT("luminoPackageToolsDir: " + m_luminoPackageToolsDir);
//		CLI::info(_TT("projectTemplatesDirPath: " + m_projectTemplatesDirPath);
//	}
//
//	// LUMINO_PATH
//	{
//		auto v = ln::Environment::getEnvironmentVariable(_TT("LUMINO_PATH");
//		if (v.hasValue()) {
//			CLI::info(ln::String::format(_TT("LUMINO_PATH: {0}", v.value()));
//		}
//		else {
//			CLI::warning(ln::String::format(_TT("LUMINO_PATH is unset. Use local package instead."));
//			ln::Environment::setEnvironmentVariable(_TT("LUMINO_PATH", m_luminoPackageRootDir);
//			CLI::info(ln::String::format(_TT("LUMINO_PATH: {0}", m_luminoPackageRootDir));
//		}
//	}
//}

//void BuildEnvironment::setupPathesFromRepositoryRoot(const ln::Path& repoRoot)
//{
//	m_projectTemplatesDirPath = ln::Path::combine(repoRoot, _TT("Tools", _TT("ProjectTemplates");
//}

ln::Result<> BuildEnvironment::callProcess(const ln::String& program, const ln::Array<ln::String>& arguments, const ln::Path& workingDir) {
    auto proc = ln::ProcessCommand2(program)
        .args(arguments)
        .workingDirectory(workingDir)
        .start();

    proc->wait();
    if (proc->exitCode() == 0) {
        return ln::ok();
    }
    else {
        return ln::err();
    }
}

ln::Path BuildEnvironment::findNativePackageRootDir() const {
    // 実行ファイルを少しさかのぼってパッケージのルートらしいフォルダがあればそこを採用
    ln::Path packageRootDir = ln::Path(ln::Environment::executablePath()).parent().parent();

    if (m_defaultLanguage == Language::Cpp) {
        ln::Path engineDir = ln::Path(packageRootDir, _TT("Engine"));
        ln::Path toolsDir = ln::Path(packageRootDir, _TT("Tools"));
        if (ln::FileSystem::existsDirectory(engineDir) && ln::FileSystem::existsDirectory(toolsDir)) {
            return packageRootDir;
        }
    }
    if (m_defaultLanguage == Language::Ruby) {
        ln::Path toolsDir = ln::Path(packageRootDir, _TT("Tools"));
        if (ln::FileSystem::existsDirectory(toolsDir)) {
            return packageRootDir;
        }
    }

    return ln::Path();
}

ln::Path BuildEnvironment::findRepositoryRootDir() {
    return ln::detail::EngineManager::findRepositoryRootForTesting();
}

} // namespace lna
