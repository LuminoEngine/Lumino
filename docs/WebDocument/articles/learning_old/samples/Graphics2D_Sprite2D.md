画像を表示する
====================

画像を表示するには スプライト という機能を使う方法が最も簡単です。
ここでは 2D 空間に スプライト を配置する Sprite2D オブジェクトについて説明します。

画像を表示する
--------------------
![](img/Graphics2D_Sprite2D_1.png)

```cpp
#include <Lumino.h>
using namespace ln;

void Main()
{
    Engine::initialize();

    // 画像を読み込み、スプライトオブジェクトを作成する
    auto sprite = Sprite2D::create("SampleAssets/Scene1.jpg");

    while (Engine::update())
    {
    }
}
```

部分的に表示する
--------------------
![](img/Graphics2D_Sprite2D_2.png)

```cpp
#include <Lumino.h>
using namespace ln;

void Main()
{
    Engine::initialize();

    auto sprite = Sprite2D::create("SampleAssets/Scene1.jpg");

    // 切り出す領域を指定する
    sprite->SetSrc(Rect(100, 200, 200, 100));
    
    while (Engine::update())
    {
    }
}
```

基準位置を指定する
--------------------
![](img/Graphics2D_Sprite2D_3.png)

Lumino は「アンカーポイント」という名前で基準位置を表します。

設定には setAnchorPoint() を使用します。

```cpp
LN_SAMPLE_MAIN(Graphics2D, Sprite2D, AnchorPoint)
{
	Engine::initialize();

	auto sprite = Sprite2D::create("SampleAssets/Scene1.jpg");

	// 画像の中央を基準とする
	sprite->setAnchorPoint(0.5, 0.5);

	// 表示位置を画面中央にする
	auto size = Engine::getMainViewport()->getViewSize();
	sprite->setPosition(size.width / 2, size.height / 2);

	while (Engine::update())
	{
	}
}
```
