#include <lumino.h>

int main() {
    // 1. 最初に LNInstance_Initialize で Lumino を初期化します。
    //    なおほとんどの API 関数は戻り値が LNResult となっており、エラーの有無を確認できます。
    //    以降のサンプルではエラーチェックを省略しています。
    LNResult result = LNInstance_Initialize();
    if (result != LN_OK) {
        return 1;
    }

    // 2. ウィンドウを作成して表示します。
    LNHandle window = LN_NULL_HANDLE;
    LNWindow_Create(640, 480, "Example", &window);

    // 3. ウィンドウに関連づいた GraphicsContext を取得します。
    LNHandle graphicsContext = LN_NULL_HANDLE;
    LNWindow_GetGraphicsContext(window, &graphicsContext);

    LNHandle viewPoint = LN_NULL_HANDLE;
    LNViewPoint_Create(&viewPoint);

    while (true) {
        // x. LNInstance_ProcessEvents は GUI アプリケーションに必要なイベント処理です。 この処理は定期的に呼び出す必要があります。
        //    また LNInstance_ShouldQuit はウィンドウがクローズされた場合など、アプリケーションが終了すべきかどうかを返します。
        LNBool quit = LN_FALSE;
        LNInstance_ProcessEvents();
        LNInstance_ShouldQuit(&quit);
        if (quit) {
            break;
        }

        // x. 現在のウィンドウサイズにマッチするようにバックバッファを準備し、ひとつのフレームの描画を開始します。
        //    LNGraphicsContext_BeginFrame からは 1 フレーム分の描画を行うために必要なリソース取得できます。
        //    colorBuffer と depthBuffer は描画先バッファです。これからここに描画を行います。
        int width = 0;
        int height = 0;
        LNHandle colorBuffer = LN_NULL_HANDLE;
        LNHandle depthBuffer = LN_NULL_HANDLE;
        LNHandle commandList = LN_NULL_HANDLE;
        LNWindow_GetFramebufferSize(window, &width, &height);
        LNGraphicsContext_BeginFrame(graphicsContext, width, height, &colorBuffer, &depthBuffer, &commandList);

        // x. 2D シーンを描画するための視点情報を構築します。
        //    このサンプルは画面をクリアするだけであるため、この情報はダミーです。
        LNViewPoint_SetupOrtho2D(viewPoint, 0, 0, 0, width, height, -1000, 1000);

        // x. レンダーターゲットをクリアするための RenderPass を開始します。
        //    RenderPass を開始することなくクリアする方法はありません。
        //    このサンプルではクリアするだけなので、 BeginRenderPass の後はすぐに EndRenderPass を呼び出しています。
        LNHandle renderingPass = LN_NULL_HANDLE;
        LNRenderPassDescriptor descriptor;
        descriptor.renderTargets[0].renderTarget = colorBuffer;
        descriptor.renderTargets[0].clearColor[0] = 0.60f;
        descriptor.renderTargets[0].clearColor[1] = 0.85f;
        descriptor.renderTargets[0].clearColor[2] = 0.60f;
        descriptor.renderTargets[0].clearColor[3] = 1.0f;
        descriptor.renderTargets[0].clearEnable = LN_TRUE;
        descriptor.depthBuffer.depthBuffer = depthBuffer;
        descriptor.depthBuffer.clearDepth = 1.0f;
        descriptor.depthBuffer.clearStencil = 0;
        descriptor.depthBuffer.clearDepthEnable = LN_TRUE;
        descriptor.depthBuffer.clearStencilEnable = LN_TRUE;
        LNCommandList_BeginSceneRenderPass(commandList, descriptor, viewPoint, &renderingPass);
        LNCommandList_EndSceneRenderPass(commandList, renderingPass);

        LNGraphicsContext_EndFrame(graphicsContext);
        LNWindow_Present(window);

        // x. このサンプルのように LNWindow を使っている場合、発生したデバイスロストは自動的に復旧されます。 
        //    GraphicsContext を再作成する必要はありません。
    };

    // x. リソースを開放します。
    //    ~Create で作成したハンドルは ~Release で解放する必要があります。
    //    ~Get など、 ~Craete 以外で取得したハンドルは解放の必要はありません。
    //    なお Lumino のオブジェクトは参照カウント方式で管理されています。
    //    ~Create に対して必ず 1 回呼び出してください。
    LNObject_Release(viewPoint);
    LNObject_Release(window);

    // x. LNInstance_Terminate で Lumino を終了します。
    LNInstance_Terminate();
    return 0;
}
