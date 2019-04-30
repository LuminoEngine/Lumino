#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_sceneListDock = new SceneListDock(this);
    addDockWidget(Qt::LeftDockWidgetArea, m_sceneListDock);
}

MainWindow::~MainWindow()
{
    delete ui;
}
