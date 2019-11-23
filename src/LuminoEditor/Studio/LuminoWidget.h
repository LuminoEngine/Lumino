#pragma once
#include <QOpenGLWidget>

class LuminoWidget
    : public QOpenGLWidget
    //, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit LuminoWidget(QWidget* parent = nullptr);
    ~LuminoWidget();
    const ln::Ref<ln::UINativeFrameWindow>& frameWindow() const { return m_frameWindow; }

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    virtual void mousePressEvent(QMouseEvent* e) override;
    virtual void mouseReleaseEvent(QMouseEvent* e) override;
    virtual void mouseMoveEvent(QMouseEvent* e) override;

private:
    ln::Ref<ln::UINativeFrameWindow> m_frameWindow;
    ln::Ref<ln::UIViewport> m_mainViewport;
    ln::Ref<ln::World> m_mainWorld;
    ln::Ref<ln::Camera> m_mainCamera;
    ln::Ref<ln::WorldRenderView> m_mainWorldRenderView;
    ln::Ref<ln::UIRenderView> m_mainUIRenderView;
    ln::Ref<ln::RenderTargetTexture> m_nativeRenderTarget;
    bool m_mousePressed;
};