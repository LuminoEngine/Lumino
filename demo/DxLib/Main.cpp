#include "DxLib.h"

int PlayerX, PlayerY;
int PlayerGraph;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	int Key;
	ChangeWindowMode( TRUE ) ;

	// 画面モードのセット
	SetGraphMode(640, 480, 16);
	if (DxLib_Init() == -1)	// ＤＸライブラリ初期化処理
	{
		return -1;				// エラーが起きたら直ちに終了
	}

	// 描画先画面を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	// グラフィックのロード
	//PlayerGraph = LoadGraph("Player.bmp");

	// キャラクターの初期位置をセット
	PlayerX = 0;
	PlayerY = 0;

	// ループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// キー入力取得
		Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

		// 上を押していたら上に進む
		if (Key & PAD_INPUT_UP) PlayerY -= 3;

		// 下を押していたら下に進む
		if (Key & PAD_INPUT_DOWN) PlayerY += 3;

		// 右を押していたら右に進む
		if (Key & PAD_INPUT_RIGHT) PlayerX += 3;

		// 左を押していたら左に進む
		if (Key & PAD_INPUT_LEFT) PlayerX -= 3;

		// 画面を初期化する
		ClearDrawScreen();

		// プレイヤーを描画する
		//DrawGraph(PlayerX, PlayerY, PlayerGraph, TRUE);

		// 裏画面の内容を表画面に反映させる
		ScreenFlip();
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}