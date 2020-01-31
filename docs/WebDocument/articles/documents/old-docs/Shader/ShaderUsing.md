シェーダの使い方
==========

Lumino はクロスプラットフォームシェーダを構築するために、[HLSL 言語](https://docs.microsoft.com/en-us/windows/desktop/direct3dhlsl/dx-graphics-hlsl-reference) で記述されたシェーダを GLSL などに変換します。

変換されたコンパイル済みシェーダ (.lcfx ファイル) には、複数のグラフィックス API 用に変換されたシェーダコードが含まれます。

プログラムからは [Shader クラス](../../reference-cpp/classln_1_1_shader.html) を使用してシェーダの定数バッファやパスの定義にアクセスします。


シェーダをコンパイルする (lumino-cli fxc)
----------
シェーダを変換するためのシェーダコンパイラは `lumino-cli` に統合されています。

例えば次のようなコマンドを実行すると、"example.fx" と同じフォルダに "example.lcfx" というコンパイル済みシェーダが作成されます。
```
lumino-cli fxc example.fx
```


シェーダをコンパイルする (Builtin shader compiler)
----------
デスクトップターゲットの Lumino は、Shader クラスに直接シェーダファイルを入力することでもコンパイル可能です。

ただしこれは開発中のデバッグ作業で使用することを想定しています。リリース環境では使用しないでください。


コンパイル済みシェーダ読み込む
----------

```cpp
auto shader = Shader::create("example.lcfx");
```




