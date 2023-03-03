#pragma once
#include <Workspace.hpp>
#include "EditorContext.hpp"

class MainWindow;

class EditorApplication
    : public ln::Application
{
public:
    static Ref<ln::UICommand> NewCommand;
    static Ref<ln::UICommand> OpenCommand;
    static Ref<ln::UICommand> SaveCommand;


    static EditorApplication* instance();
    static void configure();

    EditorApplication();
    ~EditorApplication();
    ln::Result<> init();
    void dispose();
    void run();

    MainWindow* mainWindow() const;
    const ln::Ref<lna::Workspace>& workspace() const { return m_workspace; }
    lna::Project* mainProject() const;
	const ln::Ref<lna::EditorContext>& editorContext() const { return m_editorContext; }

	void newProject(const ln::Path& dirPath, const ln::String& projectName);
    void openProject(const ln::Path& filePath);
    bool closeProject();
    //void importFile(const ln::Path& filePath);
    void openAssetFile(const ln::Path& filePath);

protected:
    void onInit() override;
    void onRoutedEvent(ln::UIEventArgs* e) override;

private:
	void postProjectLoaded();
    void handleNewProject(ln::UICommandEventArgs* e);
    void handleOpenProject(ln::UICommandEventArgs* e);

    Ref<lna::Workspace> m_workspace;
    Ref<lna::EditorContext> m_editorContext;
};

