#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QTextEdit>

#include "MainWindow.h"
#include "LuminoWidget.h"

//==============================================================================
// LuminoWidget

LuminoWidget::LuminoWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

LuminoWidget::~LuminoWidget()
{
}

void LuminoWidget::initializeGL()
{
    //MainWindow::initializeLumino();

    m_frameWindow = ln::makeObject<ln::UINativeFrameWindow>();
    m_frameWindow->attachRenderingThread(ln::RenderingType::Immediate);
}

void LuminoWidget::resizeGL(int width, int height)
{
    if (height == 0) height = 1;
    glViewport(0, 0, width, height);
    m_frameWindow->setClientSize(ln::Size(width, height));
    m_nativeRenderTarget = ln::makeObject<ln::RenderTargetTexture>(0, ln::TextureFormat::Unknown);
    ln::detail::TextureInternal::resetSize(m_nativeRenderTarget, width, height);
    
}

void LuminoWidget::paintGL()
{
	if (m_frameWindow) {
		auto gl = QOpenGLContext::globalShareContext();
		//qDebug() << "MapExplorer" << QOpenGLContext::areSharing(this->context(), context()) << (QOpenGLContext::currentContext() == this->context());
		//qDebug() << "global?" << QOpenGLContext::areSharing(context(), gl);

		//QOpenGLWidget::makeCurrent();

		// TODO: １回レンダリングするとメモリリークする・・・
		m_frameWindow->renderView()->setClearMode(ln::RenderViewClearMode::ColorAndDepth);
		m_frameWindow->renderView()->setBackgroundColor(ln::Color::White);
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
}

