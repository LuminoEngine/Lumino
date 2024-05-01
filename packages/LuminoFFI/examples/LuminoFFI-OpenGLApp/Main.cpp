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

    LNHandle renderingCommandList = LN_NULL_HANDLE;
    if (LNRenderingCommandList_Create(graphicsContext, &renderingCommandList) != LN_OK) {
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

    LNHandle material = LN_NULL_HANDLE;
    if (LNMaterial_Create(&material) != LN_OK) {
		return 1;
	}

    LNHandle spriteRenderer = LN_NULL_HANDLE;
    if (LNSpriteRenderer_Get(&spriteRenderer) != LN_OK) {
        return 1;
    }

    LNHandle spriteTextRenderer = LN_NULL_HANDLE;
    if (LNSpriteTextRenderer_Get(&spriteTextRenderer) != LN_OK) {
        return 1;
    }

    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        if (LNSceneRenderingViewPoint_SetupPerspective2D(sceneRenderingViewPoint, 0, 0, 0, width, height, -500, 500) != LN_OK) {
            return 1;
        }

        if (LNRenderingCommandList_Reset(renderingCommandList, sceneRenderingViewPoint, graphicsContext) != LN_OK) {
            return 1;
        }

        LNMatrix transform;
        LNMatrix_SetIdentity(&transform);
        LNSpriteRenderer_BeginBatch(spriteRenderer, renderingCommandList, material, &transform);
        LNSpriteRenderer_DrawSprite(spriteRenderer, NULL,
            200, 100,
            0, 0,
            0, 0, 1, 1,
            0, 0, 1, 1,
            LN_SPRITE_BASE_DIRECTION_BASIC2D,
            LN_BILLBOARD_TYPE_NONE);
        LNSpriteRenderer_EndBatch(spriteRenderer);




        LNSpriteTextRenderer_BeginBatch(spriteTextRenderer, renderingCommandList, material, &transform);
        LNSpriteTextRenderer_DrawFillText(spriteTextRenderer, NULL, "Hello!!");
        LNSpriteTextRenderer_EndBatch(spriteTextRenderer);

        if (LNRenderingCommandList_Submit(renderingCommandList, LN_NULL_HANDLE, graphicsContext) != LN_OK) {
            return 1;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    LNObject_Release(material);
    LNObject_Release(unlitSceneRenderingPass);
    LNObject_Release(sceneRenderingViewPoint);
    LNObject_Release(renderingCommandList);
    LNObject_Release(graphicsContext);
    LNRuntime_Terminate();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
