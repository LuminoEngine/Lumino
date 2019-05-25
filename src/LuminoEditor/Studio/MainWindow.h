#pragma once
#include <QMainWindow>
#include "ContentsViewManager.h"

class QtAwesome;
class DocumentManager;
class InspectorViewPane;
class ToolViewPane;

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
    InspectorViewPane* m_inspectorViewPane;
    ToolViewPane* m_toolViewPane;
};
