# lumino_shader

Slang シェーダをコンパイルし、マルチターゲット (SPIRV / DXIL / WGSL / Metal) のバイトコードとリフレクション情報を統合バイナリ (`.lcsh`) にまとめるパッケージ。

## 設計方針

シェーダリソースは **更新頻度別** に `ParameterBlock<T>` で分離する。

```text
Set 0 (per-view)     : カメラ・ライトなど — フレームに1回更新
Set 1 (per-material)  : マテリアルパラメータ・テクスチャ — マテリアル切り替え時
Set 2 (per-object)    : ワールド行列 — オブジェクトごと
```

Slang の `ParameterBlock<T>` が独立した descriptor set にマップされるため、クライアント側は set 番号と中身の要素だけ知ればよい。名前に意味を持たせるのはクライアント側の役割。

## シェーダの書き方

```slang
struct ViewParams {
    float4x4 viewProj;
    float4   cameraPos;
};

struct MaterialParams {
    float4 color;
    float4 specular;
};

struct MaterialData {
    ConstantBuffer<MaterialParams> params;
    Texture2D                      baseTexture;
    SamplerState                   baseSampler;
};

struct ObjectParams {
    float4x4 world;
    float4x4 normalMatrix;
};

ParameterBlock<ViewParams>     viewData;       // -> Set 0
ParameterBlock<MaterialData>   materialData;   // -> Set 1
ParameterBlock<ObjectParams>   objectData;     // -> Set 2

[shader("vertex")]
VSOutput vsMain(VSInput input) { ... }

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

### ParameterBlock のルール

- plain data のみの構造体 (`ViewParams`, `ObjectParams`) → 暗黙の ConstantBuffer として扱われる。C++ 側からはアライメント済みの構造体をそのままセットする。
- 明示的な `ConstantBuffer<T>`, `Texture2D`, `SamplerState` を混在させる場合 (`MaterialData`) → 各フィールドが個別のバインディングになる。
- `$Global` (bare uniform 変数) は非サポート。すべてのリソースは `ParameterBlock` に含める。

## C++ からの使い方

### コンパイル

```cpp
#include <lumino_shader/ShaderCompiler2.hpp>
#include <lumino_shader/UnifiedShader2.hpp>

auto compiler = *ShaderCompiler2::create();
compiler->addSearchPath("path/to/includes");  // 必要に応じて

auto result = compiler->build("BasicLit.slang");
if (!result) {
    std::cerr << result.error().message << std::endl;
    return;
}
UnifiedShader2* shader = compiler->shader();
```

### リフレクション情報の取得

```cpp
// ParameterBlock の列挙
for (const auto& block : shader->parameterBlocks()) {
    // block.name      : "viewData", "materialData", "objectData"
    // block.setIndex   : descriptor set 番号 (0, 1, 2)
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
Header:  "lcs2" (4 bytes) + version (int16)
Section: "lcs2.bl." -- Code Blobs
Section: "lcs2.pb." -- ParameterBlockLayout
Section: "lcs2.te." -- TargetEntryPoint
Section: "lcs2.tp." -- TargetShaderPass
Section: "lcs2.gp." -- GlobalShaderPass
```
