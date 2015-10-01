//==============================================================================
// 音声ファイルをループ再生します。
//==============================================================================
#include <stdio.h>
#include <LuminoC.h>

int main()
{
	LNHandle sound;

	// 音声機能を初期化する
	if (LNApplication_InitializeAudio() != LN_OK) return 1;

	// 音声ファイルから Sound オブジェクトを作る
	if (LNSound_Create(_T("../Media/ln23.ogg"), &sound) != LN_OK) return 1;
	LNSound_SetVolume(sound, 1.0);			// 音量
	LNSound_SetPitch(sound, 1.5);			// ピッチ
	LNSound_SetLoopEnabled(sound, LN_TRUE);	// ループON

	// 再生
	LNSound_Play(sound);

	printf("Enter を押すと停止します...\n");
	getchar();

	// 停止
	LNSound_Stop(sound);

	// ライブラリの終了処理
	LNApplication_Terminate();
	return 0;
}
