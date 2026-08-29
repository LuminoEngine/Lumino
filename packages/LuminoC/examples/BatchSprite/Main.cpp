/**
 * BatchSprite.c
 *
 * 1000 個以上のスプライトを動的メッシュ (LNMesh_CreateDynamic) で
 * バッチ描画するデモ。JavaScript 側で SpriteBatch を実装する想定の
 * プロトタイプです。
 *
 * 処理の流れ:
 *   1. 動的メッシュを作成 (十分な頂点/インデックス容量を確保)
 *   2. 毎フレーム:
 *      a. スプライトを zIndex でソート
 *      b. 同一テクスチャの連続スプライトをバッチ化 → SubMesh に分割
 *      c. 頂点/インデックスデータを更新
 *      d. DrawMesh で描画
 */
#include "../Utils.h"

//------------------------------------------------------------------------------
// 定数
//------------------------------------------------------------------------------
#define SPRITE_COUNT 1024
#define MAX_VERTICES (SPRITE_COUNT * 4)
#define MAX_INDICES  (SPRITE_COUNT * 6)
#define SPRITE_SIZE  16.0f

//------------------------------------------------------------------------------
// スプライトデータ (クライアント側。JS が管理する想定のもの)
//------------------------------------------------------------------------------
typedef struct Sprite {
    float x, y;
    float w, h;
    float uvL, uvT, uvW, uvH;
    float r, g, b, a;
    int zIndex;
    int textureSlot; /* 0 か 1 - 使用するマテリアル/テクスチャを決める */
} Sprite;

static Sprite s_sprites[SPRITE_COUNT];

//------------------------------------------------------------------------------
// qsort 用の比較関数 (zIndex を優先し、次にバッチ化のため textureSlot で比較)
//------------------------------------------------------------------------------
static int compareSprites(const void* a, const void* b) {
    const Sprite* sa = (const Sprite*)a;
    const Sprite* sb = (const Sprite*)b;
    if (sa->zIndex != sb->zIndex) return sa->zIndex - sb->zIndex;
    return sa->textureSlot - sb->textureSlot;
}

//------------------------------------------------------------------------------
// スプライトを格子状に初期化する
//------------------------------------------------------------------------------
static void initSprites(void) {
    int cols = 32;
    float spacing = SPRITE_SIZE + 2.0f;
    float offsetX = -(cols * spacing) * 0.5f;
    float offsetY = -(((float)SPRITE_COUNT / cols) * spacing) * 0.5f;

    for (int i = 0; i < SPRITE_COUNT; i++) {
        int col = i % cols;
        int row = i / cols;
        s_sprites[i].x = offsetX + col * spacing;
        s_sprites[i].y = offsetY + row * spacing;
        s_sprites[i].w = SPRITE_SIZE;
        s_sprites[i].h = SPRITE_SIZE;
        s_sprites[i].uvL = 0.0f;
        s_sprites[i].uvT = 0.0f;
        s_sprites[i].uvW = 1.0f;
        s_sprites[i].uvH = 1.0f;
        s_sprites[i].r = 0.5f + 0.5f * (float)col / cols;
        s_sprites[i].g = 0.5f + 0.5f * (float)row / (SPRITE_COUNT / cols);
        s_sprites[i].b = 1.0f;
        s_sprites[i].a = 1.0f;
        s_sprites[i].zIndex = row;              /* 行ごとに z ソート */
        s_sprites[i].textureSlot = (i % 2);     /* テクスチャを交互に使う */
    }
}

//------------------------------------------------------------------------------
// ソート済みスプライトから頂点/インデックスデータとサブメッシュを構築する
//------------------------------------------------------------------------------
static LNVertex s_vertexBuf[MAX_VERTICES];
static uint32_t s_indexBuf[MAX_INDICES];
static LNSubMesh s_submeshBuf[SPRITE_COUNT]; /* 最悪ケース: スプライトごとに 1 サブメッシュ */

static uint32_t buildBatches(
    const Sprite* sprites,
    int spriteCount,
    LNVertex* outVerts,
    uint32_t* outIndices,
    LNSubMesh* outSubs,
    uint32_t* outVertCount,
    uint32_t* outIdxCount) {

    uint32_t vi = 0; /* 頂点インデックス */
    uint32_t ii = 0; /* インデックスのインデックス */
    uint32_t batchCount = 0;
    int batchStart = 0;

    for (int i = 0; i <= spriteCount; i++) {
        /* バッチ境界の検出: 配列の終端またはテクスチャの切り替わり */
        if (i == spriteCount ||
            (i > batchStart && sprites[i].textureSlot != sprites[i - 1].textureSlot)) {

            /* バッチ [batchStart .. i) のサブメッシュを記録 */
            outSubs[batchCount].indexOffset = (uint32_t)(batchStart * 6);
            outSubs[batchCount].indexCount  = (uint32_t)((i - batchStart) * 6);
            outSubs[batchCount].materialIndex = (uint32_t)sprites[batchStart].textureSlot;
            batchCount++;
            batchStart = i;
        }
        if (i == spriteCount) break;

        /* このスプライト (四角形) の頂点を 4 つ出力 */
        const Sprite* s = &sprites[i];
        float hw = s->w * 0.5f;
        float hh = s->h * 0.5f;
        float u0 = s->uvL, v0 = s->uvT;
        float u1 = s->uvL + s->uvW, v1 = s->uvT + s->uvH;

        /* v0: 左上,  v1: 右上, v2: 左下, v3: 右下 */
        LNVertex v;
        memset(&v, 0, sizeof(v));
        v.normZ = 1.0f;
        v.tanX = 1.0f;

        /* v0 */
        v.posX = s->x - hw; v.posY = s->y + hh; v.posZ = 0.0f;
        v.u = u0; v.v = v0;
        v.colorR = s->r; v.colorG = s->g; v.colorB = s->b; v.colorA = s->a;
        outVerts[vi + 0] = v;

        /* v1 */
        v.posX = s->x + hw; v.posY = s->y + hh;
        v.u = u1; v.v = v0;
        outVerts[vi + 1] = v;

        /* v2 */
        v.posX = s->x - hw; v.posY = s->y - hh;
        v.u = u0; v.v = v1;
        outVerts[vi + 2] = v;

        /* v3 */
        v.posX = s->x + hw; v.posY = s->y - hh;
        v.u = u1; v.v = v1;
        outVerts[vi + 3] = v;

        /* インデックス: 0,2,1, 1,2,3 (CCW) */
        outIndices[ii + 0] = vi + 0;
        outIndices[ii + 1] = vi + 2;
        outIndices[ii + 2] = vi + 1;
        outIndices[ii + 3] = vi + 1;
        outIndices[ii + 4] = vi + 2;
        outIndices[ii + 5] = vi + 3;

        vi += 4;
        ii += 6;
    }

    *outVertCount = vi;
    *outIdxCount = ii;
    return batchCount;
}

//------------------------------------------------------------------------------
// Main
//------------------------------------------------------------------------------
int main() {
    InitializeInstance();

    LNHandle window = LN_NULL_HANDLE;
    LNWindow_Create("LuminoC-BatchSprite", WINDOW_W, WINDOW_H, &window);
    LNHandle graphicsContext = LN_NULL_HANDLE;
    LNWindow_GetGraphicsContext(window, &graphicsContext);

    // 2. テクスチャ - 両スロットに同じ画像を読み込む (デモのため)
    LNHandle texture0 = LN_NULL_HANDLE;
    LNHandle texture1 = LN_NULL_HANDLE;
    LNTexture2D_LoadFromFile(graphicsContext, ASSETS_DIR "/picture1.png", &texture0);
    LNTexture2D_LoadFromFile(graphicsContext, ASSETS_DIR "/picture1.png", &texture1);

    // 3. マテリアル - 色味の異なる Unlit マテリアルを 2 つ
    LNHandle material0 = LN_NULL_HANDLE;
    LNHandle material1 = LN_NULL_HANDLE;
    LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &material0);
    LNMaterial_SetMainTexture(material0, texture0);
    LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &material1);
    LNMaterial_SetMainTexture(material1, texture1);

    // 4. 動的メッシュ
    LNHandle mesh = LN_NULL_HANDLE;
    LNMesh_CreateDynamic(graphicsContext, MAX_VERTICES, MAX_INDICES, &mesh);

    // スロット 0 と 1 にマテリアルを事前に割り当てる
    // マテリアル配列のサイズを決めるには、最初に 1 つ以上のサブメッシュが必要。
    // SetSubMeshes が必要に応じて拡張するので、マテリアルは最初のビルド後に設定する。

    // 5. 正射影カメラ (ピクセル空間。原点は中央)
    LNHandle camera = LN_NULL_HANDLE;
    LNCamera_Create(&camera);
    LNCamera_SetOrthographic(camera, (float)WINDOW_W, (float)WINDOW_H, -1000.0f, 1000.0f);
    LNCamera_SetLookAt(camera,
        0.0f, 0.0f, 1.0f,   // 視点
        0.0f, 0.0f, 0.0f,   // 注視点
        0.0f, 1.0f, 0.0f);  // 上方向

    // 6. スプライトを初期化
    initSprites();

    printf("Lumino BatchSprite: %d sprites. Rendering...\n", SPRITE_COUNT);

    // 7. メインループ
    LNTransform identity = { 0,0,0,  0,0,0,1,  1,1,1 };
    int frame = 0;
    LNBool quit = LN_FALSE;
    int materialsAssigned = 0;

    while (LNWindow_ProcessEvents(window, &quit) == LN_OK && !quit) {
        // アニメーション: わずかに揺らす
        float t = frame * 0.02f;
        for (int i = 0; i < SPRITE_COUNT; i++) {
            s_sprites[i].zIndex = (int)(sinf(t + i * 0.1f) * 10.0f);
        }

        // スプライトをソート (zIndex を主キー、バッチ効率のためテクスチャを副キーに)
        qsort(s_sprites, SPRITE_COUNT, sizeof(Sprite), compareSprites);

        // 頂点データとバッチを構築
        uint32_t vertCount = 0, idxCount = 0;
        uint32_t batchCount = buildBatches(
            s_sprites, SPRITE_COUNT,
            s_vertexBuf, s_indexBuf, s_submeshBuf,
            &vertCount, &idxCount);

        // 動的メッシュへアップロード
        LNMesh_UpdateVertices(mesh, 0, s_vertexBuf, vertCount);
        LNMesh_UpdateIndices(mesh, 0, s_indexBuf, idxCount);
        LNMesh_SetSubMeshes(mesh, s_submeshBuf, batchCount);

        // マテリアルを割り当て (毎フレーム呼んでも安全で軽い)
        if (!materialsAssigned) {
            LNMesh_SetMaterial(mesh, 0, material0);
            LNMesh_SetMaterial(mesh, 1, material1);
            materialsAssigned = 1;
        }

        // 描画
        LNHandle renderer, colorBuffer, depthBuffer;
        LNGraphicsContext_BeginFrame(graphicsContext, WINDOW_W, WINDOW_H, &renderer, &colorBuffer, &depthBuffer);
        LNRenderPassDesc rpDesc;
        LNRenderPassDesc_Init(&rpDesc);
        rpDesc.colorAttachments[0].clearColor[0] = 0.15f;
        rpDesc.colorAttachments[0].clearColor[1] = 0.15f;
        rpDesc.colorAttachments[0].clearColor[2] = 0.2f;
        rpDesc.colorAttachments[0].clearColor[3] = 1.0f;
        LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, camera);
        LNRenderer_DrawMesh(renderer, mesh, &identity, 0);
        LNRenderer_EndRenderPass(renderer);
        LNGraphicsContext_EndFrame(graphicsContext);

        frame++;
    }

    // 8. 解放
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
