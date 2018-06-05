#pragma once

class EnvironmentSettings;
class Project;

class Workspace
	: public ln::RefObject
{
public:
	Workspace();
	virtual ~Workspace();

	Result newProject(const ln::Path& dir, const ln::String& projectName);
	Result openProject(const ln::Path& dir);

	const Ref<EnvironmentSettings>& environmentSettings() const { return m_environmentSettings; }

	Result dev_installTools() const;

private:
	Ref<EnvironmentSettings> m_environmentSettings;
	Ref<Project> m_project;
};
