
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

static MainWindow* s_mainWindow = nullptr;

MainWindow* MainWindow::instance()
{
    return s_mainWindow;
}

void MainWindow::initializeLumino()
{
    static bool init = false;
    if (!init) {

        ln::GlobalLogger::addStdErrAdapter();

        ln::EngineSettings::setUserMainWindow(instance()->winId());
        ln::EngineSettings::setDefaultObjectsCreation(false);
        ln::EngineSettings::setUseGLFWWindowSystem(false);
        ln::EngineSettings::setGraphicsContextManagement(false);
        ln::EngineSettings::setExternalMainLoop(false);
        ln::EngineSettings::setExternalRenderingManagement(true);
        ln::EngineSettings::setEngineFeatures(ln::EngineFeature::Experimental);
        ln::Engine::initialize();

        init = true;
    }
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_documentManager(nullptr)
{
    ui->setupUi(this);
    s_mainWindow = this;

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

void MainWindow::closeEvent(QCloseEvent* event)
{
    //QMessageBox::StandardButton resBtn = QMessageBox::question(this, APP_NAME,
    //    tr("Are you sure?\n"),
    //    QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
    //    QMessageBox::Yes);
    //if (resBtn != QMessageBox::Yes) {
    //    event->ignore();
    //}
    //else {
    //    event->accept();
    //}

    // MainWindow close 前に終了しないと、OpenGL コンテキストが先に開放されてしまうのでエラーとなる
    ln::Engine::finalize();
}
