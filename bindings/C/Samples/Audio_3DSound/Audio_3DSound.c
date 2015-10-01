//==============================================================================
// 3D サウンドのサンプルです。
//==============================================================================
#include <stdio.h>
#include <LuminoC.h>

int main()
{
	LNHandle sound;
	LNVector3 soundPos = { 12.0, 0.0, 0.0 };
	LNVector3 listenerPos = { 5, 0.0, 0.0 };
	LNVector3 listenerDir = { 0.0, 0.0, 1.0 };
	LNVector3 listenerVel = { 2.0, 0.0, 0.0 };

	// 音声機能を初期化する
	LNApplication_InitializeAudio();

	// 音声ファイルから Sound オブジェクトを作る
	LNSound_Create(_T("../Media/water02.wav"), &sound);
	LNSound_Set3DEnabled(sound, LN_TRUE);			// 3D 音声として再生する
	LNSound_SetEmitterPosition(sound, &soundPos);	// 発声位置
	LNSound_SetEmitterMaxDistance(sound, 10.0);		// この距離まで聞こえる
	LNSound_SetLoopEnabled(sound, LN_TRUE);			// ループON
	
	// リスナーの位置・向き・速度を指定する
	LNSoundListener_SetPosition(&listenerPos);
	LNSoundListener_SetDirection(&listenerDir);
	LNSoundListener_SetVelocity(&listenerVel);

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
