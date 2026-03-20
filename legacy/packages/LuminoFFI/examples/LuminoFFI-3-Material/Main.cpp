#include <stdio.h>
#include <vector>
#include <string>
#include <lumino.h>

int main() {
    LNConfig_SetGraphicsBackend(LN_GRAPHICS_BACKEND_WEBGPU);
    //LNConfig_SetGraphicsBackend(LN_GRAPHICS_BACKEND_VULKAN);

    LNResult result = LNInstance_Initialize();
    if (result != LN_OK) {
        return 1;
    }

    LNHandle window = LN_NULL_HANDLE;
    LNHandle graphicsContext = LN_NULL_HANDLE;
    LNHandle viewPoint = LN_NULL_HANDLE;
    LNWindow_Create(640, 480, "Example", &window);
    LNWindow_GetGraphicsContext(window, &graphicsContext);
    LNViewPoint_Create(&viewPoint);

    

    LNHandle material1 = LN_NULL_HANDLE;
    LNMaterial_CreateFromSourceFile(ASSETS_DIR "/Circles1.slang", &material1);

    LNHandle spriteRenderer = LN_NULL_HANDLE;
    LNBatchRenderer_Get(&spriteRenderer);

    int frameCount = 0;
    while (true) {
        LNBool quit = LN_FALSE;
        LNInstance_ProcessEvents();
        LNInstance_ShouldQuit(&quit);
        if (quit) {
            break;
        }
        int width = 0;
        int height = 0;
        LNHandle colorBuffer = LN_NULL_HANDLE;
        LNHandle depthBuffer = LN_NULL_HANDLE;
        LNWindow_GetFramebufferSize(window, &width, &height);
        LNGraphicsContext_BeginFrame(graphicsContext, width, height, &colorBuffer, &depthBuffer);

        LNViewPoint_SetupOrtho2D(viewPoint, 0, 0, 0, width, height, -500, 500);

        int index = -1;
        LNMaterial_FindParameterIndex(material1, "u_Time", &index);
        LNMaterial_SetFloat(material1, index, (float)frameCount / 300.0f);

        LNHandle renderingPass = LN_NULL_HANDLE;
        LNRenderPassDescriptor descriptor;
        descriptor.renderTargets[0].renderTarget = colorBuffer;
        descriptor.renderTargets[0].clearColor[0] = 0.60f;
        descriptor.renderTargets[0].clearColor[1] = 0.85f;
        descriptor.renderTargets[0].clearColor[2] = 0.60f;
        descriptor.renderTargets[0].clearColor[3] = 1.0f;
        descriptor.renderTargets[0].clearEnable = LN_TRUE;
        descriptor.depthBuffer.depthBuffer = depthBuffer;
        descriptor.depthBuffer.clearDepth = 1.0f;
        descriptor.depthBuffer.clearStencil = 0;
        descriptor.depthBuffer.clearDepthEnable = LN_TRUE;
        descriptor.depthBuffer.clearStencilEnable = LN_TRUE;
        LNGraphicsContext_BeginSceneRenderPass(graphicsContext, descriptor, viewPoint, &renderingPass);

        
        {
            LNMatrix transform;
            LNMatrix_SetIdentity(&transform);
            transform.m41 = width / 2;
            transform.m42 = height / 2;

            LNDrawSpriteParams params = {};
            params.worldTransform = transform;
            params.material = material1;
            params.size = {static_cast<float>(width - 20), static_cast<float>(height - 20)};
            params.anchorRatio = {0.5f, 0.5f};
            params.uvRect = {0, 0, 1, 1};
            params.color = {1, 1, 1, 1};
            params.baseDirection = LN_SPRITE_BASE_DIRECTION_BASIC2D;
            params.billboardType = LN_BILLBOARD_TYPE_NONE;
            LNSceneRenderPass_DrawSprite(renderingPass, &params);
        }

        LNGraphicsContext_EndSceneRenderPass(graphicsContext, renderingPass);

        LNGraphicsContext_EndFrame(graphicsContext);
        LNWindow_Present(window);

        frameCount++;
    };

    LNObject_Release(material1);
    LNObject_Release(viewPoint);
    LNObject_Release(window);
    LNInstance_Terminate();
    return 0;
}
