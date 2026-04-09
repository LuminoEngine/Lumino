/**
 * CustomShader.c
 *
 * コンパイル済みシェーダ (.lcsh) をファイルから読み込み、
 * LNMaterial_CreateFromCompiledShader で Material を作成して
 * 赤い三角形を描画するデモ。
 */
#include <string>
#include <vector>
#include <LuminoC/lumino.h>
#include <stdio.h>

static std::vector<unsigned char> loadFile(const char* path) {
    FILE* fp = fopen(path, "rb");
    if (!fp) {
        fprintf(stderr, "Failed to open: %s\n", path);
        return {};
    }
    fseek(fp, 0, SEEK_END);
    long sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    std::vector<unsigned char> buf(sz);
    fread(buf.data(), 1, sz, fp);
    fclose(fp);
    return buf;
}

int main(void) {
    /* 1. Runtime initialize */
    LNInstanceInitializeSettings settings = {};
    settings.enableValidation = LN_TRUE;
    LNInstance_Initialize(&settings);

    /* 2. Window + GraphicsContext */
    LNHandle window = LN_NULL_HANDLE;
    LNWindow_Create("LuminoC-CustomShader", 1280, 720, &window);
    LNHandle graphicsContext = LN_NULL_HANDLE;
    LNWindow_GetGraphicsContext(window, &graphicsContext);

    /* 3. Load compiled shader from file */
    auto shaderData = loadFile(ASSETS_DIR "/Shader1.lcsh");
    if (shaderData.empty()) {
        fprintf(stderr, "Failed to load shader file.\n");
        return 1;
    }

    /* 4. Create material from compiled shader */
    LNHandle material = LN_NULL_HANDLE;
    LNMaterial_CreateFromCompiledShader(graphicsContext,
        shaderData.data(), (uint32_t)shaderData.size(), &material);
    //LNMaterial_SetColor(material, 1.0f, 0.0f, 0.0f, 1.0f); /* Red */

    const float myColor[4] = {0.0f, 1.0f, 0.0f, 1.0f}; /* Green */
    LNMaterial_SetFloat4(material, "u_myColor", myColor);

    /* 5. Triangle mesh (3 vertices, 3 indices, CCW winding) */
    LNVertex vertices[3] = {
        /* posX   posY   posZ   normX normY normZ  u    v    r    g    b    a    tanX tanY tanZ tanW */
        {  0.0f,  0.5f,  0.0f,  0,0,1,  0.5f, 0.0f,  1,1,1,1,  0,0,0,0 }, /* top */
        { -0.5f, -0.5f,  0.0f,  0,0,1,  0.0f, 1.0f,  1,1,1,1,  0,0,0,0 }, /* bottom-left */
        {  0.5f, -0.5f,  0.0f,  0,0,1,  1.0f, 1.0f,  1,1,1,1,  0,0,0,0 }, /* bottom-right */
    };
    uint32_t indices[3] = { 0, 1, 2 };
    LNSubMesh sub = { 0, 3, 0 };

    LNHandle mesh = LN_NULL_HANDLE;
    LNMesh_Create(graphicsContext, vertices, 3, indices, 3, &sub, 1, &mesh);
    LNMesh_SetMaterial(mesh, 0, material);

    /* 6. Perspective camera */
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

    /* 7. Main loop */
    LNGraphicsProfilering profilering = {};
    LNTransform identity = { 0,0,0,  0,0,0,1,  1,1,1 };
    LNBool quit = LN_FALSE;
    while (LNWindow_ProcessEvents(window, &quit) == LN_OK && !quit) {
        LNHandle renderer, colorBuffer, depthBuffer;
        LNGraphicsContext_BeginFrame(graphicsContext, &renderer, &colorBuffer, &depthBuffer);
        LNRenderer_BeginRenderPass(renderer, graphicsContext, camera, 0.0f, 0.0f, 0.0f, 1.0f);
        LNRenderer_DrawMesh(renderer, mesh, &identity, 0);
        LNRenderer_EndRenderPass(renderer);

        LNDebug_GetGraphicsProfiler(graphicsContext, &profilering);
        LNDebug_Print(graphicsContext, (std::string("FPS: ") + std::to_string(profilering.fps)).c_str());
        LNDebug_Print(graphicsContext, (std::string("FrameTime(ms): ") + std::to_string(profilering.lastFrameTimeMs)).c_str());
        LNDebug_Print(graphicsContext, (std::string("DrawCall: ") + std::to_string(profilering.drawCallCount)).c_str());

        LNGraphicsContext_EndFrame(graphicsContext);
    }

    /* 8. Cleanup */
    LNObject_Release(mesh);
    LNObject_Release(material);
    LNObject_Release(camera);
    LNObject_Release(window);
    LNInstance_Terminate();

    printf("Done.\n");
    return 0;
}
