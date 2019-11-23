#pragma once

namespace lna {

class BuildEnvironment;
class Project;
class PluginManager;
class AssetDatabase;

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

    const ln::Ref<Project>& mainProject() const { return m_mainProject; }
    const ln::Ref<PluginManager>& mainPluginManager() const { return m_mainPluginManager; }
    const ln::Ref<AssetDatabase>& mainAssetDatabase() const { return m_mainAssetDatabase; }
	const ln::Ref<BuildEnvironment>& buildEnvironment() const { return m_buildEnvironment; }

    ln::Result dev_installTools() const;
	void dev_openIde(const ln::String& target) const;

    static ln::Path findProejctFile(const ln::Path& dir);

private:
    void postMainProjectLoaded();

	ln::Ref<BuildEnvironment> m_buildEnvironment;
	ln::Ref<Project> m_mainProject;
    ln::Ref<PluginManager> m_mainPluginManager;
    ln::Ref<AssetDatabase> m_mainAssetDatabase;
};

} // namespace lna
