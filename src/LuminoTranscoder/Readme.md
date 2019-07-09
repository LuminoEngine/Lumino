LuminoTranscoder
==========

`Draft`


構造体
----------

- `LN_STRUCT()` で修飾します。
- 構造体は POD 型でなければなりません。
- メンバ変数はすべて public かつ `LN_FIELD()` で修飾しなければなりません。




クラス
----------

### Static



メソッド
----------


### メソッドのオーバーロード

属性オプションとして、`OverloadPostfix` の指定が必須となります。

```
/** ... */
LN_METHOD()
void setPosition(const Vector3& pos);

/** ... */
LN_METHOD(OverloadPostfix = "XYZ")
void setPosition(float x, float y, float z);
```

### デフォルト引数

デフォルト引数として使用できる型は次の通りです。クラスや構造体は使用できません。

- 整数 (int)
- 実数 (float, double)
- Enum

```.cpp
int import(const StringRef& filePath, TextureFormat format = TextureFormat::RGBA8);
```

#### FlatC

デフォルト引数はサポートされません。デフォルト引数の無い関数宣言が生成されます。

```.c
LN_FLAT_API LnResult LnTest_Import(const LnChar* filePath, LnTextureFormat format, int* outReturn);
```


