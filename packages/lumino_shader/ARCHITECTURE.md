# lumino_shader Architecture

## 概要

lumino_shader は、Slang シェーダーソースをクロスコンパイルし、複数のグラフィックス API ターゲット向けのバイトコードを統一的に管理するパッケージです。

Lumino は物理ベースレンダリングなどの特定のレンダリングパイプラインを内蔵しません。レンダリング手法の選択は Lumino を利用する上位エンジンの責任です。lumino_shader は、どのようなレンダリングパイプラインでも利用できる **低レイヤーのシェーダー管理基盤** を提供します。

## 設計思想

### Shader + Material モデル

Lumino が提供する抽象化は以下の 2 層です：

- **Shader**: コンパイル済みシェーダープログラム（パス、エントリポイント、リフレクション情報）
- **Material**: Shader + UniformBuffer（パラメータの具体値をバインドしたもの）

ライティング、シャドウ、ポストプロセスなどのレンダリング機能は上位エンジンが定義します。lumino_shader はそれらを **サポートできる仕組み** を提供しますが、具体的な意味づけは行いません。

### ShaderPass — エントリポイントのグルーピング

ShaderPass は「頂点シェーダー・フラグメントシェーダー・コンピュートシェーダーのエントリポイントの名前付き組み合わせ」です。

- パス名の **意味（Forward、ShadowCaster 等）は lumino_shader の関知するところではありません**。上位エンジンがパス名で問い合わせ、適切なパスを選択・フォールバックします。
- 単一パスのシンプルなシェーダーでも、複数パスを持つ高度なシェーダーでも同じ仕組みで扱えます。
- レンダーステート（ブレンドモード、デプステスト等）はシェーダーメタデータに含めません。WebGPU の設計に倣い、パイプライン生成時に上位側が指定します。

### LUMINO_METADATA

パスの宣言には `#ifdef LUMINO_METADATA` ブロック内の JSON を使用します：

```hlsl
#ifdef LUMINO_METADATA
{
    "passes": [
        { "name": "Forward", "vertex": "vsMain", "fragment": "fsMain" },
        { "name": "ShadowCaster", "vertex": "vsShadow", "fragment": "fsShadow" }
    ]
}
#endif
```

この方式を採用した理由：

- Slang は言語コンパイラであり、Effects フレームワーク（Technique/Pass）の概念を持たないため、外部でメタデータを定義する必要がある
- JSON による宣言は拡張が容易
- `#ifdef` ガードにより、Slang コンパイラからは不可視でコンパイルに影響しない

> **補足**: Slang のユーザー定義属性（`[LuminoPass("Forward", "vertex")]` のような形式）で同等の仕組みを構築することも技術的には可能ですが、現行の JSON 方式で十分に機能しており、シンプルさを優先して現在の設計を維持しています。

## アーキテクチャ

### コンパイルパイプライン

```
.slang ソース
    │
    ├─ ShaderMetadataParser ─→ GlobalShaderPass（パス名 + エントリポイント名）
    │
    └─ ShaderCompiler (Slang API)
         │
         ├─ loadModule()
         ├─ createCompositeComponentType()
         ├─ link()
         │
         └─ 各ターゲット × 各エントリポイント
              ├─ getEntryPointCode() ─→ Blob（バイトコード）
              ├─ getLayout()         ─→ TargetBindingInfo（バインディング）
              └─ VertexInputAttribute（頂点属性）
                      │
                      ▼
              DescriptorLayoutBuilder
                      │
                      ▼
               UnifiedShader（インメモリ表現）
                      │
                      ▼
         UnifiedShaderSerializer ─→ .lcsh バイナリ
                                  ─→ .lcsh.inl (hex 配列)
```

### 対応ターゲット

| ターゲット | グラフィックス API | 用途 |
|---|---|---|
| SPIR-V | Vulkan | デスクトップ / モバイル |
| DXIL | DirectX 12 | Windows |
| WGSL | WebGPU | ブラウザ |
| Metal | Metal | Apple |

### データモデル

```
UnifiedShader
 ├─ GlobalShaderPass[]          … パス定義（プラットフォーム非依存）
 │    ├─ name                   … パス名（上位エンジンが意味を定義）
 │    ├─ vertexEntryPoint       … 頂点シェーダー関数名
 │    ├─ fragmentEntryPoint     … フラグメントシェーダー関数名
 │    ├─ computeEntryPoint      … コンピュートシェーダー関数名
 │    ├─ descriptorLayout       … 統合されたディスクリプタレイアウト
 │    └─ targetShaderPassIds[4] … 各ターゲット用パスへの参照
 │
 ├─ TargetShaderPass[]          … ターゲット別パス（エントリポイント ID の組）
 ├─ TargetEntryPoint[]          … ターゲット別エントリポイント
 │    ├─ target, name
 │    ├─ codeBlobId             … バイトコードへの参照
 │    ├─ bindingLayout          … バインディング情報
 │    └─ inputAttributes        … 頂点入力属性
 │
 ├─ Blob[]                      … コンパイル済みバイトコード
 ├─ GlobalResourceLayout        … リソースレイアウト（バッファ/テクスチャ/サンプラー/ストレージ）
 └─ GlobalConstantBufferMember[] … 定数バッファメンバー情報
```

### .lcsh バイナリフォーマット

チャンクベースのバイナリ形式です。各セクションは 8 バイトのシグネチャとカウントを持ちます：

| シグネチャ | 内容 |
|---|---|
| `lcsh.bl.` | コードブロブ（SPIR-V / DXIL / WGSL / Metal バイトコード） |
| `lcsh.te.` | TargetEntryPoint |
| `lcsh.tp.` | TargetShaderPass |
| `lcsh.gp.` | GlobalShaderPass |
| `lcsh.gm.` | GlobalConstantBufferMember |
| `lcsh.gl.` | GlobalResourceLayout |

`UnifiedShaderSerializer::loadFromData()` でデシリアライズし、ランタイムで利用します。

## 主要ファイル

| ファイル | 役割 |
|---|---|
| `include/lumino_shader/Common.hpp` | 列挙型・共通構造体の定義 |
| `include/lumino_shader/ShaderCompiler.hpp` | Slang コンパイラ API ラッパー |
| `include/lumino_shader/UnifiedShader.hpp` | 統合シェーダーのインメモリ表現 |
| `include/lumino_shader/UnifiedShaderSerializer.hpp` | .lcsh シリアライズ / デシリアライズ |
| `src/ShaderMetadataParser.hpp` | LUMINO_METADATA JSON パーサー |
| `src/DescriptorLayoutBuilder.hpp` | ターゲット別バインディング → 統合レイアウト変換 |

## 他エンジンとの比較

| 観点 | Unity (ShaderLab) | Godot | Lumino |
|---|---|---|---|
| パス定義 | 明示的（Pass ブロック） | 暗黙的（エンジン内部で自動生成） | 明示的（LUMINO_METADATA） |
| パスの意味 | エンジンが定義 | エンジンが定義 | **上位エンジンが定義** |
| レンダーステート | シェーダー内で宣言 | render_mode で指定 | パイプライン生成時に上位が指定 |
| シェーダー言語 | HLSL + ShaderLab DSL | 独自 (GDSL) | Slang |
| レンダリングパイプライン | 内蔵 (URP/HDRP) | 内蔵 (Forward/Forward+) | **提供しない（上位の責任）** |
| 抽象化レベル | 高い | 高い | **低い（意図的）** |

Lumino は Unity や Godot とは異なり、特定のレンダリングパイプラインに依存しない低レイヤーの設計を選択しています。これにより、上位エンジンが自由にレンダリング手法を構築できます。

