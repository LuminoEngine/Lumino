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










