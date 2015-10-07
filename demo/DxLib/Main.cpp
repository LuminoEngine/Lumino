#include "DxLib.h"
#include <d3d9.h>
#include <LuminoDxLib.h>
using namespace Lumino;

int PlayerX, PlayerY;
int PlayerGraph;

void DeviceLostFunction(void *Data)
{
	printf("Start LostFunction\n");
	Graphics::ChangeDirectX9Device(NULL);
	printf("End LostFunction\n");
}
void DeviceRestoreFunction(void *Data)
{
	printf("Start RestoreFunction\n");
	Graphics::ChangeDirectX9Device((IDirect3DDevice9*)GetUseDirect3DDevice9());
	printf("End RestoreFunction\n");
}

GUIContext* g_mainContext = NULL;

LRESULT CALLBACK HookDxLibMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_MOUSEMOVE:
		g_mainContext->InjectMouseMove(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	{
		float x = LOWORD(lParam);
		float y = HIWORD(lParam);
		switch (msg)
		{
		case WM_LBUTTONDOWN:
			g_mainContext->InjectMouseButtonDown(MouseButton::Left, x, y);
			break;
		case WM_LBUTTONUP:
			g_mainContext->InjectMouseButtonUp(MouseButton::Left, x, y);
			break;
		case WM_RBUTTONDOWN:
			g_mainContext->InjectMouseButtonDown(MouseButton::Right, x, y);
			break;
		case WM_RBUTTONUP:
			g_mainContext->InjectMouseButtonUp(MouseButton::Right, x, y);
			break;
		case WM_MBUTTONDOWN:
			g_mainContext->InjectMouseButtonDown(MouseButton::Middle, x, y);
			break;
		case WM_MBUTTONUP:
			g_mainContext->InjectMouseButtonUp(MouseButton::Middle, x, y);
			break;
		}
		return 0;
	}
	}
	//SetUseHookWinProcReturnValue(TRUE);
	return 0;
}

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
//	LPSTR lpCmdLine, int nCmdShow)
int main()
{
#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	// DX ライブラリの初期化
	bool isFullScreen = false;
	ChangeWindowMode(TRUE);
	SetDrawScreen(DX_SCREEN_BACK);
	//SetUseDirect3D9Ex(FALSE);
	SetUseDirect3DVersion(DX_DIRECT3D_9);	// DirectX9を使用するようにする。
	if (DxLib_Init() == -1 || SetDrawScreen(DX_SCREEN_BACK)) return -1;

	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// Zバッファを有効にする。
	SetUseZBuffer3D(TRUE);

	// Zバッファへの書き込みを有効にする。
	SetWriteZBuffer3D(TRUE);

	SetHookWinProc(HookDxLibMessage);

	//printf("%p\n", GetUseDirect3DDevice9());

	// Lumino 初期化
	ApplicationSettings appData;
	appData.GraphicsAPI = GraphicsAPI::DirectX9;
	appData.RenderingType = RenderingType::Immediate;
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


	{
		auto* d = (IDirect3DDevice9*)GetUseDirect3DDevice9();
		d->AddRef();
		printf("%p ref:%d\n", d, d->Release());
	}

	Graphics::ChangeDirectX9Device(NULL);

	Graphics::ChangeDirectX9Device((IDirect3DDevice9*)GetUseDirect3DDevice9());

#if 1
	GCPtr<GUIContext> context1 = GUIContext::Create();
	g_mainContext = context1;
	context1->InjectViewportSizeChanged(640, 480);


	GCPtr<PilePanel> panel1 = PilePanel::Create();
	context1->SetRootElement(panel1);

	GCPtr<UIButton> button1 = UIButton::Create();
	button1->SetSize(SizeF(200, 100));
	button1->Click += [](RoutedEventArgs* e) { printf("Click!\n"); };
	panel1->GetChildren()->Add(button1);

	//GCPtr<UIButton> button2 = UIButton::Create();
	//button2->SetSize(SizeF(100, 200));
	//panel1->GetChildren()->Add(button2);

	button1->SetEnabled(false);
#endif


	// ループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		printf("a\n");
		// 画面を初期化する
		ClearDrawScreen();

		DrawBox(0, 0, 640, 480, GetColor(0, 0, 255), TRUE);

		// 何でもいいのでTransFragを有効にして画像を描画する。
		DrawGraph(0, 0, dummyGraph, TRUE);

		// 頂点バッファに溜まった頂点データを吐き出す
		RenderVertex();


		context1->InjectElapsedTime(0.016);

		UI::UpdateAllContext();

		Renderer::BeginRendering();

		Renderer::Clear(ClearFlags::All, ColorF::White);
		UI::RenderAllContext();

		Renderer::EndRendering();


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
			printf("%p\n", GetUseDirect3DDevice9());
		}
		if (CheckHitKey(KEY_INPUT_F2) && isFullScreen)
		{
			ChangeWindowMode(TRUE);
			SetDrawScreen(DX_SCREEN_BACK);
			isFullScreen = false;
			printf("%p\n", GetUseDirect3DDevice9());
		}
	}

	DeleteGraph(dummyGraph);

	Graphics::ChangeDirectX9Device(NULL);
	Application::Finalize();

	{
		auto* d = (IDirect3DDevice9*)GetUseDirect3DDevice9();
		d->AddRef();
		printf("%p ref:%d\n", d, d->Release());
	}


	DxLib_End();


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