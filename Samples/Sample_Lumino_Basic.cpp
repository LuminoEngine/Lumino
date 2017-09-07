#include <Lumino.h>
using namespace ln;

LN_SAMPLE_MAIN(Lumino, Basic, MinCode)
{
	// Lumino の初期化処理
	Engine::initialize();

	// メインループ
	while (Engine::update())
	{
	}

	// Lumino の終了処理
	Engine::terminate();
}

LN_SAMPLE_MAIN(Lumino, Basic, HelloWorld)
{
	// Lumino の初期化処理
	Engine::initialize();

	// 文字列を表示するための TextBlock2D オブジェクトを作成する
	auto text = TextBlock2D::create(_LT("Hello, Lumino!"));

	// 表示する文字列の中央を原点とする
	text->setAnchorPoint(0.5, 0.5);

	// ウィンドウ中央に配置する
	text->setPosition(320, 240);

	// メインループ
	while (Engine::update())
	{
	}

	// Lumino の終了処理
	Engine::terminate();
}
