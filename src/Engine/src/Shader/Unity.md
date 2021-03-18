
==========

ビルトイン変数
----------
https://docs.unity3d.com/Manual/SL-UnityShaderVariables.html
https://docs.unity3d.com/ja/2018.4/Manual/SL-UnityShaderVariables.html

### _LightMatrix0, unity_WorldToLight

これらは WorldPos を、ライトの ClipSpace へ変換するための行列。= ViewProjMatrix.
変換結果の .xy を .w で割って、シャドウマップテクスチャからサンプリングしたりする。

_LightMatrix0 は自動的に unity_WorldToLight に書き換えられるようだ。
https://teratail.com/questions/201804

