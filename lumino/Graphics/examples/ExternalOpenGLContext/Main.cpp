#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#include <glad/glad.h>
#endif
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <LuminoCore.hpp>
#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoPlatform/PlatformModule.hpp>
#include <LuminoPlatform/Platform.hpp>
#include <LuminoPlatform/PlatformWindow.hpp>
#include <LuminoPlatform/detail/PlatformManager.hpp>
#include <LuminoEngine/Asset/detail/AssetManager.hpp>
#include <LuminoShaderCompiler/detail/ShaderManager.hpp>
#include <LuminoGraphics/Shader.hpp>
#include <LuminoGraphics/ShaderDescriptor.hpp>
#include <LuminoGraphics/VertexLayout.hpp>
#include <LuminoGraphics/VertexBuffer.hpp>
#include <LuminoGraphics/SwapChain.hpp>
#include <LuminoGraphics/GraphicsCommandBuffer.hpp>
#include "../src/GraphicsManager.hpp"
using namespace ln;

GLFWwindow* g_glfwWindow;
Ref<Shader> g_shader;
Ref<VertexLayout> g_vertexLayout;
Ref<VertexBuffer> g_vertexBuffer;

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

void initGLFW() {
    static const struct
    {
        float x, y;
        float r, g, b;
    } vertices[3] = {
        { -0.6f, -0.4f, 1.f, 0.f, 0.f },
        { 0.6f, -0.4f, 0.f, 1.f, 0.f },
        { 0.f, 0.6f, 0.f, 0.f, 1.f }
    };

    static const char* vertex_shader_text =
        "#version 400\n"
        "uniform mat4 MVP;\n"
        "layout(location = 0) in vec3 vCol;\n"
        "layout(location = 1) in vec2 vPos;\n"
        "out vec3 color;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
        "    color = vCol;\n"
        "}\n";

    static const char* fragment_shader_text =
        "#version 400\n"
        "in vec3 color;\n"
        "void main()\n"
        "{\n"
        "    gl_FragColor = vec4(color, 1.0);\n"
        "}\n";


    GLuint vao, vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    g_glfwWindow = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!g_glfwWindow) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(g_glfwWindow);
    gladLoadGL();
    glfwSwapInterval(1);

    // NOTE: OpenGL error checks have been omitted for brevity

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*)0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*)(sizeof(float) * 2));
}

void initLumino() {
    EngineContext2::initialize();
    Logger::setLevel(LogLevel::Debug);

    //detail::PlatformManager::Settings platformManagerrSettings;
    //platformManagerrSettings.windowSystem = WindowSystem::External;
    //auto platformManager = detail::PlatformManager::initialize(platformManagerrSettings);

    detail::AssetManager::Settings assetManagerSettings;
    auto assetManager = detail::AssetManager::initialize(assetManagerSettings);
#ifdef __EMSCRIPTEN__
    assetManager->mountAssetArchive(U"Assets.lna", StringView());
#else
    assetManager->addAssetDirectory(ASSETS_DIR);
#endif

    detail::GraphicsManager::Settings graphicsManagerSettings;
    graphicsManagerSettings.assetManager = assetManager;
    graphicsManagerSettings.platformManager = nullptr;
    //platformManager;
    graphicsManagerSettings.mainWindow = nullptr;
    //platformManager->mainWindow();
    graphicsManagerSettings.graphicsAPI = GraphicsAPI::OpenGL;
    graphicsManagerSettings.priorityGPUName = U"";
    graphicsManagerSettings.debugMode = true;
    auto graphicsManager = detail::GraphicsManager::initialize(graphicsManagerSettings);

    detail::ShaderManager::Settings shaderManagerSettings;
    shaderManagerSettings.graphicsManager = graphicsManager;
    auto shaderManager = detail::ShaderManager::initialize(shaderManagerSettings);

    
}

void initApp() {
    g_shader = Shader::load(U"simple");

    g_vertexLayout = makeObject<VertexLayout>();
    g_vertexLayout->addElement(0, VertexElementType::Float4, VertexElementUsage::Position, 0);

    // CCW
    Vector4 v[] = {
        Vector4(0, 0.5, 0, 1),
        Vector4(-0.5, -0.25, 0, 1),
        Vector4(0.5, -0.25, 0, 1),
    };
    g_vertexBuffer = makeObject<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);
}

void cleanupApp() {
    g_vertexBuffer = nullptr;
    g_vertexLayout = nullptr;
    g_shader = nullptr;
}

void cleanupLumino() {
    detail::GraphicsManager::terminate();
    detail::AssetManager::terminate();
    detail::PlatformManager::terminate();
    EngineContext2::terminate();
}

void cleanupGLFW() {
    glfwDestroyWindow(g_glfwWindow);
    glfwTerminate();
}

void mainLoop() {
    //auto descriptorLayout = g_shader->descriptorLayout();
    //auto shaderPass = g_shader->techniques()[0]->passes()[0];

    //auto ctx = g_swapChain->beginFrame2();
    //auto descriptor = ctx->allocateShaderDescriptor(shaderPass);
    //descriptor->setVector(descriptorLayout->findUniformMemberIndex(U"_Color"), Vector4(1, 0, 0, 1));
    //auto renderPass = g_swapChain->currentRenderPass();
    //ctx->beginRenderPass(renderPass);
    //ctx->clear(ClearFlags::All, Color::Azure);
    //ctx->setVertexLayout(g_vertexLayout);
    //ctx->setVertexBuffer(0, g_vertexBuffer);
    //ctx->setShaderPass(shaderPass);
    //ctx->setShaderDescriptor(descriptor);
    //ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
    //ctx->drawPrimitive(0, 1);
    //ctx->endRenderPass();

    //g_swapChain->endFrame();

    
    glfwSwapBuffers(g_glfwWindow);
    glfwPollEvents();
}

int main() {
    initGLFW();
    initLumino();
    initApp();
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainLoop, 60, true);
#else
    while (!glfwWindowShouldClose(g_glfwWindow)) {
        mainLoop();
        Thread::sleep(16);
    }
#endif
    cleanupApp();
    cleanupLumino();
    cleanupGLFW();
    return 0;
}
