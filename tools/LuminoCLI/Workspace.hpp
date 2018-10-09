#pragma once

class EnvironmentSettings;
class BuildEnvironment;
class Project;

class Workspace
	: public ln::RefObject
{
public:
	Workspace();
	virtual ~Workspace();

	Result newProject(const ln::Path& projectDir, const ln::String& projectName);
	Result openProject(const ln::Path& dir);
	Result buildProject(const ln::String& target);
	Result runProject(const ln::String& target);
	Result restoreProject();

	const ln::Ref<EnvironmentSettings>& environmentSettings() const { return m_environmentSettings; }
	const ln::Ref<BuildEnvironment>& buildEnvironment() const { return m_devTools; }

	Result dev_installTools() const;
	void dev_openIde(const ln::String& target) const;

private:
	ln::Ref<EnvironmentSettings> m_environmentSettings;
	ln::Ref<BuildEnvironment> m_devTools;
	ln::Ref<Project> m_project;
};
