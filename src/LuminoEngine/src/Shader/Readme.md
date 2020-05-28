Shader
==========

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
