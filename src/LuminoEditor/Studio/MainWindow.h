#pragma once
#include <QMainWindow>
#include "ContentsViewManager.h"

class QtAwesome;
class DocumentManager;
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

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    QOpenGLContext* m_resourceContext;
    QOffscreenSurface* m_offscreenSurface;
    QtAwesome* m_awesome;
    ContentsViewManager* m_contentsViewManager;
    DocumentManager* m_documentManager;
	InspectorPaneContainer* m_inspectorPaneContainer;
	ToolPaneContainer* m_toolPaneContainer;
	ProblemsPane* m_problemsPane;
	OutputPane* m_outputPane;
};
