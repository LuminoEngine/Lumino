# LuminoC

Lumino の C API (FFI 境界) を提供するパッケージです。ネイティブ (C/C++) からも、`luminojs`
などの言語バインディングからも、Lumino のすべての機能はこの C API を経由して利用します。

## 公開契約

- [include/LuminoC/lumino.h](include/LuminoC/lumino.h) が Lumino の公開 API 契約です。関数は
  `LN<Type>_<Verb>` の命名規則に従い (例: `LNWindow_Create`)、戻り値は基本的に `LNResult` です。
- 型定義は [include/LuminoC/lumino_types.h](include/LuminoC/lumino_types.h) にあります。
- 実装は [src/LuminoAPI.cpp](src/LuminoAPI.cpp) が LuminoCore への橋渡しを行います。

lumino.h に定義のない機能は、C API からは利用できません。

## ハンドル (LNHandle)

Lumino のオブジェクトは `LNHandle` (`uint32_t`) で参照します。上位16bitが世代番号、下位16bitが
インデックスです。無効なハンドルは `LN_NULL_HANDLE` (0) です。

- `LNWindow_Create` のように名前に "Create" を含む関数で作成したハンドルは、使い終わったら `LNObject_Release` で明示的に解放してください。
- `LNWindow_GetGraphicsContext` や `LNGraphicsContext_BeginFrame` の出力のように、既存オブジェクトから
  取得するハンドル (借用ハンドル) は明示的に解放する必要はありません。

## LNResult

ほとんどの API 関数は `LNResult` を返し、成功したかどうかを確認できます。

| 値 | 意味 |
| --- | --- |
| `LN_OK` | 成功 |
| `LN_ERROR_UNKNOWN` | 不明なエラー |
| `LN_RUNTIME_UNINITIALIZED` | ランタイム未初期化 (`LNInstance_Initialize` 前) |
| `LN_ERROR_INVALID_ARGUMENT` | 無効な引数 |
| `LN_ERROR_INVALID_HANDLE` | 無効なハンドル |
| `LN_ERROR_NOT_SUPPORTED` | 現在のビルド/プラットフォームでは未対応の機能 (例: WASM ビルドでの一部 GraphicsContext API) |

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

シェーダの書き方・Material API からのパラメータ設定は
[docs/shader-conventions.md](../../docs/shader-conventions.md) を参照してください。
