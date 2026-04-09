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
#include <LuminoC/lumino.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------
#define WINDOW_W 1280
#define WINDOW_H 720
#define SPRITE_COUNT 1024
#define MAX_VERTICES (SPRITE_COUNT * 4)
#define MAX_INDICES  (SPRITE_COUNT * 6)
#define SPRITE_SIZE  16.0f

//------------------------------------------------------------------------------
// Sprite data (client-side, what JS would manage)
//------------------------------------------------------------------------------
typedef struct Sprite {
    float x, y;
    float w, h;
    float uvL, uvT, uvW, uvH;
    float r, g, b, a;
    int zIndex;
    int textureSlot; /* 0 or 1 — determines which material/texture to use */
} Sprite;

static Sprite s_sprites[SPRITE_COUNT];

//------------------------------------------------------------------------------
// Comparison function for qsort (zIndex first, then textureSlot for batching)
//------------------------------------------------------------------------------
static int compareSprites(const void* a, const void* b) {
    const Sprite* sa = (const Sprite*)a;
    const Sprite* sb = (const Sprite*)b;
    if (sa->zIndex != sb->zIndex) return sa->zIndex - sb->zIndex;
    return sa->textureSlot - sb->textureSlot;
}

//------------------------------------------------------------------------------
// Initialize sprites in a grid pattern
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
        s_sprites[i].zIndex = row;              /* z sort by row */
        s_sprites[i].textureSlot = (i % 2);     /* alternate textures */
    }
}

//------------------------------------------------------------------------------
// Build vertex/index data and submeshes from sorted sprites
//------------------------------------------------------------------------------
static LNVertex s_vertexBuf[MAX_VERTICES];
static uint32_t s_indexBuf[MAX_INDICES];
static LNSubMesh s_submeshBuf[SPRITE_COUNT]; /* worst case: 1 submesh per sprite */

static uint32_t buildBatches(
    const Sprite* sprites,
    int spriteCount,
    LNVertex* outVerts,
    uint32_t* outIndices,
    LNSubMesh* outSubs,
    uint32_t* outVertCount,
    uint32_t* outIdxCount) {

    uint32_t vi = 0; /* vertex index */
    uint32_t ii = 0; /* index index */
    uint32_t batchCount = 0;
    int batchStart = 0;

    for (int i = 0; i <= spriteCount; i++) {
        /* Detect batch boundary: end of array or texture change */
        if (i == spriteCount ||
            (i > batchStart && sprites[i].textureSlot != sprites[i - 1].textureSlot)) {

            /* Record submesh for the batch [batchStart .. i) */
            outSubs[batchCount].indexOffset = (uint32_t)(batchStart * 6);
            outSubs[batchCount].indexCount  = (uint32_t)((i - batchStart) * 6);
            outSubs[batchCount].materialIndex = (uint32_t)sprites[batchStart].textureSlot;
            batchCount++;
            batchStart = i;
        }
        if (i == spriteCount) break;

        /* Emit 4 vertices for this sprite (quad) */
        const Sprite* s = &sprites[i];
        float hw = s->w * 0.5f;
        float hh = s->h * 0.5f;
        float u0 = s->uvL, v0 = s->uvT;
        float u1 = s->uvL + s->uvW, v1 = s->uvT + s->uvH;

        /* v0: top-left,  v1: top-right, v2: bottom-left, v3: bottom-right */
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

        /* indices: 0,2,1, 1,2,3 (CCW) */
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
int main(void) {
    /* 1. Initialize */
    LNInstanceInitializeSettings settings = {};
    settings.enableValidation = LN_TRUE;
    LNInstance_Initialize(&settings);

    LNHandle window = LN_NULL_HANDLE;
    LNWindow_Create("LuminoC-BatchSprite", WINDOW_W, WINDOW_H, &window);
    LNHandle graphicsContext = LN_NULL_HANDLE;
    LNWindow_GetGraphicsContext(window, &graphicsContext);

    /* 2. Textures — load the same image for both slots (demo purposes) */
    LNHandle texture0 = LN_NULL_HANDLE;
    LNHandle texture1 = LN_NULL_HANDLE;
    LNTexture2D_LoadFromFile(graphicsContext, ASSETS_DIR "/picture1.png", &texture0);
    LNTexture2D_LoadFromFile(graphicsContext, ASSETS_DIR "/picture1.png", &texture1);

    /* 3. Materials — two unlit materials with different tints */
    LNHandle material0 = LN_NULL_HANDLE;
    LNHandle material1 = LN_NULL_HANDLE;
    LNMaterial_CreateUnlit(graphicsContext, &material0);
    LNMaterial_SetMainTexture(material0, texture0);
    LNMaterial_CreateUnlit(graphicsContext, &material1);
    LNMaterial_SetMainTexture(material1, texture1);

    /* 4. Dynamic mesh */
    LNHandle mesh = LN_NULL_HANDLE;
    LNMesh_CreateDynamic(graphicsContext, MAX_VERTICES, MAX_INDICES, &mesh);

    /* Pre-assign materials to slot 0 and 1 */
    /* We need at least one submesh to size the material array initially.
       SetSubMeshes will expand as needed, but let's set materials after first build. */

    /* 5. Orthographic camera (pixel-space: origin at center) */
    LNHandle camera = LN_NULL_HANDLE;
    LNCamera_Create(&camera);
    LNCamera_SetOrthographic(camera, (float)WINDOW_W, (float)WINDOW_H, -1000.0f, 1000.0f);
    LNCamera_SetLookAt(camera,
        0.0f, 0.0f, 1.0f,   /* eye */
        0.0f, 0.0f, 0.0f,   /* target */
        0.0f, 1.0f, 0.0f);  /* up */

    /* 6. Initialize sprites */
    initSprites();

    printf("Lumino BatchSprite: %d sprites. Rendering...\n", SPRITE_COUNT);

    /* 7. Main loop */
    LNTransform identity = { 0,0,0,  0,0,0,1,  1,1,1 };
    int frame = 0;
    LNBool quit = LN_FALSE;
    int materialsAssigned = 0;

    while (LNWindow_ProcessEvents(window, &quit) == LN_OK && !quit) {
        /* Animate: slight wobble */
        float t = frame * 0.02f;
        for (int i = 0; i < SPRITE_COUNT; i++) {
            s_sprites[i].zIndex = (int)(sinf(t + i * 0.1f) * 10.0f);
        }

        /* Sort sprites (z-index primary, texture secondary for batch efficiency) */
        qsort(s_sprites, SPRITE_COUNT, sizeof(Sprite), compareSprites);

        /* Build vertex data and batches */
        uint32_t vertCount = 0, idxCount = 0;
        uint32_t batchCount = buildBatches(
            s_sprites, SPRITE_COUNT,
            s_vertexBuf, s_indexBuf, s_submeshBuf,
            &vertCount, &idxCount);

        /* Upload to dynamic mesh */
        LNMesh_UpdateVertices(mesh, 0, s_vertexBuf, vertCount);
        LNMesh_UpdateIndices(mesh, 0, s_indexBuf, idxCount);
        LNMesh_SetSubMeshes(mesh, s_submeshBuf, batchCount);

        /* Assign materials (safe to call every frame; cheap) */
        if (!materialsAssigned) {
            LNMesh_SetMaterial(mesh, 0, material0);
            LNMesh_SetMaterial(mesh, 1, material1);
            materialsAssigned = 1;
        }

        /* Draw */
        LNHandle renderer, colorBuffer, depthBuffer;
        LNGraphicsContext_BeginFrame(graphicsContext, &renderer, &colorBuffer, &depthBuffer);
        LNRenderer_BeginRenderPass(renderer, graphicsContext, camera,
            0.15f, 0.15f, 0.2f, 1.0f);
        LNRenderer_DrawMesh(renderer, mesh, &identity, 0);
        LNRenderer_EndRenderPass(renderer);
        LNGraphicsContext_EndFrame(graphicsContext);

        frame++;
    }

    /* 8. Cleanup */
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
