# lumino_shader

Slang シェーダをコンパイルし、マルチターゲット (SPIRV / DXIL / WGSL / Metal) のバイトコードとリフレクション情報を統合バイナリ (`.lcsh`) にまとめるパッケージ。

## 設計方針

シェーダリソースは **更新頻度別** に Descriptor Set へ分離する。マテリアルパラメータは
`$Global`（bare uniform）、システムデータ（カメラ、ライト、オブジェクト行列）は
`ParameterBlock<T>` で宣言する。

```text
Set 0 ($Global / per-material) : マテリアルパラメータとテクスチャ - マテリアル切り替え時
Set 1 (per-view)               : カメラ - フレームに1回更新
Set 2 (per-scene)              : ライト - フレームに1回更新
Set 3 (per-object)             : ワールド行列 - オブジェクトごと
```

bare uniform は `$Global`（Set 0）にまとめられ、`ParameterBlock<T>` は独立した descriptor set
（Set 1-3）にマップされる。システム ParameterBlock（Set 1-3）は `import lumino;` が提供するため、
シェーダ作者が宣言するのは Set 0 のマテリアルパラメータだけでよい。

> シェーダの書き方と Material API からのパラメータ設定は
> [docs/shader-conventions.md](../../docs/shader-conventions.md) を参照。

## シェーダの書き方

`import lumino;` でシステム ParameterBlock（`viewData` / `sceneData` / `objectData`）と
共通構造体（`VSInput` 等）を取り込む。シェーダ作者が宣言するのはマテリアルパラメータ
（`$Global`, Set 0）だけ。

```slang
import lumino;   // viewData / sceneData / objectData / VSInput を提供

// マテリアルパラメータ (bare uniform → $Global, Set 0)
struct MaterialParams {
    float4 color;
    float4 specular;
};
uniform ConstantBuffer<MaterialParams> u_params;
uniform Texture2D                      u_baseTexture;
uniform SamplerState                   u_baseSampler;

[shader("vertex")]
VSOutput vsMain(VSInput input) {
    // viewData / objectData は import lumino で利用可能 (Set 1 / Set 3)
    ...
}

[shader("fragment")]
float4 fsMain(VSOutput input) : SV_TARGET { ... }

#ifdef LUMINO_METADATA
{
    "passes": [
        { "name": "Forward", "vertex": "vsMain", "fragment": "fsMain" }
    ]
}
#endif
```

[Slang Documentation](https://docs.shader-slang.org/en/stable/index.html)

### リソース宣言のルール

- **マテリアルパラメータ**は bare uniform（`uniform ConstantBuffer<T>`, `uniform Texture2D`,
  `uniform SamplerState`, または単独の `uniform float4` 等）で宣言する。これらは `$Global`
  （Set 0）にまとめられ、Material API から名前指定で値を設定する。
- **システムデータ**は `ParameterBlock<T>` で宣言する。plain data のみの構造体
  (`ViewParams`, `ObjectParams`) は暗黙の ConstantBuffer として扱われ、C++ 側からは
  アライメント済みの構造体をそのままセットする。
- `ParameterBlock<T>` 内に明示的な `ConstantBuffer<T>`, `Texture2D`, `SamplerState` を
  混在させた場合、各フィールドが個別のバインディングになる。

### WGSL の制約に注意

Slang では書けても **WGSL（WebGPU）では不正になる**コードがあります。代表例は
「非 uniform な早期 `return` / `break` の後でテクスチャをサンプリングする」パターンです。

```slang
// NG: 早期 return の後にサンプリングしている
if (input.uv.x > 0.5) { return float4(1, 0, 0, 1); }
float4 c = u_baseTexture.Sample(u_baseSampler, input.uv);   // WGSL では不正

// OK: サンプリングを分岐より前に巻き上げる
float4 c = u_baseTexture.Sample(u_baseSampler, input.uv);
if (input.uv.x > 0.5) { return float4(1, 0, 0, 1); }
```

これは `ShaderCompiler2` が生成 WGSL を Dawn で検証するため、**コンパイル時にエラー**
になります（`WgslValidator`）。検証を持たない構成でこの種のシェーダを使うと、
WebGPU 実行時にシェーダモジュールが無効になり画面が真っ黒になります。

その他のパターン（`ddx` / `fwidth`、ループ内の `break`、ヘルパ関数経由のサンプリング、
`SampleLevel` による回避など）は
[docs/shader-conventions.md の「WGSL（WebGPU）の制約」](../../docs/shader-conventions.md#wgslwebgpu-の制約)
にまとめてあります。

## C++ からの使い方

### コンパイル

```cpp
#include <lumino_shader/ShaderCompiler2.hpp>
#include <lumino_shader/UnifiedShader2.hpp>

auto compiler = *ShaderCompiler2::create();
compiler->addSearchPath("path/to/includes");  // 必要に応じて
// 生成 WGSL の検証は既定で有効。切りたいときのみ:
// compiler->setWgslValidationEnabled(false);

auto result = compiler->build("BasicLit.slang");
if (!result) {
    std::cerr << result.error().message << std::endl;
    return;
}
UnifiedShader2* shader = compiler->shader();
```

### リフレクション情報の取得

```cpp
// ParameterBlock の列挙 (システムデータ: viewData / sceneData / objectData)
// ※ マテリアルパラメータは $Global (Set 0) 側。ここには現れない。
for (const auto& block : shader->parameterBlocks()) {
    // block.name      : "viewData", "sceneData", "objectData"
    // block.setIndex   : descriptor set 番号 (1, 2, 3)
    // block.hasImplicitConstantBuffer : plain data のみなら true

    for (const auto& elem : block.elements) {
        // elem.name               : "params", "baseTexture", "" (implicit CB)
        // elem.kind               : ParameterBlockElementKind_ConstantBuffer / _Texture / _SamplerState
        // elem.constantBufferSize : CB のバイトサイズ (-1 if not CB)
    }
}

// シェーダパスの取得
for (const auto& pass : shader->globalShaderPasses()) {
    // pass->name             : "Forward"
    // pass->vertexEntryPoint : "vsMain"
    // pass->fragmentEntryPoint : "fsMain"

    // ターゲット別のシェーダパス
    auto tpId = pass->getTargetShaderPassId(ShaderTarget_SPIRV);
    TargetShaderPass2* tp = shader->targetShaderPass(tpId);

    // エントリポイントのコード取得
    TargetEntryPoint2* vertEP = shader->targetEntryPoint(tp->vertEntryPointId);
    Blob* code = shader->blob(vertEP->codeBlobId);
    // code->data : バイトコード (std::vector<uint8_t>)

    // 頂点入力属性
    for (const auto& attr : vertEP->inputAttributes) {
        // attr.usage          : AttributeUsage_Position, _Normal, etc.
        // attr.layoutLocation : location index
    }

    // マージされたバインディングレイアウト (vertex + fragment の used フラグ統合済み)
    for (const auto& binding : tp->bindingLayout.bindings) {
        // binding.setIndex    : descriptor set 番号
        // binding.bindingIndex: set 内のバインディング番号
        // binding.kind        : リソース種別
        // binding.size        : CB のバイトサイズ
        // binding.used        : ShaderStageFlags (Vertex | Pixel)
    }
}
```

### シリアライズ / デシリアライズ

```cpp
#include <lumino_shader/UnifiedShaderSerializer2.hpp>

// 保存
UnifiedShaderSerializer2::saveToFile(shader, "output.lcsh");

// 読み込み
std::vector<uint8_t> data = readFile("output.lcsh");
auto loaded = *UnifiedShaderSerializer2::loadFromData(data.data(), data.size());
```

## 主要クラス

| Class | Description |
| --- | --- |
| `ShaderCompiler2` | Compile Slang shaders, collect reflection info |
| `WgslValidator` | Validate generated WGSL with Dawn (Null backend) |
| `UnifiedShader2` | Data model holding compilation results |
| `UnifiedShaderSerializer2` | Read/write `.lcsh` binary format |
| `ParameterBlockLayout2` | ParameterBlock layout (set index, element list) |
| `ParameterBlockElement2` | Single element within a ParameterBlock |
| `TargetBinding2` | Per-target binding info (set, binding, size) |
| `GlobalShaderPass2` | Shader pass definition (entry point name pairs) |
| `TargetShaderPass2` | Per-target shader pass (compiled entry point refs) |
| `TargetEntryPoint2` | Per-target entry point (bytecode, vertex attributes) |

## 対応ターゲット

| Target | Profile | Usage |
| --- | --- | --- |
| SPIRV | glsl_450 | Vulkan |
| DXIL | sm_6_0 | Direct3D 12 |
| WGSL | - | WebGPU |
| Metal | glsl_450 | Apple Metal |

## バイナリフォーマット (`lcs2`)

```text
Header:  "lcs2" (4 bytes) + version (int16) + sourceName (string, v3 以降)
Section: "lcs2.bl." -- Code Blobs
Section: "lcs2.pb." -- ParameterBlockLayout
Section: "lcs2.te." -- TargetEntryPoint
Section: "lcs2.tp." -- TargetShaderPass
Section: "lcs2.gp." -- GlobalShaderPass
```
