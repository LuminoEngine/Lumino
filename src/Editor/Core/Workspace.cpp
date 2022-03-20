
#include "EnvironmentSettings.hpp"
#include "Project/Project.hpp"
#include "Project/AssetDatabase.hpp"
#include "Project/PluginManager.hpp"
#include "Project/ProjectTemplateManager.hpp"
#include "Workspace.hpp"

namespace lna {

#ifdef LN_DEBUG
bool Workspace::developMode = true;
#else
bool Workspace::developMode = false;
#endif

//ln::Result Workspace::init()
//{
//    assert(!s_instance);
//    s_instance = new Workspace();
//    return true;
//}
//
//void Workspace::finalize()
//{
//    if (s_instance) {
//        delete s_instance;
//        s_instance = nullptr;
//    }
//}

Workspace::Workspace()
	: m_buildEnvironment(ln::makeRef<BuildEnvironment>())
{
	ln::String name = ln::Path(ln::Environment::executablePath()).fileNameWithoutExtension();
	if (name.endsWith(_TT("-rb")))
		m_primaryLang = _TT("ruby");
	else
		m_primaryLang = _TT("cpp");

//#ifdef LN_DEBUG
//	m_buildEnvironment->setupPathes(EnvironmentPathBase::Repository);
//#else
	m_buildEnvironment->setupPathes(developMode);
//#endif

	m_projectTemplateManager = ln::makeObject2<ProjectTemplateManager>();
	m_projectTemplateManager->search();

	
}

Workspace ::~Workspace() {
}

ln::Result Workspace::newMainProject(const ln::Path& projectDir, const ln::String& projectName)
{
    if (LN_REQUIRE(!m_mainProject)) return ln::err();
    m_mainProject = ln::makeRef<Project>(this);
    if (!m_mainProject->newProject(projectDir, projectName, _TT(""), _TT("NativeProject"))) {
        m_mainProject = nullptr;
        return ln::err();
    }
    postMainProjectLoaded();
    return ln::ok();
}

ln::Result Workspace::openMainProject(const ln::Path& filePath)
{
    if (LN_REQUIRE(!m_mainProject)) return ln::err();
    m_mainProject = ln::makeRef<Project>(this);
    if (!m_mainProject->openProject2(filePath)) {
        m_mainProject = nullptr;
        return ln::err();
    }
    postMainProjectLoaded();
    return ln::ok();
}

ln::Result Workspace::closeMainProject()
{
    if (m_mainAssetDatabase) {
        m_mainAssetDatabase->close();
        m_mainAssetDatabase = nullptr;
    }
    if (m_mainProject) {
        m_mainProject->close();
        m_mainProject = nullptr;
    }
    return ln::ok();
}

ln::Result Workspace::runProject(const ln::String& target)
{
	// Windows
	if (ln::String::compare(target, _TT("Windows"), ln::CaseSensitivity::CaseInsensitive) == 0)
	{
		auto exe = ln::FileSystem::getFile(ln::Path::combine(m_mainProject->windowsProjectDir(), _TT("bin"), _TT("x64"), _TT("Debug"), _TT("*.exe")));
		ln::Process::execute(exe);
	}
	// Web
	else if (ln::String::compare(target, _TT("Web"), ln::CaseSensitivity::CaseInsensitive) == 0)
	{
		auto buildDir = ln::Path::combine(m_mainProject->acquireBuildDir(), _TT("Web")).canonicalize();

		ln::String text = _TT(R"(# -*- coding: utf-8 -*-
import http.server
from http.server import HTTPServer, BaseHTTPRequestHandler
import socketserver

PORT = 8000

Handler = http.server.SimpleHTTPRequestHandler
Handler.extensions_map={
    '.wasm': 'application/wasm',
    '.manifest': 'text/cache-manifest',
    '.html': 'text/html',
    '.png': 'image/png',
    '.jpg': 'image/jpg',
    '.svg':	'image/svg+xml',
    '.css':	'text/css',
    '.js': 'application/x-javascript',
    '': 'application/octet-stream', # Default
}
httpd = socketserver.TCPServer(("", PORT), Handler)

print("serving at port", PORT)
httpd.serve_forever()
)");
		auto scriptName = ln::Path(buildDir, _TT("serve.py"));
		ln::FileSystem::writeAllText(scriptName, text);

		ln::Process proc;
		proc.setProgram(m_buildEnvironment->python());
		proc.setArguments({ scriptName });
		proc.setWorkingDirectory(buildDir);
		proc.setUseShellExecute(false);
		proc.start();

		{
			auto files = ln::FileSystem::getFiles(buildDir, _TT("*.html"));
			if (files.isEmpty()) {
				CLI::error(_TT("Not found *.html file."));
				return ln::err();
			}

			ln::Process proc2;
            proc2.setUseShellExecute(true);
			proc2.setProgram(_TT("http://localhost:8000/") + (*files.begin()).fileName());
			proc2.start();
		}

		proc.wait();
	}
	else
	{
		CLI::error(ln::format(_TT("{0} is invalid target."), target));
		return ln::err();
	}

	return ln::ok();
}

ln::Result Workspace::restoreProject()
{
    m_mainProject->restore();
    return ln::ok();
}

ln::Result Workspace::dev_installTools() const
{
	m_buildEnvironment->prepareEmscriptenSdk();
    return ln::ok();
}

void Workspace::dev_openIde(const ln::String& target) const
{
#if defined(_WIN32)
	if (ln::String::compare(target, _TT("Android"), ln::CaseSensitivity::CaseInsensitive) == 0)
	{
		HKEY hKey = NULL;
		LONG lRet = RegOpenKeyExW(HKEY_LOCAL_MACHINE,
			L"SOFTWARE\\Android Studio",
			NULL,
			KEY_READ | KEY_WOW64_64KEY,	// https://stackoverflow.com/questions/252297/why-is-regopenkeyex-returning-error-code-2-on-vista-64bit
			&hKey);
		if (lRet != ERROR_SUCCESS) {
			LN_LOG_ERROR(_TT("Android Studio not installed."));
			return;
		}

		DWORD type, size;
		WCHAR path[MAX_PATH];
		RegQueryValueExW(hKey, L"Path", NULL, &type, (LPBYTE)path, &size);

		ln::Environment::setEnvironmentVariable(_TT("LUMINO"), buildEnvironment()->luminoPackageRootDir());

		ln::Process proc;
		proc.setProgram(ln::Path::combine(ln::String::fromCString(path), _TT("bin"), _TT("studio")));
		proc.setArguments({ m_mainProject->androidProjectDir() });
		proc.start();
	}
	else
	{
		auto files = ln::FileSystem::getFiles(ln::Environment::currentDirectory(), _TT("*.sln"));
		if (files.isEmpty()) {
			CLI::error(_TT("Not found *.sln file."));
			return;
		}

		ln::Environment::setEnvironmentVariable(_TT("LUMINO"), buildEnvironment()->luminoPackageRootDir());

		ln::Process proc;
		proc.setProgram(*files.begin());
		proc.start();
	}
#elif defined(__APPLE__)
	ln::Process proc;
	proc.setProgram(_TT("/usr/bin/open");
	proc.setArguments({ _TT("/Applications/Xcode.app/", ln::Path(m_project->macOSProjectDir(), _TT("LuminoApp.macOS.xcodeproj") });
	proc.start();
#else
	LN_NOTIMPLEMENTED();	// TODO: putenv は書き込み可能なポインタを渡さないとならないみたい？
#endif
}

ln::Path Workspace::findProejctFile(const ln::Path& dir)
{
    auto files = ln::FileSystem::getFiles(dir, _TT("*") + Project::ProjectFileExt);
    if (!files.isEmpty()) {
    	return *files.begin();
    }
    else {
        return ln::Path();
    }
}

void Workspace::postMainProjectLoaded()
{
    m_mainAssetDatabase = ln::makeRef<AssetDatabase>();
    if (!m_mainAssetDatabase->init(m_mainProject)) {
        return;
    }

    m_mainPluginManager = ln::makeRef<PluginManager>();
    if (!m_mainPluginManager->init(m_mainProject)) {
        return;
    }

    m_mainPluginManager->reloadPlugins();
}

} // namespace lna
