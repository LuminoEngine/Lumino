#include <stdio.h>
#include <vector>
#include <string>
#include <lumino.h>

static size_t GetFileSize(FILE* stream) {
    struct stat stbuf;
    int handle = fileno(stream);
    if (handle == 0) return 0;
    if (fstat(handle, &stbuf) == -1) return 0;
    return stbuf.st_size;
}

std::vector<uint8_t> ReadAllBytes(const std::string& filePath) {
    FILE* file = fopen(filePath.c_str(), "rb");
    if (!file) {
        printf("Error: %s\n", filePath.c_str());
        return {};
    }
    size_t size = GetFileSize(file);
    std::vector<uint8_t> buffer(size);
    fread(buffer.data(), 1, size, file);
    fclose(file);
    return buffer;
}

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

    
    std::string assetsDir = ASSETS_DIR;
    std::vector<uint8_t> imageData = ReadAllBytes(assetsDir  + "/icon256.png");
    LNHandle texture1 = LN_NULL_HANDLE;
    if (LNTexture2D_CreateFromImageFileData(imageData.data(), imageData.size(), &texture1) != LN_OK) {
        return 1;
    }

    LNHandle material1 = LN_NULL_HANDLE;
    LNMaterial_Create(&material1);
    LNMaterial_SetMainTexture(material1, texture1);

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
        LNHandle commandList = LN_NULL_HANDLE;
        LNWindow_GetFramebufferSize(window, &width, &height);
        LNGraphicsContext_BeginFrame(graphicsContext, width, height, &colorBuffer, &depthBuffer, &commandList);

        LNViewPoint_SetupOrtho2D(viewPoint, 0, 0, 0, width, height, -500, 500);

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
        LNCommandList_BeginSceneRenderPass(commandList, descriptor, viewPoint, &renderingPass);


        LNDrawSpriteParams params = {};
        params.material = material1;
        params.size = {100, 100};
        params.anchorRatio = {0, 0}; //{0.5f, 0.5f};
        params.uvRect = {0, 0, 1, 1};
        params.color = {1, 1, 1, 1};
        params.baseDirection = LN_SPRITE_BASE_DIRECTION_BASIC2D;
        params.billboardType = LN_BILLBOARD_TYPE_NONE;
        LNSceneRenderPass_DrawSprite(renderingPass, &params);
        
        //{
        //    LNMatrix transform;
        //    LNMatrix_SetIdentity(&transform);
        //    transform.m41 = 320 + (100.0f * cosf(0.05f * frameCount));
        //    transform.m42 = 240 + (100.0f * sinf(0.05f * frameCount));
        //    LNBatchRenderer_BeginBatch(spriteRenderer, commandList, material1, &transform);
        //    LNBatchRenderer_DrawSprite_deprecated(spriteRenderer, NULL,
        //        100, 100,
        //        0,0,//0.5f, 0.5f,
        //        0, 0, 1, 1,
        //        1, 1, 1, 1,
        //        LN_SPRITE_BASE_DIRECTION_BASIC2D,
        //        LN_BILLBOARD_TYPE_NONE);
        //    LNBatchRenderer_EndBatch(spriteRenderer);
        //}

        LNCommandList_EndSceneRenderPass(commandList, renderingPass);

        LNGraphicsContext_EndFrame(graphicsContext);
        LNWindow_Present(window);

        frameCount++;
    };

    LNObject_Release(material1);
    LNObject_Release(texture1);
    LNObject_Release(viewPoint);
    LNObject_Release(window);
    LNInstance_Terminate();
    return 0;
}
