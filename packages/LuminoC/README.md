# LuminoC

Lumino の C API (FFI 境界) を提供するパッケージです。ネイティブ (C/C++) からも、`luminojs` などの言語バインディングからも、Lumino のすべての機能はこの C API を経由して利用します。

## ハンドル (LNHandle)

Lumino のオブジェクトは `LNHandle` で参照します。上位16bitが世代番号、下位16bitがインデックスです。無効なハンドルは `LN_NULL_HANDLE` (0) です。

- `LNWindow_Create` のように名前に "Create" を含む関数で作成したハンドルは、使い終わったら `LNObject_Release` で明示的に解放してください。
- `LNWindow_GetGraphicsContext` や `LNGraphicsContext_BeginFrame` の出力のように、既存オブジェクトから取得するハンドル (借用ハンドル) は明示的に解放する必要はありません。

## LNResult

ほとんどの API 関数は `LNResult` を返し、成功したかどうかを確認できます。エラーコードは enum 定義のコメントを参照してください。

## 基本的なライフサイクル

1. `LNInstance_Initialize` で Lumino を初期化する。
2. `LNWindow_Create` (デスクトップ) または `LNWindow_CreateFromCanvas` (Web) でウィンドウを作成し、
   `LNWindow_GetGraphicsContext` で `GraphicsContext` を取得する。
3. メインループの中で `LNWindow_ProcessEvents` → `LNGraphicsContext_BeginFrame` →
   `LNRenderer_BeginRenderPass` → 描画コマンド → `LNRenderer_EndRenderPass` →
   `LNGraphicsContext_EndFrame` を繰り返す。
4. `LNObject_Release` で作成したハンドルを解放し、`LNInstance_Terminate` で終了する。

最小構成のサンプルは [examples/ClearScreen/Main.cpp](examples/ClearScreen/Main.cpp) を参照してください。

## examples

| サンプル | 内容 |
| --- | --- |
| [ClearScreen](examples/ClearScreen/Main.cpp) | 画面をクリアするだけの最小サンプル |
| [HelloSprite](examples/HelloSprite/Main.cpp) | 正射影カメラで1枚のスプライトを描画する最小サンプル |
| [HelloTexture](examples/HelloTexture/Main.cpp) | PNG 画像を読み込み、四角形メッシュに貼り付けて描画 |
| [BatchSprite](examples/BatchSprite/Main.cpp) | 動的メッシュによる大量スプライトのバッチ描画 |
| [BatchRendering](examples/BatchRendering/Main.cpp) | `LNRenderer_DrawSprite` / `LNRenderer_DrawMesh` の自動バッチ描画 |
| [CustomShader](examples/CustomShader/Main.cpp) | コンパイル済みシェーダ (`.lcsh`) から Material を作成 |
| [RenderTargets](examples/RenderTargets/Main.cpp) | 複数のレンダーターゲットテクスチャへの描画 |
| [SSR](examples/SSR/Main.cpp) | マルチパスのスクリーンスペースリフレクション |
