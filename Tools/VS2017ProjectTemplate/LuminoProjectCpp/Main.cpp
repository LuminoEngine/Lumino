
#include <Lumino.h>
using namespace ln;

// Lumino アプリケーションのエントリーポイント
void Main()
{
	// Lumino の初期化処理
	Engine::initialize();

	// 画像を読み込み、2Dスプライトを作成する
	auto sprite = Sprite2D::create("Assets/icon256.png");

	// 2Dスプライトの位置を設定する
	sprite->setPosition(200, 100);

	// Lumino の更新処理 (ウィンドウへの描画などを行い、ウィンドウが閉じられていれば false を返す)
	while (Engine::update())
	{
	}

	// Lumino の終了処理
	Engine::terminate();
}
