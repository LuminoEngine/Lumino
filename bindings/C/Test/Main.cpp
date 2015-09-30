#include <iostream>
#include <LuminoC.h>

#include <Windows.h>
HWND GetConsoleHwnd(void)
{
#define MY_BUFSIZE 1024 // コンソール ウィンドウのタイトル用のバッファサイズ
	HWND hwndFound;         // 呼び出し側へ返される値
	TCHAR pszNewWindowTitle[MY_BUFSIZE];
	// 作成されるウィンドウのタイトルが入ります
	TCHAR pszOldWindowTitle[MY_BUFSIZE]; // 元のウィンドウタイトルが入ります

	// 現在のウィンドウタイトルを取得します

	GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);

	// 独自に、ウィンドウの新規タイトルをフォーマットします

	wsprintf(pszNewWindowTitle, _T("%d/%d"),
		GetTickCount(),
		GetCurrentProcessId());

	// 現在のウィンドウタイトルを変更します

	SetConsoleTitle(pszNewWindowTitle);

	// ウィンドウタイトルのアップデートを確実なものにさせます

	Sleep(40);

	// ウィンドウの新規タイトルを探しにいきます

	hwndFound = FindWindow(NULL, pszNewWindowTitle);

	// 元のウィンドウタイトルへ戻します

	SetConsoleTitle(pszOldWindowTitle);

	return(hwndFound);
}

int main()
{
#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	LNResult r;

	//LNConfig_SetUserWindowHandle((intptr_t)GetConsoleHwnd());
	//LNConfig_SetDirectMusicMode(LN_DIRECTMUSICMODE_NORMAL);
	//LNConfig_SetDirectMusicReverbLevel(1.0f);

	//LNConfig_RegisterArchive(_T("D:/Proj/Lumino/build/bindings/C/Test/Debug/audio.lna"), _T("test"));
	LNApplication_InitializeAudio();

	//LNGameAudio_PlayBGM(_T("D:/tmp/ZIGG-ZAGG.mp3"));
	//::Sleep(2000);
	//LNGameAudio_PlayME(_T("D:/Proj/Lumino/bindings/DotNet/Samples/Media/lnme_victory1.ogg"));

	LNVector3 pos = { 2, 0, 0 };

	LNHandle sound1;
	r = LNSound_Create(_T("D:/Proj/Lumino/bindings/Common/Media/water02.wav"), &sound1);
	//const LNChar* str;
	//LNError_GetLastErrorMessage(&str);
	LNSound_Set3DEnabled(sound1, LN_TRUE);
	LNSound_SetEmitterPosition(sound1, &pos);
	LNSound_Play(sound1);

	//LNSound_Stop(sound1);
	//
	getchar();
//	::Sleep(2000);
//LNObject_Release(sound1);
//
	//::Sleep(2000);
	LNApplication_Terminate();
	return 0;
}

