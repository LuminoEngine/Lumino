
#include "EnvironmentSettings.hpp"
#include "Project.hpp"
#include "Workspace.hpp"

Workspace::Workspace()
	: m_devTools(ln::makeRef<BuildEnvironment>())
{
	m_devTools->setupPathes();
}

Workspace::~Workspace()
{
}

Result Workspace::openProject(const ln::Path& dir)
{
	m_project = ln::makeRef<Project>(this);
	return m_project->openProject(dir);
}

Result Workspace::runProject(const ln::String& target)
{
	// Windows
	if (ln::String::compare(target, u"Windows", ln::CaseSensitivity::CaseInsensitive) == 0)
	{
		auto exe = ln::FileSystem::getFile(ln::Path(m_project->windowsProjectDir(), u"bin/Debug"), u"*.exe");
		ln::Process::execute(exe);
	}
	// Web
	else if (ln::String::compare(target, u"Web", ln::CaseSensitivity::CaseInsensitive) == 0)
	{
		auto buildDir = ln::Path::combine(m_project->buildDir(), u"Web").canonicalize();

		ln::Process proc;
		proc.setProgram(m_devTools->python2());
		proc.setArguments({u"-m", u"SimpleHTTPServer", u"8000"});
		proc.setWorkingDirectory(buildDir);
		proc.setUseShellExecute(false);
		proc.start();

		{
			auto files = ln::FileSystem::getFiles(buildDir, u"*.html");
			if (files.isEmpty()) {
				CLI::error("Not found *.html file.");
				return Result::Fail;
			}

			ln::Process proc2;
			proc2.setProgram(u"http://localhost:8000/" + (*files.begin()).fileName());
			proc2.start();
		}

		proc.wait();
	}
	else
	{
		CLI::error(ln::String::format(u"{0} is invalid target.", target));
		return Result::Fail;
	}

	return Result::Success;
}

Result Workspace::restoreProject()
{
	m_project->restore();
	return Result::Success;
}

Result Workspace::dev_installTools() const
{
	m_devTools->prepareEmscriptenSdk();


	
	return Result::Success;
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
		auto files = ln::FileSystem::getFiles(ln::Environment::currentDirectory(), u"*.sln");
		if (files.isEmpty()) {
			CLI::error("Not found *.sln file.");
			return;
		}

		ln::Environment::setEnvironmentVariable(u"LUMINO", buildEnvironment()->luminoPackageRootDir());

		ln::Process proc;
		proc.setProgram(*files.begin());
		proc.start();
	}
#elif defined(__APPLE__)
	ln::Process proc;
	proc.setProgram(u"/usr/bin/open");
	proc.setArguments({ u"/Applications/Xcode.app/", ln::Path(m_project->macOSProjectDir(), u"LuminoApp.macOS.xcodeproj") });
	proc.start();
#else
	LN_NOTIMPLEMENTED();	// TODO: putenv は書き込み可能なポインタを渡さないとならないみたい？
#endif
}
