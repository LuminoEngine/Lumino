/**
 * HelloTexture.c
 *
 * PNG 画像を LNTexture2D_LoadFromFile で読み込み、四角形メッシュに貼り付けて描画するデモ。
 * packages/LuminoCore/examples/HelloTexture/Main.cpp の C-API 版。
 */
#include <LuminoC/lumino.h>
#include <stdio.h>

int main(void) {
    /* 1. Runtime initialize */
    LNInstance_Initialize();

    /* 2. Window + GraphicsContext */
    LNHandle window = LN_NULL_HANDLE;
    LNWindow_Create("LuminoC-HelloTexture", 1280, 720, &window);
    LNHandle graphicsContext = LN_NULL_HANDLE;
    LNWindow_GetGraphicsContext(window, &graphicsContext);

    /* 3. Renderer */
    LNHandle renderer = LN_NULL_HANDLE;
    LNGraphicsContext_GetRenderer(graphicsContext, &renderer);

    /* 4. Texture */
    LNHandle texture = LN_NULL_HANDLE;
    LNTexture2D_LoadFromFile(graphicsContext, ASSETS_DIR "/picture1.png", &texture);

    /* 5. Unlit Material */
    LNHandle material = LN_NULL_HANDLE;
    LNMaterial_CreateUnlit(graphicsContext, &material);
    LNMaterial_SetTexture(material, texture);

    /* 6. Quad mesh (4 vertices, 6 indices, CCW winding) */
    /*   v0(-0.5, 0.5) --- v1(0.5, 0.5)
     *      |           /      |
     *   v2(-0.5,-0.5) --- v3(0.5,-0.5)  */
    LNVertex vertices[4] = {
        /* posX   posY   posZ   normX normY normZ  u    v    r    g    b    a    tanX tanY tanZ tanW */
        { -0.5f,  0.5f,  0.0f,  0,0,1,  0.0f, 0.0f,  1,1,1,1,  1,0,0,0 }, /* v0 */
        {  0.5f,  0.5f,  0.0f,  0,0,1,  1.0f, 0.0f,  1,1,1,1,  1,0,0,0 }, /* v1 */
        { -0.5f, -0.5f,  0.0f,  0,0,1,  0.0f, 1.0f,  1,1,1,1,  1,0,0,0 }, /* v2 */
        {  0.5f, -0.5f,  0.0f,  0,0,1,  1.0f, 1.0f,  1,1,1,1,  1,0,0,0 }, /* v3 */
    };
    uint32_t indices[6] = { 0, 2, 1,  1, 2, 3 };  /* CCW: upper tri + lower tri */
    LNSubMesh sub = { 0, 6, 0 };

    LNHandle mesh = LN_NULL_HANDLE;
    LNMesh_Create(graphicsContext, vertices, 4, indices, 6, &sub, 1, &mesh);
    LNMesh_SetMaterial(mesh, 0, material);

    /* 7. Perspective camera */
    LNHandle camera = LN_NULL_HANDLE;
    LNCamera_Create(&camera);
    LNCamera_SetPerspective(camera,
        60.0f * 3.14159f / 180.0f,
        1280.0f / 720.0f,
        0.1f, 100.0f);
    LNCamera_SetLookAt(camera,
        0.0f, 0.0f, 3.0f,   /* eye */
        0.0f, 0.0f, 0.0f,   /* target */
        0.0f, 1.0f, 0.0f);  /* up */

    printf("Lumino HelloTexture (C) initialized. Rendering...\n");

    /* 8. Main loop */
    LNTransform identity = { 0,0,0,  0,0,0,1,  1,1,1 };
    int continueLoop = 1;
    while (LNWindow_ProcessEvents(window, &continueLoop) == LN_OK && continueLoop) {
        LNGraphicsContext_BeginFrame(graphicsContext);
        LNRenderer_BeginFrame(renderer);
        LNRenderer_BeginRenderPass(renderer, graphicsContext, camera, 0.0f, 0.0f, 0.0f, 1.0f);
        LNRenderer_DrawMesh(renderer, mesh, &identity);
        LNRenderer_EndRenderPass(renderer);
        LNRenderer_EndFrame(renderer);
        LNGraphicsContext_EndFrame(graphicsContext);
    }

    /* 9. Cleanup */
    LNObject_Release(mesh);
    LNObject_Release(material);
    LNObject_Release(texture);
    LNObject_Release(camera);
    LNObject_Release(renderer);
    LNObject_Release(graphicsContext);
    LNObject_Release(window);
    LNInstance_Terminate();

    printf("Done.\n");
    return 0;
}
