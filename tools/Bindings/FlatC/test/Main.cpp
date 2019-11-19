
#include <stdio.h>
#include <assert.h>
#include <Lumino.FlatC.generated.h>

#ifdef _WIN32
#include <crtdbg.h>
#endif

int main()
{
#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	LnEngine_Initialize();


	// Get 系の関数を複数回呼び出しても、参照カウントは変わらないこと。
	// (Create した場合は明示的に Release する必要があるが、Get の場合 Release は不要としたい)
	LnHandle mainUIView;
	LnEngine_MainUIView(&mainUIView);
	auto c1 = LnObject_GetReferenceCount(mainUIView);
	LnEngine_MainUIView(&mainUIView);
	auto c2 = LnObject_GetReferenceCount(mainUIView);
	assert(c1 == c2);



	LnHandle texture1;
	LnTexture2D_CreateFromFile(u"D:/tmp/lnpoi.png", LN_TEXTURE_FORMAT_RGBA8, &texture1);

	LnHandle sprite1;
	LnSprite_Create(texture1, 2, 2, &sprite1);

	LnBool endRequested;
	while (true)
	{
		LnEngine_Update(&endRequested);
		if (!endRequested) break;
	}

	LnEngine_Finalize();

	return 0;
}
