/**
 * BatchRendering サンプル
 *
 * LNRenderer_DrawSprite と LNRenderer_DrawMesh による透過的なバッチ描画のデモ。
 * 2 種類のマテリアルを交互に使う 1024 個のスプライトが自動的にバッチ化され、
 * 最小限のドローコールで描画される。
 * 同じ Renderer でメッシュも 1 つ描画する。
 *
 * クライアント側でソート、頂点生成、動的メッシュの更新を手動で行っていた
 * 旧 BatchSprite サンプルと比較のこと。
 */
#include "../Utils.h"

#define SPRITE_COUNT 1024

int main() {
    InitializeInstance();

    LNHandle window = LN_NULL_HANDLE;
    LNWindow_Create("BatchRendering", WINDOW_W, WINDOW_H, &window);

    LNHandle graphicsContext = LN_NULL_HANDLE;
    LNWindow_GetGraphicsContext(window, &graphicsContext);

    // テクスチャ
    LNHandle texture0 = LN_NULL_HANDLE;
    LNHandle texture1 = LN_NULL_HANDLE;
    LNTexture2D_LoadFromFile(graphicsContext, ASSETS_DIR "/picture1.png", &texture0);
    LNTexture2D_LoadFromFile(graphicsContext, ASSETS_DIR "/picture1.png", &texture1);

    // マテリアル - Unlit マテリアルを 2 つ
    LNHandle material0 = LN_NULL_HANDLE;
    LNHandle material1 = LN_NULL_HANDLE;
    LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &material0);
    LNMaterial_SetMainTexture(material0, texture0);
    LNMaterial_SetColor(material0, 1.0f, 0.8f, 0.8f, 1.0f);
    LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &material1);
    LNMaterial_SetMainTexture(material1, texture1);
    LNMaterial_SetColor(material1, 0.8f, 0.8f, 1.0f, 1.0f);

    // カメラ (正射影、原点は中央)
    LNHandle camera = LN_NULL_HANDLE;
    LNCamera_Create(&camera);
    LNCamera_SetOrthographic(camera, (float)WINDOW_W, (float)WINDOW_H, -1000.0f, 1000.0f);
    LNCamera_SetLookAt(camera,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f);

    // スプライトとメッシュの混在描画を示すための単純な四角形メッシュ
    LNVertex quadVerts[4] = {};
    // 左上
    quadVerts[0].posX = -50.0f; quadVerts[0].posY =  50.0f; quadVerts[0].posZ = 0.0f;
    quadVerts[0].normZ = 1.0f; quadVerts[0].tanX = 1.0f;
    quadVerts[0].u = 0.0f; quadVerts[0].v = 0.0f;
    quadVerts[0].colorR = 1; quadVerts[0].colorG = 1; quadVerts[0].colorB = 1; quadVerts[0].colorA = 1;
    // 右上
    quadVerts[1].posX =  50.0f; quadVerts[1].posY =  50.0f; quadVerts[1].posZ = 0.0f;
    quadVerts[1].normZ = 1.0f; quadVerts[1].tanX = 1.0f;
    quadVerts[1].u = 1.0f; quadVerts[1].v = 0.0f;
    quadVerts[1].colorR = 1; quadVerts[1].colorG = 1; quadVerts[1].colorB = 1; quadVerts[1].colorA = 1;
    // 左下
    quadVerts[2].posX = -50.0f; quadVerts[2].posY = -50.0f; quadVerts[2].posZ = 0.0f;
    quadVerts[2].normZ = 1.0f; quadVerts[2].tanX = 1.0f;
    quadVerts[2].u = 0.0f; quadVerts[2].v = 1.0f;
    quadVerts[2].colorR = 1; quadVerts[2].colorG = 1; quadVerts[2].colorB = 1; quadVerts[2].colorA = 1;
    // 右下
    quadVerts[3].posX =  50.0f; quadVerts[3].posY = -50.0f; quadVerts[3].posZ = 0.0f;
    quadVerts[3].normZ = 1.0f; quadVerts[3].tanX = 1.0f;
    quadVerts[3].u = 1.0f; quadVerts[3].v = 1.0f;
    quadVerts[3].colorR = 1; quadVerts[3].colorG = 1; quadVerts[3].colorB = 1; quadVerts[3].colorA = 1;

    uint32_t quadIndices[6] = { 0, 2, 1, 1, 2, 3 };
    LNSubMesh quadSub = { 0, 6, 0 };
    LNHandle mesh = LN_NULL_HANDLE;
    LNMesh_Create(graphicsContext, quadVerts, 4, quadIndices, 6, &quadSub, 1, &mesh);
    LNMesh_SetMaterial(mesh, 0, material0);

    printf("BatchRendering: %d sprites + 1 mesh. Rendering...\n", SPRITE_COUNT);

    // --- メインループ ---
    int frame = 0;
    LNBool quit = LN_FALSE;

    while (LNWindow_ProcessEvents(window, &quit) == LN_OK && !quit) {
        float t = frame * 0.02f;

        // 描画
        LNHandle renderer = LN_NULL_HANDLE, colorBuffer, depthBuffer;
        LNGraphicsContext_BeginFrame(graphicsContext, WINDOW_W, WINDOW_H, &renderer, &colorBuffer, &depthBuffer);
        LNRenderPassDesc rpDesc;
        LNRenderPassDesc_Init(&rpDesc);
        rpDesc.colorAttachments[0].clearColor[0] = 0.15f;
        rpDesc.colorAttachments[0].clearColor[1] = 0.15f;
        rpDesc.colorAttachments[0].clearColor[2] = 0.2f;
        rpDesc.colorAttachments[0].clearColor[3] = 1.0f;
        LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, camera);

        // スプライトを投入
        int cols = 32;
        float spacing = 18.0f;
        float offsetX = -(cols * spacing) * 0.5f;
        float offsetY = -(((float)SPRITE_COUNT / cols) * spacing) * 0.5f;

        // ノード全体で 1 つの行列を共有し、スプライトごとには offset (x, y) だけ渡す。
        // (タイルマップのように多数のスプライトを描く際の典型パターン)
        LNMatrix nodeXf = { { 1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1 } };
        for (int i = 0; i < SPRITE_COUNT; i++) {
            int col = i % cols;
            int row = i / cols;
            float x = offsetX + col * spacing;
            float y = offsetY + row * spacing;
            int zIndex = (int)(sinf(t + i * 0.1f) * 10.0f);
            LNHandle mat = (i % 2 == 0) ? material0 : material1;

            LNRenderer_DrawSprite(
                renderer, mat, zIndex,
                &nodeXf,
                x, y,
                16.0f, 16.0f,
                0.5f, 0.5f,
                0.0f, 0.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f, 1.0f);
        }

        // メッシュを投入
        LNTransform meshTransform = { 200.0f, 200.0f, 0.0f,  0,0,0,1,  1,1,1 };
        LNRenderer_DrawMesh(renderer, mesh, &meshTransform, 0);

        LNRenderer_EndRenderPass(renderer);

        printGraphicsProfilering(graphicsContext);

        LNGraphicsContext_EndFrame(graphicsContext);

        frame++;
    }

    // --- 解放 ---
    LNObject_Release(mesh);
    LNObject_Release(material1);
    LNObject_Release(material0);
    LNObject_Release(texture1);
    LNObject_Release(texture0);
    LNObject_Release(camera);
    LNObject_Release(window);
    LNInstance_Terminate();

    printf("Done. (%d frames)\n", frame);
    return 0;
}
