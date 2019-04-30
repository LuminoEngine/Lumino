
#include <QTabWidget>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
    , m_documentManager(new DocumentManager())
{
    ui->setupUi(this);

    m_sceneListDock = new SceneListDock(this);
    addDockWidget(Qt::LeftDockWidgetArea, m_sceneListDock);


    setCentralWidget(m_documentManager->rootWidget());
}

MainWindow::~MainWindow()
{
    delete m_documentManager;
    delete ui;
}
