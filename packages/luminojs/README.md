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

### カスタムシェーダと Material の共有

コンパイル済みシェーダ (`.lcsh`) は `Shader` として読み込み、そこから `Material` を作ります。
`Shader` が GPU シェーダモジュールとパイプラインレイアウトを保持し、そこから作った
`Material` はすべてそれを共有します。

```ts
import { Material, Shader } from "luminojs";

const resp = await fetch("/bloom.lcsh");
const bloomShader = Shader.createFromCompiledShader(new Uint8Array(await resp.arrayBuffer()));

// 描画箇所ごとに Material を作る (GPU シェーダモジュールは共有される)
const bloomA = Material.createFromShader(bloomShader);
const bloomB = Material.createFromShader(bloomShader);
bloomA.setFloat4("u_params", [1, 0, 0, 0]);
bloomB.setFloat4("u_params", [0, 1, 0, 0]);
```

Material のパラメータは同一フレーム内では後勝ちになります (パラメータがマテリアル単位の
uniform buffer に載り、フレームスロットがフレーム単位のため)。そのため
**「1 フレーム内で異なるパラメータで描く箇所の数」だけ Material が必要**です。
ポストエフェクトを Viewport ごとに掛ける場合や、テクスチャごとに Material を分ける場合は、
`Shader` を 1 つだけ作ってそこから Material を量産してください。

`Material.createFromCompiledShader(data)` も引き続き使えますが、呼び出しごとに専用の
`Shader` を作るため、同一シェーダで複数の Material を作る用途には向きません。

共有できているかは `getProfiler().shaderPassCount` で計測できます (下記)。

### テクスチャのサンプリング設定

テクスチャのフィルタリングとアドレッシングは `Material` に設定します。
既定は `TextureFilterMode.Linear` + `TextureAddressMode.ClampToEdge` です。

```ts
import { Material, TextureAddressMode, TextureFilterMode } from "luminojs";

// ドット絵を拡大表示する: 補間でボケないよう Nearest にする。
const sprite = Material.createUnlit();
sprite.setSamplerState(TextureFilterMode.Nearest, TextureAddressMode.ClampToEdge);

// ポストエフェクト: 画面端の近傍をサンプルしても反対側の端から回り込まない
// (既定が ClampToEdge)。タイリングさせたいノイズだけスロット単位で上書きする。
const bloom = Material.createFromCompiledShader(bloomShaderData);
bloom.setNamedSamplerState("u_noiseTexture", TextureFilterMode.Nearest, TextureAddressMode.Repeat);
```

`setNamedSamplerState` の名前にはシェーダの `Texture2D` 変数名を指定します
(ペアの `SamplerState` 変数名ではありません)。詳細は
[shader-conventions.md](../../docs/shader-conventions.md) を参照してください。

### プロファイリング

`GraphicsContext.getProfiler()` で、ドローコール数、FPS、フレーム時間、シェーダパス数を
取得できます (C-API の `LNGraphicsProfiler` に対応)。

```ts
import type { GraphicsProfiler } from "luminojs";

function frame() {
    const info = ctx.beginFrame();
    if (info) {
        // ...描画...
        ctx.endFrame();

        // endFrame の後に読むこと。直前に描画し終えたフレームの計測値が得られる。
        const p: GraphicsProfiler = ctx.getProfiler();
        hud.textContent =
            `${p.drawCallCount} draws / ${p.fps.toFixed(1)} fps / ${p.lastFrameTimeMs.toFixed(2)} ms`;
    }
    requestAnimationFrame(frame);
}
```

| プロパティ | 意味 |
| --- | --- |
| `drawCallCount` | ドローコール数。`beginFrame()` でリセットされ、描画のたびに加算されます。 |
| `fps` | 実際のフレームレート。直前フレームの所要時間から算出した瞬間値です。 |
| `lastFrameTimeMs` | 直前フレームの所要時間 (ミリ秒)。 |
| `shaderPassCount` | 生存しているシェーダパス数 (プロセス全体)。1 パス = GPU シェーダモジュール 2 個 + パイプラインレイアウト 1 個。フレームではリセットされません。 |

**呼び出しタイミングに注意してください。** `fps` と `lastFrameTimeMs` は `endFrame()` の
中で更新されるため、`beginFrame()` - `endFrame()` の間に読むと 1 フレーム古い値になります。
また `drawCallCount` は `beginFrame()` でリセットされるので、フレームの途中で読むと
その時点までの途中経過になります。

スプライトのバッチングが効いているかは `drawCallCount` で直接確認できます。同一マテリアルの
スプライトがバッチングされていれば、描画枚数を増やしてもこの値はほとんど増えません。

`shaderPassCount` は、1 つの `Shader` から作った Material 群が GPU シェーダモジュールを
共有できているかの確認に使います。ビルトインシェーダの分も含まれるため、絶対値ではなく
**Material を増やしたときの増分**を見てください。

```ts
const before = ctx.getProfiler().shaderPassCount;
for (let i = 0; i < 8; i++) materials.push(Material.createFromShader(shader));
// ...1 フレーム描画する (GPU リソースは初回描画時に遅延生成される)...
console.log(ctx.getProfiler().shaderPassCount - before);  // 共有できていれば 0
```

## テスト

実際にロードする WASM バイナリを検証するスモークテストがあります (Playwright + Chromium)。

```sh
npm run test:smoke
```

実行前提と採用方式の詳細は [test/smoke/README.md](test/smoke/README.md) を参照してください。
