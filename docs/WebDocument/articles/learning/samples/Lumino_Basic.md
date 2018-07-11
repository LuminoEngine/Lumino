Lumino の基本
====================

ウィンドウを表示するだけの最小限のプログラム
--------------------
![](img/Lumino_Basic_1.png)

```cpp
#include <Lumino.h>
using namespace ln;

void Main()
{
	// Lumino の初期化処理
	Engine::initialize();

	// メインループ
	while (Engine::update())
	{
	}
}
```

Hello world!
--------------------
![](img/Lumino_Basic_2.png)

```cpp
#include <Lumino.h>
using namespace ln;

void Main()
{
	// Lumino の初期化処理
	Engine::initialize();

	// 文字列を表示するための TextBlock2D オブジェクトを作成する
	auto text = TextBlock2D::create(_T("Hello, Lumino!"));

	// 表示する文字列の中央を原点とする
	text->setAnchorPoint(0.5, 0.5);

	// ウィンドウ中央に配置する
	text->setPosition(320, 240);

	// メインループ
	while (Engine::update())
	{
	}
}
```
