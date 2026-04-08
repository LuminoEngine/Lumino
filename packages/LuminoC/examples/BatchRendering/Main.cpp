/**
 * BatchRendering example
 *
 * Demonstrates transparent batch rendering via LNRenderer_DrawSprite
 * and LNRenderer_DrawMesh. 1024 sprites with 2 alternating materials
 * are automatically batched and drawn with minimal draw calls.
 * A mesh is also drawn via the same Renderer.
 *
 * Compare with the old BatchSprite example where the client had to
 * manage sorting, vertex generation, and dynamic mesh updates manually.
 */
#include "../Utils.h"
#include <stdio.h>
#include <math.h>

#define WINDOW_W 1280
#define WINDOW_H 720
#define SPRITE_COUNT 1024

int main(void) {
    /* --- Initialization --- */
    LNInstanceInitializeSettings settings = {};
    settings.enableValidation = LN_TRUE;
    LNInstance_Initialize(&settings);

    LNHandle window = LN_NULL_HANDLE;
    LNWindow_Create("BatchRendering", WINDOW_W, WINDOW_H, &window);

    LNHandle graphicsContext = LN_NULL_HANDLE;
    LNWindow_GetGraphicsContext(window, &graphicsContext);

    /* Textures */
    LNHandle texture0 = LN_NULL_HANDLE;
    LNHandle texture1 = LN_NULL_HANDLE;
    LNTexture2D_LoadFromFile(graphicsContext, ASSETS_DIR "/picture1.png", &texture0);
    LNTexture2D_LoadFromFile(graphicsContext, ASSETS_DIR "/picture1.png", &texture1);

    /* Materials — two unlit materials */
    LNHandle material0 = LN_NULL_HANDLE;
    LNHandle material1 = LN_NULL_HANDLE;
    LNMaterial_CreateUnlit(graphicsContext, &material0);
    LNMaterial_SetMainTexture(material0, texture0);
    LNMaterial_SetColor(material0, 1.0f, 0.8f, 0.8f, 1.0f);
    LNMaterial_CreateUnlit(graphicsContext, &material1);
    LNMaterial_SetMainTexture(material1, texture1);
    LNMaterial_SetColor(material1, 0.8f, 0.8f, 1.0f, 1.0f);

    /* Camera (orthographic, origin at center) */
    LNHandle camera = LN_NULL_HANDLE;
    LNCamera_Create(&camera);
    LNCamera_SetOrthographic(camera, (float)WINDOW_W, (float)WINDOW_H, -1000.0f, 1000.0f);
    LNCamera_SetLookAt(camera,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f);

    /* A simple quad mesh to demonstrate mixed sprite + mesh rendering */
    LNVertex quadVerts[4] = {};
    /* Top-left */
    quadVerts[0].posX = -50.0f; quadVerts[0].posY =  50.0f; quadVerts[0].posZ = 0.0f;
    quadVerts[0].normZ = 1.0f; quadVerts[0].tanX = 1.0f;
    quadVerts[0].u = 0.0f; quadVerts[0].v = 0.0f;
    quadVerts[0].colorR = 1; quadVerts[0].colorG = 1; quadVerts[0].colorB = 1; quadVerts[0].colorA = 1;
    /* Top-right */
    quadVerts[1].posX =  50.0f; quadVerts[1].posY =  50.0f; quadVerts[1].posZ = 0.0f;
    quadVerts[1].normZ = 1.0f; quadVerts[1].tanX = 1.0f;
    quadVerts[1].u = 1.0f; quadVerts[1].v = 0.0f;
    quadVerts[1].colorR = 1; quadVerts[1].colorG = 1; quadVerts[1].colorB = 1; quadVerts[1].colorA = 1;
    /* Bottom-left */
    quadVerts[2].posX = -50.0f; quadVerts[2].posY = -50.0f; quadVerts[2].posZ = 0.0f;
    quadVerts[2].normZ = 1.0f; quadVerts[2].tanX = 1.0f;
    quadVerts[2].u = 0.0f; quadVerts[2].v = 1.0f;
    quadVerts[2].colorR = 1; quadVerts[2].colorG = 1; quadVerts[2].colorB = 1; quadVerts[2].colorA = 1;
    /* Bottom-right */
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

    /* --- Main loop --- */
    int frame = 0;
    int continueLoop = 1;

    while (LNWindow_ProcessEvents(window, &continueLoop) == LN_OK && continueLoop) {
        float t = frame * 0.02f;

        /* Render */
        LNHandle renderer = LN_NULL_HANDLE;
        LNGraphicsContext_BeginFrame(graphicsContext, &renderer);
        LNRenderer_BeginRenderPass(renderer, graphicsContext, camera,
            0.15f, 0.15f, 0.2f, 1.0f);

        /* Add sprites */
        int cols = 32;
        float spacing = 18.0f;
        float offsetX = -(cols * spacing) * 0.5f;
        float offsetY = -(((float)SPRITE_COUNT / cols) * spacing) * 0.5f;

        for (int i = 0; i < SPRITE_COUNT; i++) {
            int col = i % cols;
            int row = i / cols;
            float x = offsetX + col * spacing;
            float y = offsetY + row * spacing;
            int zIndex = (int)(sinf(t + i * 0.1f) * 10.0f);
            LNHandle mat = (i % 2 == 0) ? material0 : material1;

            LNRenderer_DrawSprite(
                renderer, mat, zIndex,
                x, y, 0.0f,
                16.0f, 16.0f,
                0.0f, 0.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f, 1.0f,
                0.0f);
        }

        /* Add mesh */
        LNTransform meshTransform = { 200.0f, 200.0f, 0.0f,  0,0,0,1,  1,1,1 };
        LNRenderer_DrawMesh(renderer, mesh, &meshTransform, 0);

        LNRenderer_EndRenderPass(renderer);

        printGraphicsProfilering(graphicsContext);

        LNGraphicsContext_EndFrame(graphicsContext);

        frame++;
    }

    /* --- Cleanup --- */
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
