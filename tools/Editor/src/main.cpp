#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

class ViewModel : public QObject
{
    Q_OBJECT
public:
    //Q_INVOKABLE QDateTime getCurrentDateTime() const {
    //    return QDateTime::currentDateTime();
    //}

    Q_INVOKABLE void clicked()
    {
        printf("\n");
    }
};

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QCoreApplication::addLibraryPath("./");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QObject* obj = engine.rootObjects().front();

    ViewModel vm;
    engine.rootContext()->setContextProperty("vm", &vm);


	    
    return app.exec();
}

#include "main.moc"

