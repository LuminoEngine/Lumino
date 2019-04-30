#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include "DocumentManager.h"


#include <QOpenGLFunctions>
#include <QOpenGLWidget>

class MyOpenGLWidget
    : public QOpenGLWidget
    , protected QOpenGLFunctions
{
    Q_OBJECT

public:
    MyOpenGLWidget(QWidget *parent);
    ~MyOpenGLWidget();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    // OpenGL の API を呼ぶ前に呼ぶ。
    //initializeOpenGLFunctions();
}

MyOpenGLWidget::~MyOpenGLWidget()
{
}

/**
 * @brief 初期化処理を記述する。
 */
void MyOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
}

/**
 * @brief リサイズされた際の処理を記述する。
 */
void MyOpenGLWidget::resizeGL(int width, int height)
{
    if (height == 0) height = 1;
    glViewport(0, 0, width, height);
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //qgluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
}

/**
 * @brief 描画する際の処理を記述する。
 */
void MyOpenGLWidget::paintGL()
{
    glClearColor(0.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
    glColor3d(1.f, 0.f, 0.f);  // 赤色
    glVertex3d(0.f, 0.8f, 0.f);
    glVertex3d(-0.8f, 0.f, 0.f);
    glVertex3d(0.8f, 0.f, 0.f);
    glEnd();
}


DocumentManager::DocumentManager(QObject* parent)
    : QObject(parent)
    , m_rootWidget(new QWidget())
    , m_tabWidget(new QTabWidget(m_rootWidget))
{
    QVBoxLayout* vertical = new QVBoxLayout(m_rootWidget);
    vertical->addWidget(m_tabWidget);
    vertical->setMargin(0);
    vertical->setSpacing(0);


    //QTextEdit* editor1 = new QTextEdit(tr("1つめのタブ"), m_tabWidget);
    QTextEdit* editor2 = new QTextEdit(tr("2つめのタブ"), m_tabWidget);
    m_tabWidget->addTab(new MyOpenGLWidget(m_tabWidget), tr("TAB1"));
    m_tabWidget->addTab(editor2, tr("TAB2"));
    m_tabWidget->setTabsClosable(true);
}

DocumentManager::~DocumentManager()
{
    delete m_tabWidget;
}

#include "DocumentManager.moc"
