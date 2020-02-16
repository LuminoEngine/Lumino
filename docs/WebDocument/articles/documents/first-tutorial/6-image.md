画像を描画する
==========

この章では、2D 画像を表す `Texture` と、それを効率的にワールド上に表示する `Sprite` について学びます。


画像ファイルを読み込んで描画する
----------

2D 画像を表示するには `Sprite` を使います。

Sprite は画像を張り付けて表示するための板のようなものであると考えてください。

ここでは次の画像を表示してみます。チュートリアル冒頭で作成したプロジェクトの `assets` フォルダの中に、`picture1.jpg` というファイルが入っていることを確認して実行してください。

![](img/picture1.jpg)

# [C++](#tab/lang-cpp)
```cpp
#include <Lumino.hpp>

class App : public Application
{
    virtual void onInit() override
    {
        auto sprite = Sprite::load(u"picture1.jpg");
    }
};

LUMINO_APP(App);
```
# [Ruby](#tab/lang-ruby)
```ruby

```
---

![](img/image-1.png)



TODO: ライト削除



### 対応している画像フォーマット

画像フォーマットの読み込みがサポートされています。

| フォーマット | 拡張子 | 対応状況 |
|---|---|---|
| PNG | png | ✔ |
| JPEG | jpg, jpeg | ✔ |
| BMP | bmp | ✔ |
| GIF | gif | ✔ (アニメーション非対応) |
| TGA | tga | ✔ |


