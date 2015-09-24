#include <iostream>
#include <LuminoC.h>

int main()
{
	LNResult r;

	//LNConfig_RegisterArchive(_T("D:/Proj/Lumino/build/bindings/C/Test/Debug/audio.lna"), _T("test"));
	LNApplication_InitializeAudio();

	LNVector3 pos = { 2, 0, 0 };

	LNHandle sound1;
	r = LNSound_Create(_T("D:/Proj/Lumino/build/bindings/C/Test/Debug/audio/ZIGG-ZAGG.mp3"), &sound1);
	const LNChar* str;
	LNError_GetLastErrorMessage(&str);
	LNSound_Set3DEnabled(sound1, LN_TRUE);
	LNSound_SetEmitterPosition(sound1, &pos);
	LNSound_Play(sound1);

	getchar();

	LNApplication_Terminate();
	return 0;
}

