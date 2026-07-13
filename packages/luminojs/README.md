# luminojs

Lumino グラフィックスエンジンの TypeScript / WebAssembly バインディングです。

`luminojs` は、Emscripten で WebAssembly にコンパイルされた Lumino レンダリングエンジンの TypeScript API を提供します。ネイティブ C/C++ 版と同じコアエンジンを使用して、ブラウザ上で GPU アクセラレーションによる 2D/3D レンダリングを実現します。

## ビルド方法

リポジトリルートの README.md を参照してください。

## 使い方

```ts
import { Runtime, GraphicsContext } from "luminojs";

// WASM モジュールをロードし、C-API 関数をバインドし、Lumino インスタンスを初期化する。
// アプリケーションの起動時に一度だけ呼び出す。
await Runtime.initialize();

// HTML canvas に紐づく GraphicsContext を作成する。
const ctx = await GraphicsContext.createFromCanvas("#my-canvas");

function frame() {
    const { renderer } = ctx.beginFrame();
    // ...renderer を使った描画コマンド...
    ctx.endFrame();
    requestAnimationFrame(frame);
}
requestAnimationFrame(frame);
```

`Runtime.initialize()` は WASM モジュールのロード、C-API 関数の `cwrap` バインド、C++ 側との
ABI レイアウト検証 (構造体サイズの照合)、`LNInstance_Initialize` の呼び出しまでを行います。
動作するサンプルは [luminojs-examples](../luminojs-examples) を参照してください。

## テスト

実際にロードする WASM バイナリを検証するスモークテストがあります (Playwright + Chromium)。

```sh
npm run test:smoke
```

実行前提と採用方式の詳細は [test/smoke/README.md](test/smoke/README.md) を参照してください。
