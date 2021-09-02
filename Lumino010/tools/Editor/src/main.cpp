
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQuick/QQuickView>

#include "Squircle.h"

QQuickWindow *g_window;

class ViewModel : public QObject
{
    Q_OBJECT
public:
    //Q_INVOKABLE QDateTime getCurrentDateTime() const {
    //    return QDateTime::currentDateTime();
    //}

    Q_INVOKABLE void onCompleted()
    {
        auto* c = g_window->openglContext();
        printf("\n");
    }

    Q_INVOKABLE void clicked()
    {
        auto* c = g_window->openglContext();
        printf("\n");
    }
};

//class LSMainWindow : public QQuickWindow
//{
//};

class ApplicationWindowDriver : public QObject
{
    Q_OBJECT
public:
    ApplicationWindowDriver(QQuickWindow* window)
    {
        connect(window, SIGNAL(sceneGraphInitialized()), this, SLOT(onSceneGraphInitialized()), Qt::DirectConnection);
        connect(window, SIGNAL(sceneGraphInvalidated()), this, SLOT(onsceneGraphInvalidated()), Qt::DirectConnection);
    }

protected slots:
    void onSceneGraphInitialized() {
        auto* c = g_window->openglContext();
        printf("");
    }
    void onsceneGraphInvalidated() {
        auto* c = g_window->openglContext();
        printf("");
    }
};

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<Squircle>("OpenGLUnderQML", 1, 0, "Squircle");
    qmlRegisterType<FboInSGRenderer>("SceneGraphRendering", 1, 0, "Renderer");
    //qmlRegisterType<LSMainWindow>("SceneGraphRendering", 1, 0, "LSMainWindow");

    QCoreApplication::addLibraryPath("./");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    //QObject* obj = engine.rootObjects().front();
    //auto* window = dynamic_cast<QQuickWindow*>(obj);
    //window->setresize


    QObject *topLevel = engine.rootObjects().value(0);
    QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);
    g_window = window;
    ViewModel vm;
    engine.rootContext()->setContextProperty("vm", &vm);
    ApplicationWindowDriver driver(window);
    //QQuickView  *view = qobject_cast<QQuickView  *>(topLevel);
    window->show();


    //QOpenGLContext* context = window->openglContext();
    //QOpenGLContext* shareContext = context->shareContext();

    //QOpenGLContext* backgroundContext = new QOpenGLContext(window);

    
    //view.setResizeMode(QQuickView::SizeRootObjectToView);


    //QQuickView view;
    //view.setResizeMode(QQuickView::SizeRootObjectToView);
    //view.setSource(QUrl("qrc:/main.qml"));
    //view.show();

    
	    
    return app.exec();
}

#include "main.moc"

