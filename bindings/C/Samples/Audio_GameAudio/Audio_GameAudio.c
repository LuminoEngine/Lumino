//==============================================================================
// ゲーム用の音声再生機能のサンプルです。
//==============================================================================
#include <stdio.h>
#include <LuminoC.h>

int main()
{
	// 音声機能を初期化する
	LNApplication_InitializeAudio();

	printf("Enter を押すと BGM を再生します...\n");
	getchar();	// Enter 待ち

	// BGM を再生する
	LNGameAudio_PlayBGM(_T("../Media/ln23.ogg"), 1, 1, 0);

	printf("Enter を押すと BGS を再生します...\n");
	getchar();	// Enter 待ち

	// BGS を再生する
	LNGameAudio_PlayBGS(_T("../Media/water02.wav"), 0.5, 1, 0);

	printf("Enter を押すと ME を再生します...\n");
	getchar();	// Enter 待ち

	// ME を再生する
	LNGameAudio_PlayME(_T("../Media/lnme_victory1.ogg"), 1, 1);

	printf("Enter を押すと SE を再生します...\n");
	getchar();	// Enter 待ち

	// SE を再生する
	LNGameAudio_PlaySE(_T("../Media/ln_cursor_1.wav"), 1, 1);

	printf("Enter を押すと BGM と BGS を停止します...\n");
	getchar();	// Enter 待ち

	// BGM を停止する (5秒かけてフェードアウト)
	LNGameAudio_StopBGM(5.0);

	// BGS を停止する
	LNGameAudio_StopBGS(0.0);

	printf("Enter を押すと終了します...\n");
	getchar();	// Enter 待ち

	// ライブラリの終了処理
	LNApplication_Terminate();
	return 0;
}
