# ARCHITECTURE

Lumino のレイヤー構造・パッケージ構成・シェーダパイプラインをまとめたドキュメントです。
各機能の詳細は、この文書からリンクする各パッケージの README や `docs/` 以下のドキュメントを参照してください。

設計思想の背景 (FFI として提供する理由、旧実装からの教訓など) は [AGENTS.md](AGENTS.md) を参照してください。

## レイヤー構造

Lumino はネイティブ (C++) のコアエンジンと、それを FFI 経由で呼び出す TypeScript / WebAssembly
バインディングで構成されます。上位のクライアントアプリケーションは `luminojs` を介して Lumino を
利用します。

```
[クライアントアプリケーション (ゲームエンジン / エディタ等)]
        | import * as LUMINO from "luminojs"
        v
[luminojs (TypeScript / WebAssembly バインディング)]
  Runtime (cwrap で C-API をバインド, ASYNCIFY 初期化)
  クラスラッパー (Texture/Material/Mesh/Camera/Renderer)
  ResidencyManager (GPU リソースの遅延生成 + 自動 evict)
  構造体を SIZEOF_* 定数で WASM 線形メモリへ手動シリアライズ
        | FFI (C-API 呼び出し)
        v
[LuminoC (C API / FFI 境界)]
  lumino.h (LN<Type>_<Verb> 命名, LNResult + out ハンドル)
  LuminoAPI.cpp (C++ 実装への橋渡し)
        v
[LuminoCore]
  Runtime/ObjectRegistry (世代付きハンドル)
  Graphics/Renderer (パス管理, 動的 UBO, BindGroup キャッシュ, ステンシル)
  Graphics/Batch (SortKey, スプライト頂点生成, マテリアル境界分割)
  Graphics/Material・PipelineCache・DynamicUniformAllocator
  rhi/ (WebGPU 準拠の RHI 抽象)
    rhi/vulkan (デスクトップ既定)
    rhi/webgpu (WASM/ブラウザ + デスクトップ Dawn)
        v
[LuminoBase] RefObject/Ref, Result (tl::expected), Logger, math
[LuminoShader] ShaderCompiler2 (Slang, 4 ターゲット同時コンパイル), UnifiedShaderSerializer2 (.lcsh)
```

ネイティブ版 (デスクトップ / コンシューマゲーム機) では、`luminojs` の代わりに C++ から直接
LuminoC の C-API を呼び出すことも想定しています。サンプルは
[LuminoC/examples/ClearScreen/Main.cpp](packages/LuminoC/examples/ClearScreen/Main.cpp) を参照してください。

## パッケージ一覧

| パッケージ | 役割 | 詳細 |
|---|---|---|
| [LuminoBase](packages/LuminoBase/README.md) | 参照カウント (RefObject/Ref)・Result・Logger・数学 (Matrix4x4 等) の共通基盤 | [README](packages/LuminoBase/README.md) |
| [LuminoShader](packages/LuminoShader/README.md) | Slang シェーダコンパイラのラッパーと `.lcsh` バイナリフォーマット | [README](packages/LuminoShader/README.md) / [ARCHITECTURE](packages/LuminoShader/ARCHITECTURE.md) |
| [LuminoCore](packages/LuminoCore/README.md) | RHI (Vulkan/WebGPU 準拠の抽象) + 描画中核 (Renderer/Batch/Material/PipelineCache) | [README](packages/LuminoCore/README.md) |
| [LuminoC](packages/LuminoC/README.md) | C API (FFI 境界)。`lumino.h` が公開契約 | [README](packages/LuminoC/README.md) |
| [luminojs](packages/luminojs/README.md) | TypeScript / WebAssembly バインディング | [README](packages/luminojs/README.md) |
| [luminosc](packages/luminosc/README.md) | シェーダコンパイラ CLI (`.slang` を `.lcsh` へクロスコンパイル) | [README](packages/luminosc/README.md) |
| luminosc-npm / luminosc-x64-windows | `luminosc` の npm 配布パッケージ (ビルド成果物) | - |
| luminojs-examples | `luminojs` を使った Vite サンプル集 | - |

## シェーダパイプライン

```
*.slang --(luminosc / Slang)--> *.lcsh (SPIR-V + DXIL + WGSL + Metal を1つに束ねたバイナリ + リフレクション情報)
                                *.lcsh.inl (C++ にバイトコードを埋め込むための hex 配列)
```

- 組み込みシェーダ (Unlit/BasicLit/StencilMask 等): デスクトップの CMake ビルドで `luminosc` を
  使い自動生成されます。
- サンプル・アプリ側のシェーダ: `luminosc` CLI で事前コンパイルし、生成された `.lcsh` /
  `.lcsh.inl` をコミットして利用します。
- ランタイムコンパイル (デスクトップ, `LUMINO_USE_SLANG` 有効時): `.slang` ソースを実行時に
  コンパイルして `Material`化することもできます。WASM ビルドではこの経路は使えません。

シェーダの書き方・パラメータ設定は [docs/shader-conventions.md](docs/shader-conventions.md)、
座標系・行列などグラフィックスの基本規約は [docs/graphics-conventions.md](docs/graphics-conventions.md)
を参照してください。`.lcsh` バイナリフォーマットの詳細は
[LuminoShader/README.md](packages/LuminoShader/README.md) と
[LuminoShader/ARCHITECTURE.md](packages/LuminoShader/ARCHITECTURE.md) を参照してください。
