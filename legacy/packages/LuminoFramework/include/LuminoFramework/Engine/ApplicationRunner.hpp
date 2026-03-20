#pragma once
#include "Application.hpp"

namespace ln {

class ApplicationRunnerBase {
protected:
    ApplicationRunnerBase();
    virtual ~ApplicationRunnerBase();

    static Result<> initEngine();
    static bool updateEngine();
    static void renderEngine(GraphicsCommandList* commandList, RenderTargetTexture* renderTarget);
    static void terminateEngine();
};

class StandaloneApplicationRunner : public ApplicationRunnerBase {
public:
    StandaloneApplicationRunner();
    Result<> run(ConfigureAppFunc configureApp, CreateAppInstanceFunc createAppInstance);

private:
    Ref<Application> m_app;
};

// MainWindow 1つだけの描画先を持ち、SwapChain のフリップを外部で行うアプリケーション用の Runner.
class ExternalWindowApplicationRunner : public ApplicationRunnerBase {
public:
    ExternalWindowApplicationRunner();
    Result<> init(ConfigureAppFunc configureApp, CreateAppInstanceFunc createAppInstance);
    bool updateFrame(int width, int height);
    void renderFrame(GraphicsCommandList* commandList, RenderTargetTexture* renderTarget);
    void terminate();

private:
    Ref<Application> m_app;
};

} // namespace ln
