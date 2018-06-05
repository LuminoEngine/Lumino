
#include "EnvironmentSettings.hpp"
#include "Project.hpp"
#include "Workspace.hpp"

Workspace::Workspace()
	: m_environmentSettings(ln::makeRef<EnvironmentSettings>())
{
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

Result Workspace::dev_installTools() const
{
	auto srcIncludeDir = ln::Path::combine(m_environmentSettings->luminoPackageRootPath(), u"Emscripten", u"include");
	auto dstIncludeDir = ln::Path::combine(m_environmentSettings->emscriptenRootPath(), u"system", u"include");
	ln::FileSystem::copyDirectory(srcIncludeDir, dstIncludeDir, true, true);

	auto srcLibDir = ln::Path::combine(m_environmentSettings->luminoPackageRootPath(), u"Emscripten", u"lib");
	auto dstLibDir = ln::Path::combine(m_environmentSettings->emscriptenRootPath(), u"system", u"lib");
	ln::FileSystem::copyDirectory(srcLibDir, dstLibDir, true, true);
	
	return Result::OK;
}
