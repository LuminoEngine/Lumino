#include <stdio.h>
#include <vector>
#include <lumino.h>

int main() {
    LNResult result = LNInstance_Initialize();
    if (result != LN_OK) {
        return 1;
    }

    LNHandle window = LN_NULL_HANDLE;
    LNWindow_Create(640, 480, "Example", &window);

    LNHandle graphicsContext = LN_NULL_HANDLE;
    LNWindow_GetGraphicsContext(window, &graphicsContext);

    LNHandle commandList = LN_NULL_HANDLE;
    LNCommandList_Get(graphicsContext, &commandList);

    LNHandle viewPoint1 = LN_NULL_HANDLE;
    LNCamera_Create(&viewPoint1);

    while (true) {
        LNBool quit = LN_FALSE;
        LNInstance_ShouldQuit(&quit);
        if (quit) {
            break;
        }
        LNInstance_ProcessEvents();

        // TODO: get backbuffer size
        int width = 640;
        int height = 480;
        LNCamera_SetupPerspective2D(viewPoint1, 0, 0, 0, width, height, -500, 500);

        LNHandle backbuffer = LN_NULL_HANDLE;
        LNHandle depthBuffer = LN_NULL_HANDLE;
        LNGraphicsContext_GetCurrentColorBuffer(graphicsContext, &backbuffer);
        LNGraphicsContext_GetCurrentDepthBuffer(graphicsContext, &depthBuffer);

        LNCommandList_Reset(commandList);

        // x. レンダーターゲットをクリアするための RenderPass を開始します。
        //    なお WebGPU などと同様、 RenderPass を開始することなくクリアする方法はありません。
        LNHandle renderingPass = LN_NULL_HANDLE;
        LNRenderPassDescriptor descriptor;
        descriptor.renderTargets[0].renderTarget = backbuffer;
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
        LNCommandList_BeginRenderPass(commandList, descriptor, viewPoint1, &renderingPass);
        LNRenderPass_End(renderingPass);

        LNGraphicsContext_SubmitCommandList(graphicsContext, commandList);
        LNWindow_Present(window);
    };

    LNObject_Release(window);
    LNInstance_Terminate();
    return 0;
}
