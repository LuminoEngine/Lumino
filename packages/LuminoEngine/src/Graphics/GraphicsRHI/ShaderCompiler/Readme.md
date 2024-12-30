Shader
==========


命名規則 (2020/6/15 時点)
----------

基本方針は [DirectX-Graphics-Samples](https://github.com/microsoft/DirectX-Graphics-Samples) に含まれている HLSL コードに従います。

### 構造体・関数

- `PascalCase` を使います。
- `_` は含みません。これはビルトインのシンボルで名前空間を表現するために使用します。

```hlsl
// bad
struct VS_Input { }
float3 getNormal() { }
float4 PS_Main() { }

// good
struct VSInput { }
float3 GetNormal() { }
float4 PSMain() { }
```

### 変数

- `camelCase` を使います。
- `_` は含みません。これはビルトインのシンボルで名前空間を表現するために使用します。

```hlsl
// bad
float Dot_L;
float3 Position: POSITION;  // struct or cbuffer member.

// good
float dotL;
float3 position: POSITION;  // struct or cbuffer member.
```

### 定数


### 公開シンボル (uniform 変数)

- `camelCase` を使い、`_` プレフィックスを付けます。


### 定数バッファ (uniform 変数)

考え中…


ライブラリとしての命名規則
----------

### ビルトインの変数と関数

公開用のシンボルは名前空間を表現するために次の規則を設けます。

- 変数は `ln_` プレフィックスを付けます。
- 関数は `LN_` プレフィックスを付けます。




----------

計画中…

```plantuml
left to right direction
package "Effect" {
    package "CodeContainers" {
        [CodeContainer.MyVSMain#1]
        [CodeContainer.MyPSMain#1]
        [CodeContainer.MyVSMain#2]
        [CodeContainer.MyPSMain#2]
    }
    package "DescriptorLayouts" {
        [DescriptorLayout.$Global]
        [DescriptorLayout.MyBuffer]
    }
    package "Technique#1" {
        package "Pass#1" {
            [VertexShaderCodeRef] as VertexShaderCodeRef_Pass1
            [PixelShaderCodeRef] as PixelShaderCodeRef_Pass1
            package "DescriptorLayouts#1" {
                [DescriptorLayoutRef.$Global] as DescriptorLayoutRef_Global_Pass1
                [DescriptorLayoutRef.MyBuffer] as DescriptorLayoutRef_MyBuffer_Pass1
            }
        }
        package "Pass#2" {
            [VertexShaderCodeRef] as VertexShaderCodeRef_Pass2
            [PixelShaderCodeRef] as PixelShaderCodeRef_Pass2
            package "DescriptorLayouts#2" {
                [DescriptorLayoutRef.$Global] as DescriptorLayoutRef_Global_Pass2
            }
        }
    }
}

[VertexShaderCodeRef_Pass1] --> [CodeContainer.MyVSMain#1]
[PixelShaderCodeRef_Pass1] --> [CodeContainer.MyPSMain#1]
[DescriptorLayoutRef_Global_Pass1] --> [DescriptorLayout.$Global]
[DescriptorLayoutRef_MyBuffer_Pass1] --> [DescriptorLayout.MyBuffer]
[VertexShaderCodeRef_Pass2] --> [CodeContainer.MyVSMain#2]
[PixelShaderCodeRef_Pass2] --> [CodeContainer.MyPSMain#2]
[DescriptorLayoutRef_Global_Pass2] --> [DescriptorLayout.$Global]
```

- 0.9.0 時点では xxxxShader と CodeContainer は 1:1. #define などが無い場合はコードが共通化できるが、その一致確認は未実装。
- 0.9.0 時点では、グローバルなところにある DescripterLayout は未実装。このため #136 の問題が出ている。
- DescriptorLayoutRef は、どのフィールドが alive (本当に使われているか) を持っている。
- ConstantBuffer は Pass 間で共有。0.9.0 では Shader が持っているが、今後は Material に持たせる。
    - Material は、Shader が割り当てられた時、Global の DescriptorLayouts をもとに ContantBuffer を作る。
    - この ConstantBuffer へ書き込むときは、Pass が持っている DescriptorLayoutRef を元に、alive なフィールドにのみ書き込む。

### Material との関係

```plantuml
left to right direction
package "Material" {
    package "Shader" {
        [Submit()]
        [Mapper(DescripterLayout)]
    }
    package "ShaderDescripter" {
        [UniformBuffer]
        [Samplers]
    }
}

package "UserProgram" {
    setFloat --> [Mapper(DescripterLayout)]
    setTexture --> [Samplers]
    setData --> [UniformBuffer]
}

package "RHI Thread" {
    [UniformBufferRHI]
}

[Mapper(DescripterLayout)] -> [UniformBuffer]
[UniformBuffer] --> [Submit()]
[Samplers] --> [Submit()]
[Submit()] --> [UniformBufferRHI] : Submit(Create bluk data)
```

- Material は setShader と同時に、ShaderDescripter を作成する。


0.9.0 時点の トランスパイラの動作について
----------

### register と binding

Lumino としては入力は HLSL になるが、GLSL へは次のような考え方でマップする。

- register 種類 -> GLSL の set
- register(bX) の X -> GLSL の binding

register と set の対応は次の通り。

| register | set (DescriptorSet) |
|----------|----------|
| b        | 0        |
| t        | 1        |
| s        | 2        |

イメージとしては次のような変換が行われる。

```
cbuffer ConstBuff0 : register(b1) { ... };
```

to

```
layout(set=0, binding=1) uniform ConstBuff0 { ... };
```

ただし、これはあくまでイメージ。0.9.0 時点では、実装優先で、次のようなルールでマップされている。

- binding はレジスタ要素を見つけた順番で、0 から番号を割り振る。ユーザーが HLSL で指定したレジスタ番号は無視される。
    - `TODO:` ユーザプログラムで UBO などを名前検索するときは問題ないが、レジスタ番号指定でデータをセットするときに問題になるので、修正が必要。
- register の種類は、変数の型によって再割り当てされる。
    - textureXD は register 't' (set=1) に割り当てられる。
    - SamplerState は register 's' (set=2) に割り当てられる。
    - `samplerXD は register 't' (set=1) に割り当てられる。`
        - `TODO:` これも実装優先のための現状仕様だが、実際に HLSL 手書きするときは register(s0) とかを使うことが多いので、合わせたほうがいいだろう。

実際の動きは `Test_Shader_UnifiedShader.LayoutTest2` を参照すること。

このマップ処理は Lumino ShaderCodeTranspiler::mapIOAndGenerateSpirv() で行われる。



用語
----------

### Uniform-buffer vs Constant-buffer

- DirectX: Constant-buffer
- Metal: 単に Buffer? ググると Uniform-buffer で使ってる記事が多く見える。
- Vulkan: Uniform-buffer
- WebGPU: https://gpuweb.github.io/gpuweb/#buffer-usage

多数決で Uniform-buffer で。


コンパイルの流れ
----------
UnifiedShaderCompiler がエントリポイント。
- UnifiedShaderCompiler::compile() でコンパイルする。
- UnifiedShaderCompiler::link() でリンクする。
- UnifiedShaderCompiler::unifiedShader() で結果を取得する。



```plantuml
start
:Hello world;
:This is on defined on
several **lines**;
stop
```

OpenGL のサンプラーオブジェクトについて
----------
HLSL(5) では SamplerObject と TextureObject は分離されているが、GLSL では 統合されている。

SPIRV-Cross は、この差を埋めるための機能を提供している。

例えば
```
Texture2D ln_MaterialTexture;
SamplerState ln_MaterialTextureSamplerState;
...
color = ln_MaterialTexture.Sample(ln_MaterialTextureSamplerState, uv);
```
というコードは、
```
uniform sampler2D lnCIS_lnT_ln_MaterialTexture_lnS_ln_MaterialTextureSamplerState;
...
color = texture(lnCIS_lnT_ln_MaterialTexture_lnS_ln_MaterialTextureSamplerState, uv);
```
に変換される。

C++ 側ではこの lnCIS_lnT_ln_MaterialTexture_lnS_ln_MaterialTextureSamplerState に対して、Texture と Sampler 情報をセットする必要がある。


### DirectX との互換性について
Lumino としては、Unity などと同じく、基本は Texture が SamplerState を持っている。

で、API が DirectX の時は「texture 型の変数に set したとき、別に SamplerState 型で、名前の末尾が SamplerState である変数」を見つけたとき、
Texture が持っている SamplerState をセットする。(2018/12/30 時点では未実装)


### 変数名について
この変数名は自分で考える必要がある。

- lnCIS_ : マジックナンバー
- lnT_<変数名> : texture 型変数の名前
- lnS_<変数名> : SamplerState 型変数の名前


Issue? Global な uniform だけに layout(row_major) がつく？
----------

```
// test.frag (HLSL)
#pragma pack_matrix(row_major)

float4x4 _CameraProjectionMatrix;

cbuffer LNRenderViewBuffer
{
    float4x4 ln_Projection;
};

float4 main()
{
    float4 c = float4(1, 0, 0, 1);
    c = mul(c, _CameraProjectionMatrix);
    c = mul(c, ln_Projection);
    return c;
}
```

```
C:\VulkanSDK\1.2.141.0\Bin\glslangValidator -e main -o test.spv -V -D test.frag
C:\VulkanSDK\1.2.141.0\Bin\spirv-dis.exe test.spv
```

```
OpName %_Global "$Global"
OpMemberName %_Global 0 "_CameraProjectionMatrix"
OpName %_ ""
OpName %LNRenderViewBuffer "LNRenderViewBuffer"
OpMemberName %LNRenderViewBuffer 0 "ln_Projection"
OpName %__0 ""
OpName %_entryPointOutput "@entryPointOutput"
OpMemberDecorate %_Global 0 RowMajor            ← _CameraProjectionMatrix は RowMajor
OpMemberDecorate %_Global 0 Offset 0
OpMemberDecorate %_Global 0 MatrixStride 16
OpDecorate %_Global Block
OpDecorate %_ DescriptorSet 0
OpDecorate %_ Binding 0
OpMemberDecorate %LNRenderViewBuffer 0 ColMajor     ← ln_Projection は ColMajor
OpMemberDecorate %LNRenderViewBuffer 0 Offset 0
OpMemberDecorate %LNRenderViewBuffer 0 MatrixStride 16
```

`#pragma pack_matrix(row_major)` がない場合は↓

```
OpName %_Global "$Global"
OpMemberName %_Global 0 "_CameraProjectionMatrix"
OpName %_ ""
OpName %LNRenderViewBuffer "LNRenderViewBuffer"
OpMemberName %LNRenderViewBuffer 0 "ln_Projection"
OpName %__0 ""
OpName %_entryPointOutput "@entryPointOutput"
OpMemberDecorate %_Global 0 RowMajor            ← _CameraProjectionMatrix は RowMajor
OpMemberDecorate %_Global 0 Offset 0
OpMemberDecorate %_Global 0 MatrixStride 16
OpDecorate %_Global Block
OpDecorate %_ DescriptorSet 0
OpDecorate %_ Binding 0
OpMemberDecorate %LNRenderViewBuffer 0 RowMajor     ← ln_Projection は RowMajor
OpMemberDecorate %LNRenderViewBuffer 0 Offset 0
OpMemberDecorate %LNRenderViewBuffer 0 MatrixStride 16
```


```
C:\VulkanSDK\1.2.141.0\Bin\dxc -spirv -T ps_6_0 -E main test.frag -Fo test.spv
```
dxc を使うと、両方 ColMajor になる。glslang の問題っぽい。







https://github.com/KhronosGroup/glslang/issues/789#issuecomment-288481630

HLSL is row-major oriented. (行優先)
SPIR-V is column-major oriented. (列優先)


### 行列乗算の書き方の一般的なところ

VulkanExample より

```
gl_Position = ubo.projection * ubo.view * ubo.model * boneTransform * vec4(inPos.xyz, 1.0);
```

DirectX-Graphics-Samples より

```
result.position = mul(float4(input.position, 1.0f), g_mWorldViewProj);
```

HLSL で書くなら、やっぱり matrix は後ろに持ってきたいかなぁ…。そうすると以下どちらかの対策が必要になる。
- HLSL の行列を全部 row_major で修飾する or #pragma pack_matrix(row_major)
- C++ 側で行列を転置する。





```
layout(std140) uniform _Global
{
    layout(row_major) mat4 _CameraProjectionMatrix;
    layout(row_major) mat4 _CameraInverseProjectionMatrix;
} _129;

layout(std140) uniform LNRenderViewBuffer
{
    mat4 ln_View;
    mat4 ln_Projection;
} _241;
```

float4 pos = (mul(_CameraInverseProjectionMatrix, clipPosition)); で正しい変換となった。
float4 pos = (mul(clipPosition, ln_Projection)); で正しい変換となった。


GLSL と HLSL の Matrix layout (Matrix storage order)
----------

### 実際の動作

- デフォルトの状態では、HLSL の floatXxX を GLSL の matXX に変換すると、`layout(row_major)` で修飾される。(例： layout(row_major) mat4 viewProjection;)
- HLSL で `#pragma pack_matrix(row_major)` を宣言すると、変換された GLSL から `layout()` 修飾は消える。(GLSL のデフォルトになる)

これらのことから、
- `HLSL は HLSL の世界で`、 column_major がデフォルト
- `GLSL は GLSL の世界で`、 column_major がデフォルト

と推測できる。

…本当に？




[MSDN の GLSL-HLSL 対応表](https://docs.microsoft.com/ja-jp/windows/uwp/gaming/glsl-to-hlsl-reference) では、次のように説明されている。

- GLSL: Row-major matrices (default)
- HLSL: Column-major matrices (default)
    - Row-major にしたいときは row_major で変数を修飾する必要がある。

### glslang の動作

ものすごく注意点として、`HLSL の layout 指定と、glslang の内部フォーマットは逆` だった。

`#pragma pack_matrix(row_major)` をパースするときに、逆転させている。
https://github.com/KhronosGroup/glslang/blob/master/hlsl/hlslParseHelper.cpp#L543-L551

また、HLSL の layout 指定のトークンを解析するときにも逆転している。
https://github.com/KhronosGroup/glslang/blob/master/hlsl/hlslGrammar.cpp#L690-L694





https://github.com/KhronosGroup/glslang/blob/master/hlsl/hlslParseHelper.cpp#L76

https://www.khronos.org/opengl/wiki/Interface_Block_(GLSL)
GLSL行列は常に列優先です。この仕様は、GLSLがバッファからデータをフェッチする方法のみを変更します。

### とりあえず方針

Lumino 側で転置するのは大変なので、シェーダが UBO をフェッチするときの設定に任せたい。

そのためできるだけ ShaderTranspiler の中でカバーする。

なお、もし Lumino 側で転置するとなると、4x4 行列以外も考えないとならないので負担激増する。
