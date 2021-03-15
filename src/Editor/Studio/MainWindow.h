#pragma once
#include <QMainWindow>
#include "ContentsViewManager.h"

namespace lna { class Workspace; }
class QtAwesome;
class ActionManager;
class DocumentManager;
class AssetBrowserContentsViewProvider;
class SpritesetContentsViewProvider;
class AudioContentsViewProvider;
class InspectorPaneContainer;
class ToolPaneContainer;
class ProblemsPane;
class OutputPane;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow* instance();
    static void initializeLumino();

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QtAwesome* awesome() const { return m_awesome; }
    DocumentManager* documentManager() const { return m_documentManager; }

    void importFile(QString filePath);
    void openFile(QString filePath);

protected:
    void closeEvent(QCloseEvent* event) override;

	void onNewProject();
	void onOpenProject();

private:
    void closeProject();

    ln::Ref<lna::Workspace> m_workspace;

    QOpenGLContext* m_resourceContext;
    QOffscreenSurface* m_offscreenSurface;
    QtAwesome* m_awesome;
    ContentsViewManager* m_contentsViewManager;
	ActionManager* m_actionManager;
    DocumentManager* m_documentManager;

    AssetBrowserContentsViewProvider* m_assetBrowserContentsViewProvider;
    SpritesetContentsViewProvider* m_spritesetContentsViewProvider;
    AudioContentsViewProvider* m_audioContentsViewProvider;

	InspectorPaneContainer* m_inspectorPaneContainer;
	ToolPaneContainer* m_toolPaneContainer;
	ProblemsPane* m_problemsPane;
	OutputPane* m_outputPane;
};
