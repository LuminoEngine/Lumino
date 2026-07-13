# lumino_shader Architecture


## 座標変換周り

優先事項は次の通り。

- 1. mul の順序は `mul(M, v)` とする。
- 2. メモリレイアウトを行優先 (row-major) にする。

バックエンドに応じて解釈が変わるので、あとはプログラムで対応する。

### 1. mul の順序は `mul(M, v)` とする。

ひとまず、これを最優先事項にする。シェーダコードはユーザーが直接書く部分なので、ここの表記は基本的に変えられないと考えたほうが良い。

判断理由:
1. 数学・業界標準との整合性
現代の主要シェーダ言語はすべて列ベクトル慣習:
- GLSL (OpenGL/Vulkan): gl_Position = M * v
- WGSL (WebGPU): result = mat * vec
- MSL (Metal): result = M * v

2. 変換チェーンの数学的直感との一致

```c
// mul(M, v): 右から左に読む = 数学記法と同じ
float4 pos = mul(Proj, mul(View, mul(Model, localPos)));
//                                       ↑最初に適用

// mul(v, M): 左から右に読む = 直感的に見えるが行列が転置になる
float4 pos = mul(localPos, mul(ModelT, mul(ViewT, ProjT)));
//           転置行列が必要になり混乱しやすい
```

3. 教材・リファレンスとの整合性

- Real-Time Rendering、線形代数の教科書、Vulkan/Metal の公式ドキュメントはすべて $Mv$ 記法を使う。
- slang 公式サンプルも同様。 https://github.com/shader-slang/slang/blob/master/examples/model-viewer/shaders.slang

mul(v, M) が有利なケース:
- レガシー HLSL/DirectX 資産を大量に流用する場合（DirectX 9 時代は行ベクトル慣習が主流だった）
- エンジン内部で 行主序 (row-major) のメモリレイアウト を採用する場合

### 2. メモリレイアウトを行優先 (row-major) にする。

メモリレイアウトも、mul の演算順序と同様、クライアントプログラムからメモリを書き込んでもらうことにになるので、ここも簡単には変えてはならないところ。

自分の調べた範囲だと、ライブラリやゲームエンジンの多数派にみえるため。

- [glTF](https://github.com/KhronosGroup/glTF-Tutorials/blob/main/gltfTutorial/gltfTutorial_004_ScenesNodes.md#local-and-global-transforms)
- [glm](https://github.com/g-truc/glm/blob/master/glm/ext/matrix_transform.inl)
- DirectX の XMMATRIX
- godot の Transform3D/Projection
- Babylon.js の Matrix
- UE の [FMatrix](https://rodolphe-vaillant.fr/entry/145/unreal-engine-c-tmap-doc-sheet-1)

メモリレイアウトが Column-major なのは:
- [Three.js](https://threejs.org/docs/#Matrix4)
- [Unity](https://github.com/Unity-Technologies/UnityCsReference/blob/master/Runtime/Export/Math/Matrix4x4.cs#L359)

## SLANG_MATRIX_LAYOUT_COLUMN_MAJOR について

これを指定すると、 SPIR-V で行列メモリレイアウトを RowMajor にするよう Slang に指示できる。
値が COLUMN_MAJOR とか書いてあるのでとてもわかり辛いが、多分 slang は HLSL を源流としているためかもしれない。

実際の SPIR-V を spirv-dis で見てみると、次のように RowMajor 属性が付いていることがわかるはず。
- OpMemberDecorate %ObjectParams_std140 0 RowMajor



------------------------

## 現行アーキテクチャ (v2 / lcs2)

コンパイルパイプライン・データモデル・`.lcsh` バイナリフォーマットなど、現行 (シグネチャ `lcs2`)
の設計は [README.md](README.md) にまとめています。本ファイルの上記セクション (座標変換周り) は
実装バージョンに依存しない恒常的な規約なので、そのまま有効です。

------------------------

## 付録: v1 時代の設計メモ (歴史的記録)

以下は `.lcsh` フォーマットが `lcsh.bl.` 等のシグネチャを使い、クラス名も `UnifiedShader` /
`ShaderCompiler` / `UnifiedShaderSerializer` (末尾に `2` が付かない) だった頃の設計メモです。
現行の実装は `UnifiedShader2` / `ShaderCompiler2` / `UnifiedShaderSerializer2` で、バイナリの
シグネチャも `lcs2` に変わっています。ファイルパスやクラス名は当時のままなので、現行実装の
参照には使わないでください。Shader + Material モデルや ShaderPass、LUMINO_METADATA といった
設計思想の変遷を追うための参考として残しています。

## 概要

lumino_shader は、Slang シェーダーソースをクロスコンパイルし、複数のグラフィックス API ターゲット向けのバイトコードを統一的に管理するパッケージです。

Lumino は物理ベースレンダリングなどの特定のレンダリングパイプラインを内蔵しません。レンダリング手法の選択は Lumino を利用する上位エンジンの責任です。lumino_shader は、どのようなレンダリングパイプラインでも利用できる **低レイヤーのシェーダー管理基盤** を提供します。

## 設計思想

### Shader + Material モデル

Lumino が提供する抽象化は以下の 2 層です：

- **Shader**: コンパイル済みシェーダープログラム（パス、エントリポイント、リフレクション情報）
- **Material**: Shader + UniformBuffer（パラメータの具体値をバインドしたもの）

ライティング、シャドウ、ポストプロセスなどのレンダリング機能は上位エンジンが定義します。lumino_shader はそれらを **サポートできる仕組み** を提供しますが、具体的な意味づけは行いません。

### ShaderPass - エントリポイントのグルーピング

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

