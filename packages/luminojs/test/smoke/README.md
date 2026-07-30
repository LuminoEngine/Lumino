# luminojs WASM スモークテスト (改善案 #12)

luminojs が実際にロードする WASM バイナリ (`lib/LuminoC.wasm`) を、ブラウザが実際に
ロードする経路で検証する自動テストです。デスクトップ (Vulkan) の GoogleTest とは別に、
「WASM でも動く」ことを機械的に確認します。

## 検証項目

1. WASM モジュール (`LuminoC.mjs` / `LuminoC.wasm`) がブラウザでロードできる
2. `Runtime.initialize()` が成功する (WASM ロード + 全 C-API バインド + 構造体レイアウト
   照合 (改善案 #10) + `LNInstance_Initialize` による WebGPU デバイス生成)
3. `LNHelloTest(42)` が `42` を返す
4. `Runtime.getBuildTimestamp()` が空でない文字列を返す
5. `Runtime.decodeImage()` が小さな PNG を期待どおりの RGBA8 ピクセルにデコードする
   (GPU 非依存の純 CPU 経路)
6. `LNDebug_GetGraphicsProfiler` が WASM にエクスポートされている (GPU 非依存)。
   実バイナリが報告する `sizeof(LNGraphicsProfiler)` が 12 バイトであること、
   無効ハンドルでの呼び出しが `LN_ERROR_INVALID_HANDLE` を返すことを確認します。
7. `GraphicsContext.getProfiler()` が `drawCallCount` / `fps` / `lastFrameTimeMs` を返す
   (WebGPU 必須)。同一マテリアルのスプライト 16 枚がバッチングされ、`drawCallCount` が
   枚数に比例して増えないことも確認します。

GPU (WebGPU) の実描画経路のうち、描画結果のピクセル検証 (Capture 等) はスコープ外です
(`smoke.spec.mjs` 末尾の TODO を参照)。

## 実行前提

- WASM がビルド済みであること。 `lib/LuminoC.mjs` と `lib/LuminoC.wasm` が必要です。
  無い場合は先にリポジトリルートで WASM ビルドを実行してください
  (`scripts/build_wasm.py` 等)。JS ラッパ (`lib/luminojs.mjs`) は `pretest:smoke` が
  自動で再ビルドしますが、WASM 本体は再ビルドしません。
- Playwright のフル Chromium が導入済みであること。 未導入なら次を実行します。

  ```sh
  npx playwright install chromium
  ```

## 実行方法

```sh
# packages/luminojs で実行
npm run test:smoke
```

`pretest:smoke` が `npm run build` (tsc 型チェック + rollup) を先に走らせるため、
`src` の変更は自動でバンドルへ反映されます。

## WebGPU が無い環境での挙動 / スキップ切り替え

検証項目のうち `Runtime.initialize()` / `getProfiler()` / デバイスロスト復旧が WebGPU
デバイス生成を伴います (残りの項目は GPU 非依存で、WebGPU が無くても通ります)。
WebGPU が使えない環境でこれらをどう扱うかは `smoke.spec.mjs` 冒頭の定数で切り替えます。

```js
const SKIP_INITIALIZE_WHEN_NO_WEBGPU =
    false || process.env.LUMINO_SMOKE_SKIP_NO_WEBGPU === "1";
```

- `false` (既定): skip しない。WebGPU が使えない環境では WebGPU 依存の項目が 失敗 する。
  「どの環境で WebGPU が使える/使えないか」に気づくための設定。
- `true`: WebGPU アダプタが取得できない環境では WebGPU 依存の項目を 自動 skip する
  (GPU 非依存の項目だけを回す)。GPU 無しの CI でグリーンを保ちたいときに切り替える。

定数を書き換えても、環境変数 `LUMINO_SMOKE_SKIP_NO_WEBGPU=1` でも切り替えられます。
なお WebGPU アダプタの有無と initialize の成否は分けて判定しているため、`true` にしても
「アダプタはあるのに initialize が落ちる (実装バグ)」は skip されず検出されます。

### 別環境の再現 (どの環境で WebGPU が使えるかの確認)

Chromium の起動フラグを `LUMINO_SMOKE_CHROMIUM_ARGS` で追加できます。GPU を無効化して
「WebGPU が使えない環境」を手元で再現し、テストの振る舞いを確認できます。

```sh
# WebGPU 無しを再現 -> 既定 (skip OFF) では initialize が失敗する
LUMINO_SMOKE_CHROMIUM_ARGS="--disable-gpu" npm run test:smoke

# WebGPU 無し + skip ON -> initialize は skip され、残り 4 項目がグリーン
LUMINO_SMOKE_CHROMIUM_ARGS="--disable-gpu" LUMINO_SMOKE_SKIP_NO_WEBGPU=1 npm run test:smoke
```

参考: Playwright 既定の headless_shell や `--disable-gpu` の Chromium では WebGPU
アダプタが取得できない (=`requestAdapter()` が `null`) ため、`Runtime.initialize()` は
失敗する。GPU の無い CI ランナーで WebGPU を使うにはソフトウェア実装 (SwiftShader /
Mesa lavapipe 等) が必要で、成立性は OS/環境に強く依存する。

## 採用方式と技術的な理由

方式: Playwright + フル Chromium (`channel: "chromium"`)

- `LuminoC.mjs` は `-sENVIRONMENT=web` でビルドされており、素の Node.js では
  instantiate 時に "not compiled for this environment" で失敗します
  (方式B: Node 直接ロードは不可)。
- `Runtime.initialize()` は内部で `LNInstance_Initialize` -> WebGPU デバイス生成を
  行うため WebGPU が必須です。Node には WebGPU が無いため、`web,node` で再ビルド
  しても initialize は成功しません。
- Playwright 既定の headless_shell は `navigator.gpu.requestAdapter()` が `null` を
  返し WebGPU デバイスを生成できません。フル Chromium (`channel: "chromium"`) を
  使うことで、ハードウェア GPU、もしくは `--enable-unsafe-swiftshader` による
  ソフトウェア実装で WebGPU デバイスを生成できます。
- WebGPU (`navigator.gpu`) は secure context でのみ露出するため、テストは
  `127.0.0.1` 上の HTTP でファイルを配信します (`static-server.mjs`)。

## ファイル構成

- `smoke.spec.mjs` - テスト本体
- `playwright.config.mjs` - Playwright 設定 (フル Chromium + swiftshader フラグ)
- `static-server.mjs` - `lib/` を配信する最小 HTTP サーバ (WASM の MIME を明示)
- `make-png.mjs` - 既知ピクセルの PNG を生成する最小エンコーダ
