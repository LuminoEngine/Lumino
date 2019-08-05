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
    //static ln::Result init();
    //static void finalize();

	ln::Result openProject2(const ln::Path& filePath);
    ln::Result runProject(const ln::String& target);
    ln::Result restoreProject();

    const ln::Ref<Project>& project() const { return m_project; }
	const ln::Ref<BuildEnvironment>& buildEnvironment() const { return m_devTools; }





    ln::Result dev_installTools() const;
	void dev_openIde(const ln::String& target) const;

    static ln::Path findProejctFile(const ln::Path& dir);

private:

	ln::Ref<BuildEnvironment> m_devTools;
	ln::Ref<Project> m_project;
};

} // namespace lna
