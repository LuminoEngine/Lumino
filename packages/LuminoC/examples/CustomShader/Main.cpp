/**
 * CustomShader.c
 *
 * コンパイル済みシェーダ (.lcsh) をファイルから読み込み、
 * LNShader_CreateFromCompiledShader で Shader を作成し、
 * そこから LNMaterial_CreateFromShader で Material を作成して
 * 三角形を描画するデモ。
 *
 * Shader は GPU シェーダモジュールとパイプラインレイアウトを保持する共有オブジェクトで、
 * 1 つの Shader から Material を何個作ってもそれらは増えません
 * (HUD の ShaderPass 表示で確認できます)。
 */
#include "../Utils.h"

int main() {
    InitializeInstance();

    // 2. Window + GraphicsContext
    LNHandle window = LN_NULL_HANDLE;
    LNWindow_Create("LuminoC-CustomShader", WINDOW_W, WINDOW_H, &window);
    LNHandle graphicsContext = LN_NULL_HANDLE;
    LNWindow_GetGraphicsContext(window, &graphicsContext);

    // 3. Load compiled shader from file
    auto shaderData = loadFile(ASSETS_DIR "/Shader1.lcsh");
    if (shaderData.empty()) {
        fprintf(stderr, "Failed to load shader file.\n");
        return 1;
    }

    // 4. Create shader (GPU シェーダモジュール + パイプラインレイアウト)
    LNHandle shader = LN_NULL_HANDLE;
    LNShader_CreateFromCompiledShader(graphicsContext,
        shaderData.data(), (uint32_t)shaderData.size(), &shader);

    // 5. Create material from the shader
    //    同じ Shader からいくつ Material を作っても GPU リソースは増えない。
    LNHandle material = LN_NULL_HANDLE;
    LNMaterial_CreateFromShader(shader, &material);
    //LNMaterial_SetColor(material, 1.0f, 0.0f, 0.0f, 1.0f); // Red

    const float myColor[4] = {0.0f, 1.0f, 0.0f, 1.0f}; // Green
    LNMaterial_SetFloat4(material, "u_myColor", myColor);

    // 6. Triangle mesh (3 vertices, 3 indices, CCW winding)
    LNVertex vertices[3] = {
        // posX   posY   posZ   normX normY normZ  u    v    r    g    b    a    tanX tanY tanZ tanW
        {  0.0f,  0.5f,  0.0f,  0,0,1,  0.5f, 0.0f,  1,1,1,1,  0,0,0,0 }, // top
        { -0.5f, -0.5f,  0.0f,  0,0,1,  0.0f, 1.0f,  1,1,1,1,  0,0,0,0 }, // bottom-left
        {  0.5f, -0.5f,  0.0f,  0,0,1,  1.0f, 1.0f,  1,1,1,1,  0,0,0,0 }, // bottom-right
    };
    uint32_t indices[3] = { 0, 1, 2 };
    LNSubMesh sub = { 0, 3, 0 };

    LNHandle mesh = LN_NULL_HANDLE;
    LNMesh_Create(graphicsContext, vertices, 3, indices, 3, &sub, 1, &mesh);
    LNMesh_SetMaterial(mesh, 0, material);

    // 7. Perspective camera
    LNHandle camera = LN_NULL_HANDLE;
    LNCamera_Create(&camera);
    LNCamera_SetPerspective(camera,
        60.0f * 3.14159f / 180.0f,
        (float)WINDOW_W / (float)WINDOW_H,
        0.1f, 100.0f);
    LNCamera_SetLookAt(camera,
        0.0f, 0.0f, 3.0f,   // eye
        0.0f, 0.0f, 0.0f,   // target
        0.0f, 1.0f, 0.0f);  // up

    // 8. Main loop
    LNGraphicsProfiler profilering = {};
    LNTransform identity = { 0,0,0,  0,0,0,1,  1,1,1 };
    LNBool quit = LN_FALSE;
    while (LNWindow_ProcessEvents(window, &quit) == LN_OK && !quit) {
        LNHandle renderer, colorBuffer, depthBuffer;
        LNGraphicsContext_BeginFrame(graphicsContext, WINDOW_W, WINDOW_H, &renderer, &colorBuffer, &depthBuffer);
        LNRenderPassDesc rpDesc;
        LNRenderPassDesc_Init(&rpDesc);
        LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, camera);
        LNRenderer_DrawMesh(renderer, mesh, &identity, 0);
        LNRenderer_EndRenderPass(renderer);

        LNDebug_GetGraphicsProfiler(graphicsContext, &profilering);
        LNDebug_Print(graphicsContext, (std::string("FPS: ") + std::to_string(profilering.fps)).c_str());
        LNDebug_Print(graphicsContext, (std::string("FrameTime(ms): ") + std::to_string(profilering.lastFrameTimeMs)).c_str());
        LNDebug_Print(graphicsContext, (std::string("DrawCall: ") + std::to_string(profilering.drawCallCount)).c_str());
        // 生存シェーダパス数 (= シェーダモジュール数 / 2 = パイプラインレイアウト数)。
        // Material を増やしても増えないことを確認できる。
        LNDebug_Print(graphicsContext, (std::string("ShaderPass: ") + std::to_string(profilering.shaderPassCount)).c_str());

        LNGraphicsContext_EndFrame(graphicsContext);
    }

    // 9. Cleanup
    LNObject_Release(mesh);
    LNObject_Release(material);
    LNObject_Release(shader);
    LNObject_Release(camera);
    LNObject_Release(window);
    LNInstance_Terminate();

    printf("Done.\n");
    return 0;
}
