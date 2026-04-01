/**
 * ClearScreen.c
 *
 * C_API を使って画面をクリアするだけのサンプル。
 * 他のサンプル (HelloTriangle.cpp など) が C++ API を使っているのに対し、
 * このサンプルは C_API (lumino_api) のみを使って記述しています。
 */

#include <LuminoC/lumino.h>
#include <stdio.h>

int main(void) {
    // 1. Lumino を初期化します。
    LNResult result = LNInstance_Initialize();
    if (result != LN_OK) {
        fprintf(stderr, "LNInstance_Initialize failed: %d\n", result);
        return 1;
    }

    // 2. ウィンドウを作成します。内部で GraphicsContext (Vulkan デバイス・スワップチェーン) も作成されます。
    LNHandle window = LN_NULL_HANDLE;
    result = LNWindow_Create("Lumino - Clear Screen", 1280, 720, &window);
    if (result != LN_OK) {
        fprintf(stderr, "LNWindow_Create failed: %d\n", result);
        LNInstance_Terminate();
        return 1;
    }

    // 3. ウィンドウに関連づいた GraphicsContext を取得します。
    LNHandle graphicsContext = LN_NULL_HANDLE;
    result = LNWindow_GetGraphicsContext(window, &graphicsContext);
    if (result != LN_OK) {
        fprintf(stderr, "LNWindow_GetGraphicsContext failed: %d\n", result);
        LNObject_Release(window);
        LNInstance_Terminate();
        return 1;
    }

    // 4. メインループ
    int cont = 1;
    while (cont) {
        // イベント処理。ウィンドウが閉じられると cont が 0 になります。
        result = LNWindow_ProcessEvents(window, &cont);
        if (result != LN_OK || !cont) break;

        // フレーム開始
        result = LNGraphicsContext_BeginFrame(graphicsContext);
        if (result != LN_OK) break;

        // レンダーパス開始 (ライトグリーンでクリア)
        result = LNGraphicsContext_BeginRenderPass(graphicsContext, 0.60f, 0.85f, 0.60f, 1.0f);
        if (result != LN_OK) break;

        // このサンプルはクリアするだけなので、描画コマンドはありません。

        // レンダーパス終了
        result = LNGraphicsContext_EndRenderPass(graphicsContext);
        if (result != LN_OK) break;

        // フレーム終了・画面表示
        result = LNGraphicsContext_EndFrame(graphicsContext);
        if (result != LN_OK) break;
    }

    // 5. リソースを解放します。
    LNObject_Release(graphicsContext);
    LNObject_Release(window);
    LNInstance_Terminate();

    return 0;
}
