
#include <QTabWidget>
#include <QOpenGLWidget>
#include "mainwindow.h"
#include "ui_mainwindow.h"

class QOpenGLWidget2 : public QOpenGLWidget
{
public:
    explicit QOpenGLWidget2(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags())
        : QOpenGLWidget(parent, f)
    {

    }

    void initializeGL() override
    {

    }
};

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_documentManager(nullptr)
{
    ui->setupUi(this);

    //new QOpenGLWidget2(this);

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
