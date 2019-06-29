#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // apply theme
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


    //QSurfaceFormat format;
    //format.setSamples(16);
    //format.setDepthBufferSize(24);
    //format.setVersion(3, 0);
    //format.setProfile(QSurfaceFormat::CoreProfile);
    //QSurfaceFormat::setDefaultFormat(format);


    MainWindow mainWindow;



    mainWindow.show();
    int exitCode = a.exec();

    return exitCode;
}
