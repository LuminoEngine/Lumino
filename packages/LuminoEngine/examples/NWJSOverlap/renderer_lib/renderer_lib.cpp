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
#include <LuminoGraphics/GPU/Shader.hpp>
#include <LuminoGraphics/GPU/ShaderDescriptor.hpp>
#include <LuminoGraphics/GPU/VertexLayout.hpp>
#include <LuminoGraphics/GPU/VertexBuffer.hpp>
#include <LuminoGraphics/GPU/SwapChain.hpp>
#include <LuminoGraphics/GPU/GraphicsCommandBuffer.hpp>
#include <LuminoGraphics/GPU/GraphicsExtensionOpenGL.hpp>
#include <LuminoGraphics/detail/GraphicsManager.hpp>
using namespace ln;

#ifdef __cplusplus
extern "C" {
#endif

extern void gogo_test();

Ref<Shader> g_shader;
Ref<VertexLayout> g_vertexLayout;
Ref<VertexBuffer> g_vertexBuffer;

void EMSCRIPTEN_KEEPALIVE myFunction(int argc, char ** argv) {
    printf("MyFunction Called\n");
    //gogo_test();
}

void EMSCRIPTEN_KEEPALIVE initLumino() {
    RuntimeModule::initialize();
    Logger::setLevel(LogLevel::Debug);

    detail::AssetManager::instance()->mountAssetArchive(U"Assets.lna", StringView());

    detail::GraphicsManager::Settings graphicsManagerSettings;
    graphicsManagerSettings.graphicsAPI = GraphicsAPI::OpenGL;
    graphicsManagerSettings.priorityGPUName = U"";
    graphicsManagerSettings.debugMode = true;
    auto graphicsManager = detail::GraphicsManager::initialize(graphicsManagerSettings);

    detail::ShaderManager::Settings shaderManagerSettings;
    shaderManagerSettings.graphicsManager = graphicsManager;
    auto shaderManager = detail::ShaderManager::initialize(shaderManagerSettings);
    
}

void EMSCRIPTEN_KEEPALIVE initApp() {
    g_shader = Shader::load(U"simple");

    g_vertexLayout = makeObject_deprecated<VertexLayout>();
    g_vertexLayout->addElement(0, VertexElementType::Float4, VertexElementUsage::Position, 0);

    // CCW
    Vector4 v[] = {
        Vector4(0, 0.5, 0, 1),
        Vector4(-0.5, -0.25, 0, 1),
        Vector4(0.5, -0.25, 0, 1),
    };
    g_vertexBuffer = makeObject_deprecated<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);
}

void EMSCRIPTEN_KEEPALIVE renderApp(int32_t width, int32_t height) {
    printf("w:%d h:%d\n", width, height);
    auto descriptorLayout = g_shader->descriptorLayout();
    auto shaderPass = g_shader->techniques()[0]->passes()[0];

    auto commandList = OpenGLIntegration::getCommandListFromCurrentContext();
    detail::GraphicsCommandListInternal::beginCommandRecoding(commandList);
    //commandList->enterRenderState();

    auto renderPass = OpenGLIntegration::getRenderPass(0, width, height);

    auto descriptor = commandList->allocateShaderDescriptor_deprecated(shaderPass);
    descriptor->setVector(descriptorLayout->findUniformMemberIndex(U"_Color"), Vector4(1, 0, 0, 1));
    commandList->beginRenderPass(renderPass);
    commandList->clear(ClearFlags::Depth, Color());
    commandList->setVertexLayout(g_vertexLayout);
    commandList->setVertexBuffer(0, g_vertexBuffer);
    commandList->setShaderPass(shaderPass);
    commandList->setShaderDescriptor_deprecated(descriptor);
    commandList->setPrimitiveTopology(PrimitiveTopology::TriangleList);
    commandList->drawPrimitive(0, 1);
    commandList->endRenderPass();


    //commandList->leaveRenderState();
    detail::GraphicsCommandListInternal::endCommandRecoding(commandList);
}

#ifdef __cplusplus
}
#endif

