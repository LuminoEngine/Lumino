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


