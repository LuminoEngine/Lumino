Shader
==========





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










