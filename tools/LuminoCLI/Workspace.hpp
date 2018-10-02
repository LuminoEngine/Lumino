﻿#pragma once

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
	void restoreProject();

	const Ref<EnvironmentSettings>& environmentSettings() const { return m_environmentSettings; }
	const Ref<BuildEnvironment>& buildEnvironment() const { return m_devTools; }

	Result dev_installTools() const;
	void dev_openIde(const ln::String& target) const;

private:
	Ref<EnvironmentSettings> m_environmentSettings;
	Ref<BuildEnvironment> m_devTools;
	Ref<Project> m_project;
};
