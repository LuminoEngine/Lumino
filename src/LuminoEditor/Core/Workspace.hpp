#pragma once

class BuildEnvironment;
class Project;

class Workspace
	: public ln::RefObject
{
public:
	Workspace();
	virtual ~Workspace();

	ln::Result openProject(const ln::Path& dir);
    ln::Result runProject(const ln::String& target);
    ln::Result restoreProject();

    const ln::Ref<Project>& project() const { return m_project; }
	const ln::Ref<BuildEnvironment>& buildEnvironment() const { return m_devTools; }

    ln::Result dev_installTools() const;
	void dev_openIde(const ln::String& target) const;

private:
	ln::Ref<BuildEnvironment> m_devTools;
	ln::Ref<Project> m_project;
};
