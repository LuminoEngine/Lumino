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
    
    LNResult result = LNInstance_Initialize();
    if (result != LN_OK) {
        return 1;
    }

    LNHandle window = LN_NULL_HANDLE;
    LNHandle graphicsContext = LN_NULL_HANDLE;
    LNHandle viewPoint = LN_NULL_HANDLE;
    LNWindow_Create(640, 480, "RenderItem Example", &window);
    LNWindow_GetGraphicsContext(window, &graphicsContext);
    LNViewPoint_Create(&viewPoint);

    // テクスチャの読み込み
    std::string assetsDir = ASSETS_DIR;
    std::vector<uint8_t> imageData = ReadAllBytes(assetsDir + "/icon256.png");
    LNHandle texture1 = LN_NULL_HANDLE;
    if (LNTexture2D_CreateFromImageFileData(imageData.data(), imageData.size(), &texture1) != LN_OK) {
        return 1;
    }

    // マテリアルの作成
    LNHandle material1 = LN_NULL_HANDLE;
    LNMaterial_Create(&material1);
    LNMaterial_SetMainTexture(material1, texture1);

    // RenderItem の作成と設定 (事前に作成しておく)
    LNHandle renderItem = LN_NULL_HANDLE;
    LNRenderItem_Create(&renderItem);
    
    // スプライトを追加
    LNVector2 size = {100.0f, 100.0f};
    LNVector2 anchorRatio = {0.5f, 0.5f};
    LNRect uvRect = {0.0f, 0.0f, 1.0f, 1.0f};
    LNRenderItem_AddSprite(renderItem, &size, texture1, &uvRect, &anchorRatio);
    
    // マテリアルと基本設定
    LNRenderItem_SetMaterial(renderItem, material1);
    LNRenderItem_SetBillboardType(renderItem, LN_BILLBOARD_TYPE_NONE);
    LNRenderItem_SetBaseDirection(renderItem, LN_SPRITE_BASE_DIRECTION_BASIC2D);
    
    // 色を設定
    LNColor white = {1.0f, 1.0f, 1.0f, 1.0f};
    LNRenderItem_SetColor(renderItem, &white);

    // 2つ目の RenderItem (別の位置に表示)
    LNHandle renderItem2 = LN_NULL_HANDLE;
    LNRenderItem_Create(&renderItem2);
    LNRenderItem_AddSprite(renderItem2, &size, texture1, &uvRect, &anchorRatio);
    LNRenderItem_SetMaterial(renderItem2, material1);
    LNRenderItem_SetBillboardType(renderItem2, LN_BILLBOARD_TYPE_NONE);
    LNRenderItem_SetBaseDirection(renderItem2, LN_SPRITE_BASE_DIRECTION_BASIC2D);
    LNColor tint = {1.0f, 0.5f, 0.5f, 1.0f}; // 赤みがかった色
    LNRenderItem_SetColor(renderItem2, &tint);

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

        LNHandle renderingPass = LN_NULL_HANDLE;
        LNRenderPassDescriptor descriptor;
        descriptor.renderTargets[0].renderTarget = colorBuffer;
        descriptor.renderTargets[0].clearColor[0] = 0.20f;
        descriptor.renderTargets[0].clearColor[1] = 0.25f;
        descriptor.renderTargets[0].clearColor[2] = 0.30f;
        descriptor.renderTargets[0].clearColor[3] = 1.0f;
        descriptor.renderTargets[0].clearEnable = LN_TRUE;
        descriptor.depthBuffer.depthBuffer = depthBuffer;
        descriptor.depthBuffer.clearDepth = 1.0f;
        descriptor.depthBuffer.clearStencil = 0;
        descriptor.depthBuffer.clearDepthEnable = LN_TRUE;
        descriptor.depthBuffer.clearStencilEnable = LN_TRUE;
        LNGraphicsContext_BeginSceneRenderPass(graphicsContext, descriptor, viewPoint, &renderingPass);
        
        // RenderItem の位置を更新 (毎フレーム円運動)
        float x1 = 320.0f + 100.0f * cosf(0.05f * frameCount);
        float y1 = 240.0f + 100.0f * sinf(0.05f * frameCount);
        LNRenderItem_SetPosition(renderItem, x1, y1, 0.0f);
        
        // 2つ目の RenderItem は逆回転
        float x2 = 320.0f + 100.0f * cosf(-0.05f * frameCount + 3.14159f);
        float y2 = 240.0f + 100.0f * sinf(-0.05f * frameCount + 3.14159f);
        LNRenderItem_SetPosition(renderItem2, x2, y2, 0.0f);
        
        // 回転も設定 (Z軸回転)
        LNRenderItem_SetRotation(renderItem, 0.0f, 0.0f, frameCount * 2.0f);
        LNRenderItem_SetRotation(renderItem2, 0.0f, 0.0f, -frameCount * 2.0f);
        
        // RenderItem を描画 (実際の描画内容は事前に設定済み)
        LNSceneRenderPass_DrawRenderItem(renderingPass, renderItem);
        LNSceneRenderPass_DrawRenderItem(renderingPass, renderItem2);
        
        LNGraphicsContext_EndSceneRenderPass(graphicsContext, renderingPass);
        LNGraphicsContext_EndFrame(graphicsContext);
        LNWindow_Present(window);

        frameCount++;
    }

    // リソースの解放
    LNObject_Release(renderItem);
    LNObject_Release(renderItem2);
    LNObject_Release(material1);
    LNObject_Release(texture1);
    LNObject_Release(viewPoint);
    LNObject_Release(window);
    LNInstance_Terminate();
    return 0;
}
