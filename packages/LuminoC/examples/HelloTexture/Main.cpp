/**
 * PNG 画像を LNTexture2D_LoadFromFile で読み込み、四角形メッシュに貼り付けて描画するデモ。
 */
#include "../Utils.h"

int main(void) {
    InitializeInstance();

    // Window + GraphicsContext
    LNHandle window = LN_NULL_HANDLE;
    LNWindow_Create("LuminoC-HelloTexture", 1280, 720, &window);
    LNHandle graphicsContext = LN_NULL_HANDLE;
    LNWindow_GetGraphicsContext(window, &graphicsContext);

    // Texture
    LNHandle texture = LN_NULL_HANDLE;
    LNTexture2D_LoadFromFile(graphicsContext, ASSETS_DIR "/picture1.png", &texture);

    // Unlit Material
    LNHandle material = LN_NULL_HANDLE;
    LNMaterial_CreateUnlit(graphicsContext, &material);
    LNMaterial_SetMainTexture(material, texture);

    // Quad mesh (4 vertices, 6 indices, CCW winding)
    //   v0(-0.5, 0.5) --- v1(0.5, 0.5)
    //      |           /      |
    //   v2(-0.5,-0.5) --- v3(0.5,-0.5)
    LNVertex vertices[4] = {
        // posX   posY   posZ   normX normY normZ  u    v    r    g    b    a    tanX tanY tanZ tanW
        { -0.5f,  0.5f,  0.0f,  0,0,1,  0.0f, 0.0f,  1,1,1,1,  1,0,0,0 }, // v0
        {  0.5f,  0.5f,  0.0f,  0,0,1,  1.0f, 0.0f,  1,1,1,1,  1,0,0,0 }, // v1
        { -0.5f, -0.5f,  0.0f,  0,0,1,  0.0f, 1.0f,  1,1,1,1,  1,0,0,0 }, // v2
        {  0.5f, -0.5f,  0.0f,  0,0,1,  1.0f, 1.0f,  1,1,1,1,  1,0,0,0 }, // v3
    };
    uint32_t indices[6] = { 0, 2, 1,  1, 2, 3 };  // CCW: upper tri + lower tri
    LNSubMesh sub = { 0, 6, 0 };

    LNHandle mesh = LN_NULL_HANDLE;
    LNMesh_Create(graphicsContext, vertices, 4, indices, 6, &sub, 1, &mesh);
    LNMesh_SetMaterial(mesh, 0, material);

    // Perspective camera
    LNHandle camera = LN_NULL_HANDLE;
    LNCamera_Create(&camera);
    LNCamera_SetPerspective(camera,
        60.0f * 3.14159f / 180.0f,
        1280.0f / 720.0f,
        0.1f, 100.0f);
    LNCamera_SetLookAt(camera,
        0.0f, 0.0f, 3.0f,   // eye
        0.0f, 0.0f, 0.0f,   // target
        0.0f, 1.0f, 0.0f);  // up
    
    // Main loop
    LNGraphicsProfilering profilering = {};
    LNTransform identity = { 0,0,0,  0,0,0,1,  1,1,1 };
    LNBool quit = LN_FALSE;
    while (LNWindow_ProcessEvents(window, &quit) == LN_OK && !quit) {
        LNHandle renderer, colorBuffer, depthBuffer;
        LNGraphicsContext_BeginFrame(graphicsContext, &renderer, &colorBuffer, &depthBuffer);
        LNRenderPassDesc rpDesc;
        LNRenderPassDesc_Init(&rpDesc);
        rpDesc.colorAttachments[0].clearColor[0] = 0.60f;
        rpDesc.colorAttachments[0].clearColor[1] = 0.85f;
        rpDesc.colorAttachments[0].clearColor[2] = 0.60f;
        rpDesc.colorAttachments[0].clearColor[3] = 1.0f;
        rpDesc.colorAttachments[0].loadOp = LN_LOAD_OP_CLEAR;
        LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, camera);
        LNRenderer_DrawMesh(renderer, mesh, &identity, 0);
        LNRenderer_EndRenderPass(renderer);

        LNDebug_GetGraphicsProfiler(graphicsContext, &profilering);
        LNDebug_Print(graphicsContext, (std::string("FPS: ") + std::to_string(profilering.fps)).c_str());
        LNDebug_Print(graphicsContext, (std::string("FrameTime(ms): ") + std::to_string(profilering.lastFrameTimeMs)).c_str());
        LNDebug_Print(graphicsContext, (std::string("DrawCall: ") + std::to_string(profilering.drawCallCount)).c_str());

        LNGraphicsContext_EndFrame(graphicsContext);
    }

    // Cleanup
    LNObject_Release(mesh);
    LNObject_Release(material);
    LNObject_Release(texture);
    LNObject_Release(camera);
    LNObject_Release(window);
    LNInstance_Terminate();

    return 0;
}
