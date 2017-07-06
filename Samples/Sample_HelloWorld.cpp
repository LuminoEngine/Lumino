#include <Lumino.h>
using namespace ln;

LN_SAMPLE_MAIN(HelloWorld)
{
	// Lumino の初期化処理
	LN_SAMPLE_INITIALIZE;

	// 文字列を表示するための TextBlock2D オブジェクトを作成する
	auto text = TextBlock2D::create(_T("Hello, Lumino!"));

	// 表示する文字列の中央を原点とする
	text->setAnchorPoint(0.5, 0.5);

	// ウィンドウ中央に配置する
	text->setPosition(320, 240);

	// メインループ
	while (LN_SAMPLE_UPDATE)
	{
	}

	// Lumino の終了処理
	LN_SAMPLE_TERMINATE;
}
