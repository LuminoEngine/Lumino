#include <Lumino.h>
using namespace ln;


LN_SAMPLE_MAIN(HelloWorld)
{
	LN_SAMPLE_INITIALIZE;

	// 文字列を表示するための TextBlock2D オブジェクトを作成する
	auto text = TextBlock2D::create(_T("Hello, Lumino!"));

	// TextBlock2D オブジェクトの中央を原点とする
	text->setAnchorPoint(0.5, 0.5);

	// ウィンドウ中央に配置する
	text->setPosition(320, 240);

	// メインループ
	while (Engine::update())
	{
	}

	LN_SAMOLE_TERMINATE;
}



void Main_HelloWorld()
{
	LN_SAMPLE_INITIALIZE;

	// 文字列を表示するための TextBlock2D オブジェクトを作成する
	auto text = TextBlock2D::create(_T("Hello, Lumino!"));

	// TextBlock2D オブジェクトの中央を原点とする
	text->setAnchorPoint(0.5, 0.5);

	// ウィンドウ中央に配置する
	text->setPosition(320, 240);

	// メインループ
	while (Engine::update())
	{
	}

	LN_SAMOLE_TERMINATE;
}

