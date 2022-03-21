﻿#include <LuminoEngine.hpp>
using namespace ln;

class App : public Application {
public:
    static void configure() {
        EngineSettings::setMainWindowTitle(U"HelloApp Example");
    }

    App() {
    }

    void onUpdate() override {
    }
};

LUMINO_APP(App);


//========================================================================
//========================================================================

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

//#include "linmath.h"

#include <stdlib.h>
#include <stdio.h>

static const struct
{
    float x, y;
    float r, g, b;
} vertices[3] = {
    { -0.6f, -0.4f, 1.f, 0.f, 0.f },
    { 0.6f, -0.4f, 0.f, 1.f, 0.f },
    { 0.f, 0.6f, 0.f, 0.f, 1.f }
};

//static const char* vertex_shader_text =
//    "#version 110\n"
//    "uniform mat4 MVP;\n"
//    "attribute vec3 vCol;\n"
//    "attribute vec2 vPos;\n"
//    "varying vec3 color;\n"
//    "void main()\n"
//    "{\n"
//    "    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
//    "    color = vCol;\n"
//    "}\n";
//
//static const char* fragment_shader_text =
//    "#version 110\n"
//    "varying vec3 color;\n"
//    "void main()\n"
//    "{\n"
//    "    gl_FragColor = vec4(color, 1.0);\n"
//    "}\n";
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

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(void) {
    GLFWwindow* window;
    GLuint vao, vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
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

    EngineSettings::setUseExternalSwapChain(true);
    EngineSettings::setUserMainWindow((intptr_t)glfwGetWin32Window(window));
    EngineSettings::setGraphicsAPI(GraphicsAPI::OpenGL);
    Application* app = ::LuminoCreateApplicationInstance();
    AppIntegration::initialize(app);

    while (!glfwWindowShouldClose(window)) {
        float ratio;
        int width, height;
        Matrix m;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        m = Matrix::makeRotationZ((float)glfwGetTime());

        glUseProgram(program);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)m.data());
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //AppIntegration::update(app);
        //AppIntegration::render(app);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    AppIntegration::terminate(app);
    delete app;

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

