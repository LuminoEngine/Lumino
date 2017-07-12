#include <Lumino.h>
using namespace ln;

// 画像を表示する
LN_SAMPLE_MAIN(Graphics2D, Sprite2D)
{
	Engine::initialize();

	// 画像を読み込み、スプライトオブジェクトを作成する
	auto sprite = Sprite2D::create(_T("Scene1.jpg"));

	while (Engine::update())
	{
	}
}
