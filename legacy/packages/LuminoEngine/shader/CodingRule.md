# Shader Coding Rule

Basically, follow the steps below:

1. [Slang coding-conventions](https://github.com/shader-slang/slang/blob/master/docs/design/coding-conventions.md)
2. HLSL code included in [DirectX-Graphics-Samples](https://github.com/microsoft/DirectX-Graphics-Samples)

## File Names

- `PascalCase`

> [!NOTE]
> This is the rule followed in the Lumino codebase.
> (The file is written in PascalCase because it matches the type names (i.e. C++ class names) of the elements it primarily exposes)
>
> The exception is the standard module barrel, `lumino.slang`, which is lowercase.
> これは、 slahder-slang では モジュール名 (ファイル名) を小文字としているため、 slang 等から提供される他のモジュールと合わせる目的もあります。

## ビルトイン uniform(global) 変数

- `ln_` で始め、後続は `PascalCase` です。


```hlsl
Sampler2D ln_MaterialTexture;
float4x4 ln_World;
```

## 0.10.0 以前との差や細かなメモ

### ln_World は ln_world じゃなくてよいのか？

元は gl_Position とかを参考にしたのと、まぁ camelCase とみなせなくもないだろうという判断。

### エントリポイントは VSMain や VS_Main じゃなくて vsMain でよいのか？

camelCase ということで。

あと _ は ln_ をはじめとしてビルトインの要素で沢山使っているので、ユーザーコードで目を引いてしまうかもしれない。
（エントリポイントなので、目を引く意味はあんまりなさそう）

### VSInput.Pos は VSInput.position でよいのか？

- camelCase とすることにした
- DX12 のサンプルコードの多くが position
- 略語の pos にはしない。 gl_Position が gl_Pos ではないのと同じように。


