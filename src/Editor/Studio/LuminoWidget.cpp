#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QTextEdit>

#include <LuminoEngine/UI/UIContext.hpp>
#include "MainWindow.h"
#include "LuminoWidget.h"

//==============================================================================
// LuminoWidget

LuminoWidget::LuminoWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , m_mousePressed(false)
{
    setMouseTracking(true);
}

LuminoWidget::~LuminoWidget()
{
}

void LuminoWidget::initializeGL()
{
    //MainWindow::initializeLumino();

    m_frameWindow = ln::makeObject<ln::UINativeFrameWindow>();
    m_frameWindow->attachRenderingThread(ln::RenderingType::Immediate);

    m_mainViewport = ln::makeObject<ln::UIViewport>();
    m_frameWindow->addElement(m_mainViewport);

    m_mainWorld = ln::makeObject<ln::World>();
    m_mainCamera = ln::makeObject<ln::Camera>();
    m_mainCamera->setPosition(5, 5, -10);
    m_mainCamera->lookAt(ln::Vector3(0, 0, 0));
    m_mainWorldRenderView = ln::makeObject<ln::WorldRenderView>();
    m_mainWorldRenderView->setTargetWorld(m_mainWorld);
    m_mainWorldRenderView->setCamera(m_mainCamera);
    m_mainWorldRenderView->setClearMode(ln::RenderViewClearMode::ColorAndDepth);
    m_mainWorldRenderView->setBackgroundColor(ln::Color::White);
    m_mainViewport->addRenderView(m_mainWorldRenderView);

    // TODO: Test
    //auto ctl = ln::makeObject<ln::CameraOrbitControlComponent>();
    //m_mainCamera->addComponent(ctl);


    m_mainCamera->addComponent(ln::makeObject<ln::CameraOrbitControlComponent>());
    m_mainCamera->setBackgroundColor(ln::Color::Gray);
    //m_mainWorldRenderView->setClearMode(ln::RenderViewClearMode::None);
}

void LuminoWidget::resizeGL(int width, int height)
{
    if (height == 0) height = 1;
    glViewport(0, 0, width, height);
    m_frameWindow->setClientSize(ln::Size(width, height));
    m_nativeRenderTarget = ln::makeObject<ln::RenderTargetTexture>(0, ln::TextureFormat::Unknown);
    ln::detail::TextureInternal::resetSize(m_nativeRenderTarget, width, height);
 
    ln::Engine::mainUIContext()->setLayoutRootElement(m_frameWindow);
    ln::Engine::mainUIContext()->updateStyleTree();
   // m_mainUIContext->updateStyleTree();
    m_frameWindow->updateLayoutTree();
}

void LuminoWidget::paintGL()
{
	if (m_frameWindow) {

		//glGetFramebufferAttachmentParameter with GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE



		auto gl = QOpenGLContext::globalShareContext();
		//qDebug() << "MapExplorer" << QOpenGLContext::areSharing(this->context(), context()) << (QOpenGLContext::currentContext() == this->context());
		//qDebug() << "global?" << QOpenGLContext::areSharing(context(), gl);

		//QOpenGLWidget::makeCurrent();

		//glClearColor(0.0, 1.0, 1.0, 1.0);
		//glClear(GL_COLOR_BUFFER_BIT);

		// Current framebuffer にアタッチされているテクスチャId を取り出しておく
		ln::detail::TextureInternal::resetOpenGLTextureIdFromCurrentFramebuffer(m_nativeRenderTarget);

		// TODO: １回レンダリングするとメモリリークする・・・
		//m_frameWindow->renderView()->setClearMode(ln::RenderViewClearMode::ColorAndDepth);
		//m_frameWindow->renderView()->setBackgroundColor(ln::Color::White);
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


void LuminoWidget::mousePressEvent(QMouseEvent* e)
{
    switch (e->button())
    {
    case Qt::LeftButton:
        m_frameWindow->inputInjector()->injectMouseButtonDown(ln::MouseButtons::Left);
        e->accept();
        m_mousePressed = true;
        break;
    case Qt::RightButton:
        m_frameWindow->inputInjector()->injectMouseButtonDown(ln::MouseButtons::Right);
        e->accept();
        m_mousePressed = true;
        break;
    case Qt::MiddleButton:
        m_frameWindow->inputInjector()->injectMouseButtonDown(ln::MouseButtons::Middle);
        e->accept();
        m_mousePressed = true;
        break;
    default:
        QWidget::mousePressEvent(e);
        break;
    }
}

void LuminoWidget::mouseReleaseEvent(QMouseEvent* e)
{
    switch (e->button())
    {
    case Qt::LeftButton:
        m_frameWindow->inputInjector()->injectMouseButtonUp(ln::MouseButtons::Left);
        e->accept();
        break;
    case Qt::RightButton:
        m_frameWindow->inputInjector()->injectMouseButtonUp(ln::MouseButtons::Right);
        e->accept();
        break;
    case Qt::MiddleButton:
        m_frameWindow->inputInjector()->injectMouseButtonUp(ln::MouseButtons::Middle);
        e->accept();
        break;
    default:
        QWidget::mouseReleaseEvent(e);
        break;
    }

    m_mousePressed = false;
}

void LuminoWidget::mouseMoveEvent(QMouseEvent* e)
{
    m_frameWindow->inputInjector()->injectMouseMove(e->pos().x(), e->pos().y());
    QWidget::mouseMoveEvent(e);

    if (m_mousePressed) {
        update();
    }
}
