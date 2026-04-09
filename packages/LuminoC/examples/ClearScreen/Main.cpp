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
    // Lumino を初期化します。
    //    なおほとんどの API 関数は戻り値が LNResult となっており、エラーの有無を確認できます。
    //    以降のサンプルではエラーチェックを省略しています。
    LNInstanceInitializeSettings settings = {};
    settings.enableValidation = LN_TRUE;
    LNResult result = LNInstance_Initialize(&settings);
    if (result != LN_OK) {
        fprintf(stderr, "LNInstance_Initialize failed: %d\n", result);
        return 1;
    }

    // ウィンドウを作成し、関連づいた GraphicsContext を取得します。
    //    内部で GraphicsContext (RHI Device, SwapChain) も作成されます。
    LNHandle window = LN_NULL_HANDLE;
    LNHandle graphicsContext = LN_NULL_HANDLE;
    LNWindow_Create("Lumino - Clear Screen", 1280, 720, &window);
    LNWindow_GetGraphicsContext(window, &graphicsContext);

    // メインループ
    LNBool quit = LN_FALSE;
    while (!quit) {
        // イベント処理。ウィンドウが閉じられると quit が LN_TRUE になります。
        //   LNWindow_ProcessEvents は GUI アプリケーションに必要なイベント処理です。 この処理は定期的に呼び出す必要があります。
        //   また quit にはウィンドウがクローズされた場合など、アプリケーションが終了すべきかどうかを返します。
        LNWindow_ProcessEvents(window, &quit);

        // フレーム開始
        //   現在のウィンドウサイズにマッチするようにバックバッファを準備し、ひとつのフレームの描画を開始します。
        LNHandle renderer, colorBuffer, depthBuffer;
        LNGraphicsContext_BeginFrame(graphicsContext, &renderer, &colorBuffer, &depthBuffer);

        // レンダーパス開始 (ライトグリーンでクリア)
        LNRenderPassDesc rpDesc;
        LNRenderPassDesc_Init(&rpDesc);
        rpDesc.colorAttachment.clearColor[0] = 0.60f;
        rpDesc.colorAttachment.clearColor[1] = 0.85f;
        rpDesc.colorAttachment.clearColor[2] = 0.60f;
        rpDesc.colorAttachment.clearColor[3] = 1.0f;
        LNGraphicsContext_BeginRenderPass(graphicsContext, &rpDesc);

        // このサンプルはクリアするだけなので、描画コマンドはありません。

        // レンダーパス終了
        LNGraphicsContext_EndRenderPass(graphicsContext);

        // フレーム終了・画面表示
        LNGraphicsContext_EndFrame(graphicsContext);
    }

    // 5. リソースを解放します。
    LNObject_Release(graphicsContext);
    LNObject_Release(window);
    LNInstance_Terminate();

    return 0;
}
