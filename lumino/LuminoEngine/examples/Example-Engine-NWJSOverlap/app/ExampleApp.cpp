#include <stdio.h>
#include <emscripten/emscripten.h>
#include <GL/gl.h>

#include <LuminoCore.hpp>
#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoPlatform/PlatformModule.hpp>
#include <LuminoPlatform/Platform.hpp>
#include <LuminoPlatform/PlatformWindow.hpp>
#include <LuminoPlatform/detail/PlatformManager.hpp>
#include <LuminoEngine/Asset/detail/AssetManager.hpp>
#include <LuminoGraphics/ShaderCompiler/detail/ShaderManager.hpp>
#include <LuminoGraphics/RHI/Shader.hpp>
#include <LuminoGraphics/RHI/ShaderDescriptor.hpp>
#include <LuminoGraphics/RHI/VertexLayout.hpp>
#include <LuminoGraphics/RHI/VertexBuffer.hpp>
#include <LuminoGraphics/RHI/SwapChain.hpp>
#include <LuminoGraphics/RHI/GraphicsCommandBuffer.hpp>
#include <LuminoGraphics/RHI/GraphicsExtensionOpenGL.hpp>
#include <LuminoGraphics/detail/GraphicsManager.hpp>
#include <LuminoEngine.hpp>
#include <LuminoPlatform/ExternalProxyPlatformWindow.hpp>
#include <LuminoEngine/Engine/ApplicationRunner.hpp>
using namespace ln;

#ifdef __cplusplus
extern "C" {
#endif

extern void gogo_test();


class App : public Application {
public:
    static void configure() {
        EngineSettings::setMainWindowTitle(U"HelloApp Example");
        EngineSettings::setGraphicsAPI(GraphicsAPI::OpenGL);

        // Lumino 自体はウィンドウシステムを初期化しないようにする
        EngineSettings::setWindowSystem(WindowSystem::External);

        // Lumino の内部的な SwapChain を作成しない。 SwapChain の制御は外部に任せる。
        EngineSettings::setUseExternalSwapChain(true);
    }

    App() {

        m_boxMesh = BoxMesh::With().buildInto();

        ln::Engine::mainCamera()->setPosition(5, 5, 5);
        ln::Engine::mainCamera()->lookAt(0, 0, 0);
        //ln::Engine::renderView()->setClearMode(SceneClearMode::None);
        ln::Engine::renderView()->setBackgroundColor(Color::Transparency);
    }

    void onUpdate() override {
    }

    Ref<BoxMesh> m_boxMesh;
};

LUMINO_APP(App);















ExternalWindowApplicationRunner g_runner;
Ref<GraphicsCommandList> g_commandList;

void EMSCRIPTEN_KEEPALIVE myFunction(int argc, char ** argv) {
    printf("MyFunction Called\n");
    //gogo_test();
}

void EMSCRIPTEN_KEEPALIVE initLumino() {
    g_runner.init(LuminoConfigureApplication, LuminoCreateApplicationInstance);
    g_commandList = GraphicsCommandList::create();
}

void EMSCRIPTEN_KEEPALIVE initApp() {
}

int g_count = 0;
void EMSCRIPTEN_KEEPALIVE renderApp(int32_t width, int32_t height) {
    g_count++;

    printf("w:%d h:%d\n", width, height);

    
    g_runner.updateFrame(width, height);

    auto renderPass = OpenGLIntegration::getRenderPass(0, width, height);

    if (g_count > 5) {
        g_commandList->enterRenderState();
        g_commandList->beginCommandRecoding();
        printf("-----\n");
        g_runner.renderFrame(g_commandList, renderPass->renderTarget(0));
        g_commandList->endCommandRecoding();
        g_commandList->leaveRenderState();
    }
}

#ifdef __cplusplus
}
#endif

