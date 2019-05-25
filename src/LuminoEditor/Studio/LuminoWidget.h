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

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    ln::Ref<ln::UINativeFrameWindow> m_frameWindow;
    ln::Ref<ln::RenderTargetTexture> m_nativeRenderTarget;
};