オブジェクトのメモリ管理
==========


参照カウント
----------
Lumino はメモリ管理の方法として、参照カウント方式を採用しています。

参照カウントはガベージコレクション戦略の１つで、Lumino ではオブジェクト自身が参照カウントを保持します。


RefObject クラス と Ref クラス
----------
RefObject は参照カウントを持つオブジェクトのベースクラスです。
Lumino のほとんどのオブジェクトは RefObject クラスから派生しています。

Ref クラスは RefObject を参照するためのスマートポインタクラスです。


オブジェクトの生成
----------
RefObject の派生クラスのオブジェクトを生成するには、makeRef を使用します。

makeRef は std::make_shared と同様に、受け取った引数リストでオブジェクトを構築し、スマートポインタで包んで返します。

```cpp
class MyObject : public ln::RefObject
{
    int a;
public:
    MyObject(int v)
        : a(v)
    {}
};

ln::Ref<MyObject> obj = ln::makeRef<MyObject>(5);
std::cout << obj->a;    // => 5
```



