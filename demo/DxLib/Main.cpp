#include "DxLib.h"
#include <d3d9.h>
#include <LuminoDxLib.h>
using namespace Lumino;

int PlayerX, PlayerY;
int PlayerGraph;

void DeviceLostFunction(void *Data)
{
	printf("Start LostFunction\n");

	printf("End LostFunction\n");
}
void DeviceRestoreFunction(void *Data)
{
	printf("Start RestoreFunction\n");
	printf("End RestoreFunction\n");
}

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
//	LPSTR lpCmdLine, int nCmdShow)
int main()
{
	// 初期化
	ChangeWindowMode(TRUE);
	SetUseDirect3D9Ex(FALSE);
	SetGraphMode(640, 480, 32);
	if (DxLib_Init() == -1 || SetDrawScreen(DX_SCREEN_BACK)) return -1;
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	ApplicationSettings appData;
	appData.GraphicsAPI = Graphics::GraphicsAPI::DirectX9;
	appData.RenderingType = Graphics::RenderingType::Immediate;
	appData.UserMainWindow = GetMainWindowHandle();
	appData.D3D9Device = (IDirect3DDevice9*)GetUseDirect3DDevice9();

	Application::Initialize(appData);

	// デバイスロスト時のコールバック設定(フルスクリーンウインドウ切り替えのために必要)
	SetGraphicsDeviceLostCallbackFunction(DeviceLostFunction, NULL);
	SetGraphicsDeviceRestoreCallbackFunction(DeviceRestoreFunction, NULL);

	// TODO: DxLib アーカイブからのロード

	// グラフィックのロード
	//PlayerGraph = LoadGraph("Player.bmp");

	// キャラクターの初期位置をセット
	PlayerX = 0;
	PlayerY = 0;


	int dummyGraph = MakeGraph(32, 32);

	// ループ
	bool isFullScreen = false;
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// キー入力取得
		int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

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

		// 何でもいいのでTransFragを有効にして画像を描画する。
		DrawGraph(0, 0, dummyGraph, TRUE);

		// 頂点バッファに溜まった頂点データを吐き出す
		RenderVertex();






		// プレイヤーを描画する
		//DrawGraph(PlayerX, PlayerY, PlayerGraph, TRUE);

		//app->Render();


		// DXライブラリの設定を戻す。
		RefreshDxLibDirect3DSetting();

		// 裏画面の内容を表画面に反映させる
		ScreenFlip();

		// フルスクリーン切り替え
		if (CheckHitKey(KEY_INPUT_F1) && !isFullScreen)
		{
			ChangeWindowMode(FALSE);
			SetDrawScreen(DX_SCREEN_BACK);
			isFullScreen = true;
		}
		if (CheckHitKey(KEY_INPUT_F2) && isFullScreen)
		{
			ChangeWindowMode(TRUE);
			SetDrawScreen(DX_SCREEN_BACK);
			isFullScreen = false;
		}
	}

	DxLib_End();

	Application::Finalize();

#if 0
	// 初期化
	ChangeWindowMode(TRUE);
	SetUseDirect3D9Ex(FALSE);
	SetGraphMode(640, 480, 32);
	if (DxLib_Init() == -1 || SetDrawScreen(DX_SCREEN_BACK)) return -1;
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);


	Application::ConfigData appData;
	appData.GraphicsAPI = Graphics::GraphicsAPI::DirectX9;
	appData.RenderingType = Graphics::RenderingType::Immediate;
	appData.UserMainWindow = GetMainWindowHandle();
	appData.D3D9Device = (IDirect3DDevice9*)GetUseDirect3DDevice9();
	Application* app = Application::Create(appData);


	// デバイスロスト時のコールバック設定(フルスクリーンウインドウ切り替えのために必要)
	SetGraphicsDeviceLostCallbackFunction(DeviceLostFunction, NULL);
	SetGraphicsDeviceRestoreCallbackFunction(DeviceRestoreFunction, NULL);

	// TODO: DxLib アーカイブからのロード

	// グラフィックのロード
	//PlayerGraph = LoadGraph("Player.bmp");

	// キャラクターの初期位置をセット
	PlayerX = 0;
	PlayerY = 0;


	int dummyGraph = MakeGraph(32, 32);

	// ループ
	bool isFullScreen = false;
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// キー入力取得
		int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

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

		// 何でもいいのでTransFragを有効にして画像を描画する。
		DrawGraph(0, 0, dummyGraph, TRUE);

		// 頂点バッファに溜まった頂点データを吐き出す
		RenderVertex();






		// プレイヤーを描画する
		//DrawGraph(PlayerX, PlayerY, PlayerGraph, TRUE);

		app->Render();


		// DXライブラリの設定を戻す。
		RefreshDxLibDirect3DSetting();

		// 裏画面の内容を表画面に反映させる
		ScreenFlip();

		// フルスクリーン切り替え
		if (CheckHitKey(KEY_INPUT_F1) && !isFullScreen)
		{
			ChangeWindowMode(FALSE);
			SetDrawScreen(DX_SCREEN_BACK);
			isFullScreen = true;
		}
		if (CheckHitKey(KEY_INPUT_F2) && isFullScreen)
		{
			ChangeWindowMode(TRUE);
			SetDrawScreen(DX_SCREEN_BACK);
			isFullScreen = false;
		}
	}

	app->Release();

	DxLib_End();
#endif
	return 0;
}