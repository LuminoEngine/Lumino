画像を表示する
==========

画像を表示するためには、スプライトと呼ばれるオブジェクトを使用します。

単に画像と言っても、実際のシーンではキャラクターの位置や向き、透明度、アニメーションなど様々な動作が必要になります。スプライトには、そういった操作で必要となる機能が組み込まれています。


スプライトを表示する
----------

![](img/display-sprites-1.png)

画像ファイルを読み込み、スプライトを表示します。

```cpp
#include <Lumino.hpp>

void Main()
{
    Ref<Sprite> sprite = Sprite::create(u"icon.png");

    while (Engine::update())
    {
    }
}
```

`Ref` については [C++ プログラミングガイドのメモリ管理](../programming-guide/cpp.md#メモリ管理) を参照してください。

`Sprite::create(u"icon.png")` は、プロジェクトの Assets フォルダの中にある icon.png ファイルを読み込み、スプライトを作成します。

作成したスプライトは、Engine::update() によって画面に描画されます。


