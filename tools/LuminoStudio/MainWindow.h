#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "DocumentManager.h"
#include "SceneListDock.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    SceneListDock* m_sceneListDock;
    DocumentManager* m_documentManager;

};

#endif // MAINWINDOW_H
