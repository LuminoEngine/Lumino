#include <stdio.h>
#include <GLFW/glfw3.h>
#include <lumino.h>

int main() {
    printf("1\n");
    if (!glfwInit()) {
        return 1;
    }
    printf("2\n");

    // NOTE: 2024/4/17 時点の Windows11 デフォルトは GL 4.6 だった。
    // GLFW_OPENGL_PROFILE は GLFW_OPENGL_ANY_PROFILE でないと glfwCreateWindow() が失敗する。
     
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);

    // Enable OpenGL ES Profile.
    //glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
    GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
    if (!window) {
        return 1;
    }
    glfwMakeContextCurrent(window);

    if (LNRuntime_Initialize() != LN_OK) {
        return 1;
    }



    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    LNHandle graphicsContext = LN_NULL_HANDLE;
    if (LNGraphicsContext_CreateFromCurrentOpenGLContext(width, height, &graphicsContext) != LN_OK) {
        return 1;
    }

    LNHandle renderingContext = LN_NULL_HANDLE;
    if (LNRenderingContext_Create(graphicsContext, &renderingContext) != LN_OK) {
        return 1;
    }

    LNHandle sceneRenderingViewPoint = LN_NULL_HANDLE;
    if (LNSceneRenderingViewPoint_Create(&sceneRenderingViewPoint) != LN_OK) {
        return 1;
    }

    LNHandle unlitSceneRenderingPass = LN_NULL_HANDLE;
    if (LNUnlitSceneRenderingPass_Create(&unlitSceneRenderingPass) != LN_OK) {
        return 1;
    }

    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        if (LNSceneRenderingViewPoint_SetupPerspective2D(sceneRenderingViewPoint, 0, 0, 0, width, height, -500, 500) != LN_OK) {
            return 1;
        }



        if (LNRenderingContext_Reset(renderingContext, sceneRenderingViewPoint, graphicsContext) != LN_OK) {
            return 1;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    LNObject_Release(unlitSceneRenderingPass);
    LNObject_Release(sceneRenderingViewPoint);
    LNObject_Release(renderingContext);
    LNObject_Release(graphicsContext);
    LNRuntime_Terminate();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
