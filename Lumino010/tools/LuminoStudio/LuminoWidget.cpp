#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QTextEdit>

#include "MainWindow.h"
#include "LuminoWidget.h"

LuminoWidget::LuminoWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

LuminoWidget::~LuminoWidget()
{
}

void LuminoWidget::initializeGL()
{
    MainWindow::initializeLumino();

    m_frameWindow = ln::newObject<ln::UINativeFrameWindow>();
}

void LuminoWidget::resizeGL(int width, int height)
{
    if (height == 0) height = 1;
    glViewport(0, 0, width, height);
    m_frameWindow->setClientSize(ln::Size(width, height));
    m_nativeRenderTarget = ln::newObject<ln::RenderTargetTexture>(0, width, height);
}

void LuminoWidget::paintGL()
{
    m_frameWindow->renderView()->setClearMode(ln::RenderViewClearMode::ColorAndDepth);
    m_frameWindow->renderView()->setBackgroundColor(ln::Color::Blue);
    m_frameWindow->beginRendering(m_nativeRenderTarget);
    m_frameWindow->renderContents();
    m_frameWindow->endRendering();

    //glClearColor(0.0, 1.0, 1.0, 1.0);
    //glClear(GL_COLOR_BUFFER_BIT);

    //glBegin(GL_TRIANGLES);
    //glColor3d(1.f, 0.f, 0.f);  // 赤色
    //glVertex3d(0.f, 0.8f, 0.f);
    //glVertex3d(-0.8f, 0.f, 0.f);
    //glVertex3d(0.8f, 0.f, 0.f);
    //glEnd();
}

