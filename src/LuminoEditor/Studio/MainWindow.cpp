
#include "External/QtAwesome/QtAwesome.h"
#include "DocumentManager.h"
#include "AudioContentsView.h"
#include "MainWindow.h"

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
{
    s_mainWindow = this;

    setGeometry(0, 0, 1200, 800);

    // init OpenGL
    {
        auto* globalContext = QOpenGLContext::globalShareContext();

        m_resourceContext = new QOpenGLContext();
        m_resourceContext->setFormat(globalContext->format());
        m_resourceContext->setShareContext(globalContext);
        if (!m_resourceContext->create()) {
            qCritical() << "m_resourceContext->create()";
            return;
        }

        m_offscreenSurface = new QOffscreenSurface();
        m_offscreenSurface->setFormat(m_resourceContext->format());
        m_offscreenSurface->create();   // context を使用するスレッドで呼び出す必要がある

        m_resourceContext->makeCurrent(m_offscreenSurface);
        MainWindow::initializeLumino();
        m_resourceContext->doneCurrent();
    }

    m_awesome = new QtAwesome(this);
    m_awesome->initFontAwesome();

    m_contentsViewManager = new ContentsViewManager();
    m_documentManager = new DocumentManager();




    {
        m_contentsViewManager->addContentsViewProvider(ln::makeObject<AudioContentsViewProvider>());
    }

    auto* rootFrame = new QFrame();

    auto* hbox1 = new QHBoxLayout();
    hbox1->setAlignment(Qt::AlignLeft);
    hbox1->setMargin(0);
    hbox1->addWidget(m_contentsViewManager->sidebar());
    rootFrame->setLayout(hbox1);

    QSplitter* split1 = new QSplitter(Qt::Horizontal);
    split1->setStyleSheet("background-color: yellow;");
    split1->setContentsMargins(0, 0, 0, 0);
    split1->addWidget(m_contentsViewManager->viewContainer());
    split1->addWidget(m_documentManager->rootWidget());
    hbox1->addWidget(split1);



    setCentralWidget(rootFrame);
}

MainWindow::~MainWindow()
{
    delete m_documentManager;
    delete m_contentsViewManager;
    delete m_offscreenSurface;
    delete m_resourceContext;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    // MainWindow close 前に終了しないと、OpenGL コンテキストが先に開放されてしまうのでエラーとなる
    m_resourceContext->makeCurrent(m_offscreenSurface);
    ln::Engine::finalize();
    m_resourceContext->doneCurrent();
}
