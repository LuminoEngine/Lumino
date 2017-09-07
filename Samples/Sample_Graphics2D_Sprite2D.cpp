#include <Lumino.h>
using namespace ln;

// 画像を表示する
LN_SAMPLE_MAIN(Graphics2D, Sprite2D, Sample1)
{
	Engine::initialize();

	// 画像を読み込み、スプライトオブジェクトを作成する
	auto sprite = Sprite2D::create(_LT("SampleAssets/Scene1.jpg"));

	// 表示位置を指定する
	sprite->setPosition(110, 90);

	while (Engine::update())
	{
	}
}

// 画像を部分的に表示する
LN_SAMPLE_MAIN(Graphics2D, Sprite2D, SourceRect)
{
	Engine::initialize();

	auto sprite = Sprite2D::create(_LT("SampleAssets/Scene1.jpg"));

	// 表示する領域を指定する
	sprite->setSourceRect(Rect(50, 20, 300, 100));

	while (Engine::update())
	{
	}
}

// 基準位置を指定する
LN_SAMPLE_MAIN(Graphics2D, Sprite2D, AnchorPoint)
{
	Engine::initialize();

	auto sprite = Sprite2D::create(_LT("SampleAssets/Scene1.jpg"));

	// 画像の中央を基準とする
	sprite->setAnchorPoint(0.5, 0.5);

	// 表示位置を画面中央にする
	auto size = Engine::getMainViewport()->getViewSize();
	sprite->setPosition(size.width / 2, size.height / 2);

	while (Engine::update())
	{
	}
}
