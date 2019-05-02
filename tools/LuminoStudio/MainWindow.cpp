
#include <QTabWidget>
#include <QOpenGLContext>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_documentManager(nullptr)
{
    ui->setupUi(this);

    //QOpenGLContext* co = new QOpenGLContext(this);
    //co->setFormat(requestedFormat());
    //co->create();
    //co->makeCurrent(this);

    m_documentManager = new DocumentManager();

    m_sceneListDock = new SceneListDock(this);
    addDockWidget(Qt::LeftDockWidgetArea, m_sceneListDock);


    setCentralWidget(m_documentManager->rootWidget());
}

MainWindow::~MainWindow()
{
    delete m_documentManager;
    delete ui;
}
