Import Rule
====================

クラス
--------------------
`LN_CLASS(...)` で修飾します。

```
LN_CLASS([specifier, ...])
class ClassName { };
```

### Specifiers

#### Static
static クラスであることを示します。

static クラスは、static であるメンバしか持つことはできません。
（C# の static クラスと同様です）





プロパティ
--------------------
`LN_METHOD(Property)` で修飾します。

関数名の先頭が `set` である関数は、setter となり、それ以外の関数は getter となります。

関数名の先頭が `set`, `get`, `is` のいずれかである場合、それを除いた名前がプロパティ名となります。

setter 関数は引数が1つで、戻り値が void でなければなりません。
getter は引数が無く、戻り値が void 以外でなければなりません。



