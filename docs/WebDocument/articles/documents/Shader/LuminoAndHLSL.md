Lumino 固有の HLSL サポート
==========


※保留


SamplerState
----------
Lumino の SamperState は、Texture に結合されています。これは、古い (DirectX9 世代の) HLSL を使用したときのデフォルトの動作です。

一方、Lumino がサポートしている HLSL 5.0 でテクスチャサンプリングを行う一般的なコードは次の通りです。
```
texture2D _MainTexture;
SamplerState _MainSamplerState;
...
float4 color = _MainTexture.Sample(_MainSamplerState, uv);
```

このように HLSL では Texture と SamplerState が分かれていますが、



