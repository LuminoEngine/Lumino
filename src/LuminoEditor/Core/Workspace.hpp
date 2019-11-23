#pragma once

namespace lna {

class BuildEnvironment;
class Project;

class Workspace
    : public ln::Object
{
public:
    static Workspace* instance();
	Workspace();
	virtual ~Workspace();

	ln::Result newMainProject(const ln::Path& projectDir, const ln::String& projectName);
	ln::Result openMainProject(const ln::Path& filePath);
	ln::Result closeMainProject();
    ln::Result runProject(const ln::String& target);
    ln::Result restoreProject();

    const ln::Ref<Project>& project() const { return m_project; }
	const ln::Ref<BuildEnvironment>& buildEnvironment() const { return m_buildEnvironment; }

    ln::Result dev_installTools() const;
	void dev_openIde(const ln::String& target) const;

    static ln::Path findProejctFile(const ln::Path& dir);

private:

	ln::Ref<BuildEnvironment> m_buildEnvironment;
	ln::Ref<Project> m_project;
};

} // namespace lna
