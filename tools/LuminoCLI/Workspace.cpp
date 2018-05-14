
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
