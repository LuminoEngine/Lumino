#pragma once
#include <Workspace.hpp>

class MainWindow;

class EditorApplication
    : public ln::Application
{
public:
    static Ref<ln::UICommand> NewCommand;
    static Ref<ln::UICommand> OpenCommand;


    static EditorApplication* instance();

    EditorApplication();
    ~EditorApplication();
    ln::Result init();
    void dispose();
    void run();
	void setMainWindow(ln::UIMainWindow* window);	// onInit でのみ可

    MainWindow* mainWindow() const;
    const ln::Ref<lna::Workspace>& workspace() const { return m_workspace; }

    void importFile(const ln::Path& filePath);
    void openAssetFile(const ln::Path& filePath);

protected:
    virtual void onInit() override;

private:
    void closeProject();

    void onNewProject(ln::UICommandEventArgs* e);
    void onOpenProject(ln::UICommandEventArgs* e);

    ln::Ref<lna::Workspace> m_workspace;
};

