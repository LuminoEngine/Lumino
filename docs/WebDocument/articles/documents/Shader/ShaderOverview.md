シェーダの概要
==========

Lumino はグラフィックスのコアコンポーネントとしてプログラマブルシェーダをサポートしています。

Lumino のシェーダは [HLSL 言語](https://docs.microsoft.com/en-us/windows/desktop/direct3dhlsl/dx-graphics-hlsl-reference) に基づいて記述します。

また、頂点シェーダ、ピクセルシェーダ、レンダーステートをグループ化しプログラムからアクセスしやすくするために、 [DirectX のエフェクトフレームワーク](https://docs.microsoft.com/en-us/windows/desktop/direct3d11/d3d11-graphics-programming-guide-effects) に似た構文をサポートしています。

次のコードは、ユーザープログラムから色を受けとり、ジオメトリを塗りつぶす例です。

```hlsl
float4 _Color;

float4 VSMain(float4 Pos : POSITION) : SV_POSITION
{
    return Pos;
}

float4 PSMain() : SV_TARGET
{
    return _Color;
}

technique MainTech
{
    pass Pass0
    {
        VertexShader = VSMain;
        PixelShader = PSMain;
    }
}
```



