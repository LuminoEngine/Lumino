# luminosc

Lumino シェーダーコンパイラ CLI ツール。  
Slang シェーダーソース (`.slang`) をクロスコンパイルし、Lumino ランタイムで読み込める `.lcsh` バイナリを生成します。

## 使い方

```sh
# 通常コンパイル（ダンプなし）
luminosc input.slang

# ダンプファイルを出力する場合
luminosc --dump input.slang

# ヘルプ表示
luminosc --help
```

入力ファイルと同じディレクトリに以下の 2 ファイルが出力されます。

| 出力ファイル | 内容 |
|---|---|
| `<name>.lcsh` | バイナリ形式のコンパイル済みシェーダー (SPIR-V, DXIL, WGSL, Metal) |
| `<name>.lcsh.inl` | C++ ソースに埋め込むための hex 配列 |

### 例

```sh
luminosc assets/shaders/Standard.slang
# => assets/shaders/Standard.lcsh
# => assets/shaders/Standard.lcsh.inl
```

生成された `.lcsh.inl` は C++ から以下のように利用できます。

```cpp
static const unsigned char kShaderData[] = {
#include "Standard.lcsh.inl"
};

auto result = UnifiedShaderSerializer::loadFromData(kShaderData, sizeof(kShaderData));
```

## シェーダーソースの書き方

`.slang` ファイル内に `LUMINO_METADATA` ブロックを記述して、パス名とエントリポイントを宣言します。

```hlsl
#ifdef LUMINO_METADATA
{
    "passes": [
        {
            "name": "Forward",
            "vertex": "vsMain",
            "fragment": "fsMain"
        }
    ]
}
#endif

struct VSInput {
    float3 position : POSITION;
    float2 uv       : TEXCOORD0;
};

struct VSOutput {
    float4 position : SV_Position;
    float2 uv       : TEXCOORD0;
};

[shader("vertex")]
VSOutput vsMain(VSInput input) { ... }

[shader("fragment")]
float4 fsMain(VSOutput input) : SV_Target { ... }
```

### LUMINO_METADATA フィールド

| フィールド | 説明 |
|---|---|
| `passes[].name` | パス名（必須） |
| `passes[].vertex` | 頂点シェーダーのエントリポイント関数名 |
| `passes[].fragment` | フラグメントシェーダーのエントリポイント関数名 |
| `passes[].compute` | コンピュートシェーダーのエントリポイント関数名 |

## 対応ターゲット

コンパイラは入力ソースからすべてのターゲットを同時に生成します。

| ターゲット | 用途 |
|---|---|
| SPIR-V | Vulkan |
| DXIL | DirectX 12 |
| WGSL | WebGPU |
| Metal | Apple |

## .lcsh バイナリフォーマット

チャンクベースのバイナリ形式です。`UnifiedShaderSerializer::loadFromData()` でデシリアライズし、`UnifiedShader` オブジェクトとして各ターゲットのコンパイル済みバイトコード・リフレクション情報にアクセスできます。
