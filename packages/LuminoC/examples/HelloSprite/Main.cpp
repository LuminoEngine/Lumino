/**
 * HelloSprite サンプル
 *
 * LNCamera_SetOrthographic で正射影カメラを作成し、
 * LNRenderer_DrawSprite で画面中央に1枚のスプライトを描画する最小サンプル。
 */
#include "../Utils.h"

int main() {
    InitializeInstance();

    LNHandle window = LN_NULL_HANDLE;
    LNWindow_Create("LuminoC-HelloSprite", WINDOW_W, WINDOW_H, &window);

    LNHandle graphicsContext = LN_NULL_HANDLE;
    LNWindow_GetGraphicsContext(window, &graphicsContext);

    // テクスチャ
    LNHandle texture = LN_NULL_HANDLE;
    LNTexture2D_LoadFromFile(graphicsContext, ASSETS_DIR "/Redactor1.png", &texture);

    // Unlit マテリアル
    LNHandle material = LN_NULL_HANDLE;
    LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &material);
    LNMaterial_SetMainTexture(material, texture);
    LNMaterial_SetBlendMode(material, LN_BLEND_MODE_ALPHA);

    // 正射影カメラ (原点は画面中央)
    LNHandle camera = LN_NULL_HANDLE;
    LNCamera_Create(&camera);
    LNCamera_SetOrthographic(camera, (float)WINDOW_W, (float)WINDOW_H, -1000.0f, 1000.0f);
    LNCamera_SetLookAt(camera,
        0.0f, 0.0f, 1.0f,   // 視点
        0.0f, 0.0f, 0.0f,   // 注視点
        0.0f, 1.0f, 0.0f);  // 上方向

    printf("Lumino HelloSprite: drawing one sprite at the screen center.\n");

    LNMatrix identity = { { 1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1 } };

    int frame = 0;
    LNBool quit = LN_FALSE;
    while (LNWindow_ProcessEvents(window, &quit) == LN_OK && !quit) {
        LNHandle renderer = LN_NULL_HANDLE, colorBuffer, depthBuffer;
        LNGraphicsContext_BeginFrame(graphicsContext, WINDOW_W, WINDOW_H, &renderer, &colorBuffer, &depthBuffer);

        LNRenderPassDesc rpDesc;
        LNRenderPassDesc_Init(&rpDesc);
        rpDesc.colorAttachments[0].clearColor[0] = 0.15f;
        rpDesc.colorAttachments[0].clearColor[1] = 0.15f;
        rpDesc.colorAttachments[0].clearColor[2] = 0.2f;
        rpDesc.colorAttachments[0].clearColor[3] = 1.0f;
        LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, camera);

        // スプライトを 1 枚、画面中央に描画する (このカメラでは原点が画面中央)。
        LNRenderer_DrawSprite(
            renderer, material, 0,
            &identity,
            0.0f, 0.0f,         // offset (スプライトの位置)
            128.0f, 128.0f,     // サイズ
            0.5f, 0.5f,         // pivot (中央)
            0.0f, 0.0f, 1.0f, 1.0f, // uv
            1.0f, 1.0f, 1.0f, 1.0f); // 色

         LNRenderer_DrawSprite(
            renderer, material, 0,
            &identity,
            50.0f, 0.0f,         // offset (スプライトの位置)
            128.0f, 128.0f,     // サイズ
            0.5f, 0.5f,         // pivot (中央)
            0.0f, 0.0f, 1.0f, 1.0f, // uv
            1.0f, 1.0f, 1.0f, 1.0f); // 色

        LNRenderer_EndRenderPass(renderer);

        printGraphicsProfilering(graphicsContext);

        LNGraphicsContext_EndFrame(graphicsContext);
        frame++;
    }

    // Cleanup
    LNObject_Release(material);
    LNObject_Release(texture);
    LNObject_Release(camera);
    LNObject_Release(window);
    LNInstance_Terminate();

    printf("Done. (%d frames)\n", frame);
    return 0;
}
