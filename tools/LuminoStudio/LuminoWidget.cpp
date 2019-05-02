#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QTextEdit>


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
    ln::GlobalLogger::addStdErrAdapter();

    ln::EngineSettings::setUserMainWindow(this->winId());
    ln::EngineSettings::setDefaultObjectsCreation(false);
    ln::EngineSettings::setUseGLFWWindowSystem(false);
    ln::EngineSettings::setGraphicsContextManagement(false);
    ln::EngineSettings::setExternalMainLoop(false);
    ln::EngineSettings::setExternalRenderingManagement(true);
    ln::EngineSettings::setEngineFeatures(ln::EngineFeature::Public);
    ln::Engine::initialize();

    //m_frameWindow = ln::newObject<ln::UINativeFrameWindow>();
}

void LuminoWidget::resizeGL(int width, int height)
{
    if (height == 0) height = 1;
    glViewport(0, 0, width, height);
    //m_frameWindow->setClientSize(ln::Size(width, height));
}

void LuminoWidget::paintGL()
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

