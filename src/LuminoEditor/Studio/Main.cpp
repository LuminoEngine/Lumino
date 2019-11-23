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

    // QOpenGLContext::globalShareContext() が使えるようになる。
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

    }
#endif

	// Global font
	{
		QFont font("Meiryo UI");
		font.setPointSize(10);
		a.setFont(font);
	}

    MainWindow mainWindow;
    mainWindow.show();
    int exitCode = a.exec();

    // show しただけでメモリリークする。ずっと昔から問題があるようだ。
    // https://stackoverflow.com/questions/22521946/qt-is-there-any-memory-leak-in-the-following-code
    /*
    Detected memory leaks!
Dumping objects ->
{3085} normal block at 0x0CC822F0, 32 bytes long.
 Data: <                > 01 00 00 00 01 00 00 00 04 00 00 00 10 00 00 00 
{2956} normal block at 0x0CC61998, 32 bytes long.
 Data: <                > 01 00 00 00 01 00 00 00 02 00 00 00 10 00 00 00 
{2955} normal block at 0x0CD4BB30, 20 bytes long.
 Data: <                > 98 19 C6 0C 00 01 CD CD 00 00 00 00 00 00 00 00 
{2954} normal block at 0x001B3460, 44 bytes long.
 Data: <`       (       > 60 C5 D4 0C 00 00 00 00 28 8E D5 0C 00 00 00 00 
{2946} normal block at 0x0CC38DA8, 64 bytes long.
 Data: < g f`       ,  f> DC 67 B1 66 60 C5 D4 0C 00 00 00 00 2C F9 FD 66 
{2945} normal block at 0x0CD4C560, 28 bytes long.
 Data: <  ,           , > D4 DE 2C 0F A8 8D C3 0C 00 00 00 80 10 DF 2C 0F 
{2877} normal block at 0x0CC374B8, 64 bytes long.
 Data: <                > 01 00 00 00 00 00 00 00 04 00 00 00 10 00 00 00 
{309} normal block at 0x001969E8, 112 bytes long.
 Data: <   f        ,  f> A0 B5 B3 66 88 11 1B 00 00 00 00 00 2C F9 FD 66 
{308} normal block at 0x001B1188, 8 bytes long.
 Data: <   f i  > AC B5 B3 66 E8 69 19 00 
{307} normal block at 0x001879F0, 68 bytes long.
 Data: <            `  f> 01 00 00 00 00 00 00 00 00 00 00 00 60 ED FD 66 
Object dump complete.
*/
    return exitCode;
}
