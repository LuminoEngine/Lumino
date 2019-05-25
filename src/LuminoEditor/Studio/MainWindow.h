#pragma once
#include <QMainWindow>
#include "ContentsViewManager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow* instance();
    static void initializeLumino();

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    QOpenGLContext* m_resourceContext;
    QOffscreenSurface* m_offscreenSurface;
    ContentsViewManager* m_contentsViewManager;
};
