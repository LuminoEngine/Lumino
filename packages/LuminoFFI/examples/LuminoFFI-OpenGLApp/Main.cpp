#include <stdio.h>
#include <vector>
#include <GLFW/glfw3.h>
#include <lumino.h>
#include <LuminoCore.hpp>

static size_t GetFileSize(FILE* stream) {
    struct stat stbuf;
    int handle = fileno(stream);
    if (handle == 0) return 0;
    if (fstat(handle, &stbuf) == -1) return 0;
    return stbuf.st_size;
}

std::vector<uint8_t> ReadAllBytes(const char* filePath) {
    FILE* file = fopen(filePath, "rb");
    if (!file) {
        printf("Error: %s\n", filePath);
        return {};
    }
    size_t size = GetFileSize(file);
    std::vector<uint8_t> buffer(size);
    fread(buffer.data(), 1, size, file);
    fclose(file);
    return buffer;
}

int main() {
#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    printf("glfwInit...");
    if (!glfwInit()) {
        return 1;
    }
    printf(" Done.\n");

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

    if (LNInstance_Initialize() != LN_OK) {
        return 1;
    }




    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    LNHandle graphicsContext = LN_NULL_HANDLE;
    if (LNGraphicsContext_CreateFromCurrentGL(width, height, &graphicsContext) != LN_OK) {
        return 1;
    }

    LNHandle viewPoint1 = LN_NULL_HANDLE;
    if (LNViewPoint_Create(&viewPoint1) != LN_OK) {
        return 1;
    }

    LNHandle viewPoint2 = LN_NULL_HANDLE;
    if (LNViewPoint_Create(&viewPoint2) != LN_OK) {
        return 1;
    }

    LNHandle unlitSceneRenderingPass = LN_NULL_HANDLE;
    if (LNUnlitSceneRenderingPass_Create(&unlitSceneRenderingPass) != LN_OK) {
        return 1;
    }

    std::vector<uint8_t> imageData = ReadAllBytes("C:/Proj/LN/Lumino/assets/Distributable/assets/icon256.png");
    LNHandle texture1 = LN_NULL_HANDLE;
    if (LNTexture2D_CreateFromImageFileData(imageData.data(), imageData.size(), &texture1) != LN_OK) {
        return 1;
    }

    LNHandle material1 = LN_NULL_HANDLE;
    if (LNMaterial_Create(&material1) != LN_OK) {
		return 1;
    }

    if (LNMaterial_SetMainTexture(material1, texture1) != LN_OK) {
        return 1;
    }

    LNHandle texture2 = LN_NULL_HANDLE;
    if (LNTexture2D_Create(300, 200, &texture2) != LN_OK) {
        return 1;
    }






    LNHandle texture2Context = LN_NULL_HANDLE;
    LNTexture2D_GetContext(texture2, &texture2Context);

    LNTextureRenderingContext_StrokeText(texture2Context);
    LNTextureRenderingContext_FillText(texture2Context);

    LNHandle material2 = LN_NULL_HANDLE;
    if (LNMaterial_Create(&material2) != LN_OK) {
        return 1;
    }

    if (LNMaterial_SetMainTexture(material2, texture2) != LN_OK) {
        return 1;
    }

    LNHandle spriteRenderer = LN_NULL_HANDLE;
    if (LNBatchRenderer_Get(&spriteRenderer) != LN_OK) {
        return 1;
    }

    LNHandle spriteTextRenderer = LN_NULL_HANDLE;
    if (LNSpriteTextRenderer_Get(&spriteTextRenderer) != LN_OK) {
        return 1;
    }

    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        ln::ElapsedTimer t;

        LNHandle colorBuffer = LN_NULL_HANDLE;
        LNHandle depthBuffer = LN_NULL_HANDLE;
        LNHandle commandList = LN_NULL_HANDLE;
        LNGraphicsContext_BeginFrame(graphicsContext, 320, 240, &colorBuffer, &depthBuffer, &commandList);

        // RenderPass
        //   Lumino では RenderPass=視点を起点とした描画シーケンスと考えてみる。
        //   これはシャドウバッファの描画を考えるとイメージしやすいかもしれない。
        //   Three.js でもシャドウバッファへの描画は、その数ごとに専用のカメラを起点として Scene 全体の render() を行っている。
        if (1) {
            if (LNViewPoint_SetupPerspective2DLH(viewPoint1, 0, 0, 0, width, height, -500, 500) != LN_OK) {
                return 1;
            }

            LNHandle renderingPass = LN_NULL_HANDLE;
            LNRenderPassDescriptor descriptor;
            descriptor.renderTargets[0].renderTarget = colorBuffer;
            descriptor.renderTargets[0].clearColor[0] = 0.0f;
            descriptor.renderTargets[0].clearColor[1] = 1.0f;
            descriptor.renderTargets[0].clearColor[2] = 0.0f;
            descriptor.renderTargets[0].clearColor[3] = 1.0f;
            descriptor.renderTargets[0].clearEnable = LN_TRUE;
            descriptor.depthBuffer.depthBuffer = depthBuffer;
            descriptor.depthBuffer.clearDepth = 1.0f;
            descriptor.depthBuffer.clearStencil = 0;
            descriptor.depthBuffer.clearDepthEnable = LN_TRUE;
            descriptor.depthBuffer.clearStencilEnable = LN_TRUE;
            if (LNCommandList_BeginRenderPass(commandList, descriptor, viewPoint1, &renderingPass) != LN_OK) {
                return 1;
            }

            LNMatrix transform;
            LNMatrix_SetIdentity(&transform);
            for (int i = 0; i < 500; i++) {
                transform.m41 = 10 + i;
                LNBatchRenderer_BeginBatch(spriteRenderer, commandList, material1, &transform);
                LNBatchRenderer_DrawSprite_deprecated(spriteRenderer, NULL,
                    200, 100,
                    0, 0,
                    0, 0, 1, 1,
                    0, 0, 1, 1,
                    LN_SPRITE_BASE_DIRECTION_BASIC2D,
                    LN_BILLBOARD_TYPE_NONE);
                LNBatchRenderer_EndBatch(spriteRenderer);
            }
        
            transform.m41 = 400;
            LNBatchRenderer_BeginBatch(spriteRenderer, commandList, material2, &transform);
            LNBatchRenderer_DrawSprite_deprecated(spriteRenderer, NULL,
                300, 200,
                0, 0,
                0, 0, 1, 1,
                1, 1, 1, 1,
                LN_SPRITE_BASE_DIRECTION_BASIC2D,
                LN_BILLBOARD_TYPE_NONE);
            LNBatchRenderer_EndBatch(spriteRenderer);

            if (LNCommandList_EndRenderPass(commandList, renderingPass) != LN_OK) {
                return 1;
            }
        }

        // RenderPass
        if (0) {
            if (LNViewPoint_SetupPerspectiveOrthoLH(viewPoint2, 0, 0, -100, 0, 0, 0, width, height, -500, 500) != LN_OK) {
                return 1;
            }

            LNHandle renderingPass = LN_NULL_HANDLE;
            LNRenderPassDescriptor descriptor;
            descriptor.renderTargets[0].renderTarget = colorBuffer;
            descriptor.renderTargets[0].clearEnable = LN_FALSE;
            descriptor.depthBuffer.depthBuffer = depthBuffer;
            descriptor.depthBuffer.clearDepthEnable = LN_FALSE;
            descriptor.depthBuffer.clearStencilEnable = LN_FALSE;
            if (LNCommandList_BeginRenderPass(commandList, descriptor, viewPoint2, &renderingPass) != LN_OK) {
                return 1;
            }

            LNMatrix transform;
            LNMatrix_SetIdentity(&transform);
            transform.m41 = 0;
            LNBatchRenderer_BeginBatch(spriteRenderer, commandList, material1, &transform);
            LNBatchRenderer_DrawSprite_deprecated(spriteRenderer, NULL,
                100, 100,
                0, 0,
                0, 0, 1, 1,
                0, 0, 1, 1,
                LN_SPRITE_BASE_DIRECTION_ZPLUS,
                LN_BILLBOARD_TYPE_NONE);
            LNBatchRenderer_EndBatch(spriteRenderer);

            if (LNCommandList_EndRenderPass(commandList, renderingPass) != LN_OK) {
                return 1;
            }
        }


        //LNSpriteTextRenderer_BeginBatch(spriteTextRenderer, renderingCommandList, material, &transform);
        //LNSpriteTextRenderer_DrawFillText(spriteTextRenderer, NULL, "Hello!!");
        //LNSpriteTextRenderer_EndBatch(spriteTextRenderer);

        if (LNGraphicsContext_EndFrame(graphicsContext) != LN_OK) {
            return 1;
        }
        std::cout << t.elapsedMilliseconds() << "[ms] Total" << std::endl;


        glfwSwapBuffers(window);
        glfwPollEvents();

        // TODO: x. デバイスロスト発生時は、GraphicsContext を再作成する必要があります。
    }

    LNObject_Release(material2);
    LNObject_Release(texture2);
    LNObject_Release(material1);
    LNObject_Release(texture1);
    LNObject_Release(unlitSceneRenderingPass);
    LNObject_Release(viewPoint2);
    LNObject_Release(viewPoint1);
    LNObject_Release(graphicsContext);
    LNInstance_Terminate();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
