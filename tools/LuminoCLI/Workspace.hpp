#pragma once

class EnvironmentSettings;
class DevTools;
class Project;

class Workspace
	: public ln::RefObject
{
public:
	Workspace();
	virtual ~Workspace();

	Result newProject(const ln::Path& dir, const ln::String& projectName);
	Result openProject(const ln::Path& dir);
	Result buildProject();

	const Ref<EnvironmentSettings>& environmentSettings() const { return m_environmentSettings; }

	Result dev_installTools() const;

private:
	Ref<EnvironmentSettings> m_environmentSettings;
	Ref<DevTools> m_devTools;
	Ref<Project> m_project;
};
