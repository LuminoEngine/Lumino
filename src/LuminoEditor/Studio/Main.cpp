#include "MainWindow.h"
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    // QOpenGLContext::globalShareContext() ‚ªŽg‚¦‚é‚æ‚¤‚É‚È‚éB
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

    QApplication::setStyle(QStyleFactory::create("Fusion"));

    QApplication a(argc, argv);

    // apply theme
#if 0
    {
        QFile f(":/light.qss");
        if (!f.exists())
        {
            printf("Unable to set stylesheet, file not found\n");
        }
        else
        {
            f.open(QFile::ReadOnly | QFile::Text);
            QTextStream ts(&f);
            qApp->setStyleSheet(ts.readAll());
        }

        QFont font("Meiryo UI");
        a.setFont(font);
    }
#endif

    MainWindow mainWindow;
    mainWindow.show();
    int exitCode = a.exec();

    return exitCode;
}
