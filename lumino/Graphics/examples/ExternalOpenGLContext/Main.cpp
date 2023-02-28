#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#else
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>

#include <LuminoCore.hpp>
#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoPlatform/PlatformModule.hpp>
#include <LuminoPlatform/Platform.hpp>
#include <LuminoPlatform/PlatformWindow.hpp>
#include <LuminoPlatform/detail/PlatformManager.hpp>
#include <LuminoEngine/Asset/detail/AssetManager.hpp>
#include <LuminoGraphicsRHI/ShaderCompiler/detail/ShaderManager.hpp>
#include <LuminoGraphics.hpp>
#include <LuminoGraphics/GPU/GraphicsExtensionOpenGL.hpp>
#include <LuminoGraphics/detail/GraphicsManager.hpp>
using namespace ln;

GLFWwindow* g_glfwWindow;
GLuint vao;
GLuint program;
GLuint vertex_buffer;
GLint mvp_location; 

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

#ifdef __EMSCRIPTEN__
    static const char *vertex_shader_text =
        "uniform mat4 MVP;\n"
        "attribute vec3 vCol;\n"
        "attribute vec2 vPos;\n"
        "varying vec3 color;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
        "    color = vCol;\n"
        "}\n";

    static const char *fragment_shader_text =
        "precision mediump float;\n"
        "varying vec3 color;\n"
        "void main()\n"
        "{\n"
        "    gl_FragColor = vec4(color, 1.0);\n"
        "}\n";
#else
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
#endif

    GLuint vertex_shader;
    GLuint fragment_shader;
    GLint vpos_location;
    GLint vcol_location;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

#ifdef __EMSCRIPTEN__
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

    g_glfwWindow = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!g_glfwWindow) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(g_glfwWindow);
#ifdef __EMSCRIPTEN__
#else
    gladLoadGL();
#endif
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
    RuntimeModule::initialize({});
#ifdef __EMSCRIPTEN__
    RuntimeModule::mountAssetArchive(U"Assets.lna", StringView());
#else
    RuntimeModule::mountAssetDirectory(ASSETS_DIR);
#endif

    detail::GraphicsManager::Settings graphicsManagerSettings;
    graphicsManagerSettings.graphicsAPI = GraphicsAPI::OpenGL;
    graphicsManagerSettings.debugMode = true;
    auto graphicsManager = detail::GraphicsManager::initialize(graphicsManagerSettings);
}

void initApp() {
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

void cleanupApp() {
    g_vertexBuffer = nullptr;
    g_vertexLayout = nullptr;
    g_shader = nullptr;
}

void cleanupLumino() {
    detail::GraphicsManager::terminate();
    RuntimeModule::terminate();
}

void cleanupGLFW() {
    glfwDestroyWindow(g_glfwWindow);
    glfwTerminate();
}

void mainLoop() {
    int width, height;
    glfwGetFramebufferSize(g_glfwWindow, &width, &height);

    // OpenGL rendering
    {
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glDepthMask(GL_TRUE);
        glClearDepth(0);

        Matrix m = Matrix::makeRotationZ((float)glfwGetTime());

        glBindVertexArray(vao);
        glUseProgram(program);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)m.data());
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    // Lumino rendering
    {
        auto descriptorLayout = g_shader->descriptorLayout();
        auto shaderPass = g_shader->techniques()[0]->passes()[0];

        auto commandList = OpenGLIntegration::getCommandListFromCurrentContext();
        detail::GraphicsCommandListInternal::beginCommandRecoding(commandList);
        commandList->enterRenderState();

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

        commandList->leaveRenderState();
        detail::GraphicsCommandListInternal::endCommandRecoding(commandList);
    }
    
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
