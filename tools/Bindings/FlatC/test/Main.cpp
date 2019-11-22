
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

	// Create した直後の参照カウントは 1.
	// Create した Object は明示的に Release する必要がある。
	{
		LnHandle texture1;
		LnTexture2D_Create(32, 32, &texture1);
		auto c1 = LnObject_GetReferenceCount(texture1);
		assert(c1 == 1);
		LnObject_Release(texture1);
	}

	// Get 系の関数を複数回呼び出しても、参照カウントは変わらないこと。
	// (Create した場合は明示的に Release する必要があるが、Get の場合 Release は不要としたい)
	{
		LnHandle mainUIView;
		LnEngine_MainUIView(&mainUIView);
		auto c1 = LnObject_GetReferenceCount(mainUIView);
		LnEngine_MainUIView(&mainUIView);
		auto c2 = LnObject_GetReferenceCount(mainUIView);
		assert(c1 == c2);
	}

	// Get 系は Retain したら、同じ回数だけ Release しなければならない。
	{
		LnHandle mainUIView;
		LnEngine_MainUIView(&mainUIView);
		auto c1 = LnObject_GetReferenceCount(mainUIView);
		LnObject_Retain(mainUIView);
		LnObject_Retain(mainUIView);
		LnObject_Release(mainUIView);
		LnObject_Release(mainUIView);
		auto c2 = LnObject_GetReferenceCount(mainUIView);
		assert(c1 == c2);
	}

	// Create 系の Retain 確認。
	{
		LnHandle texture;
		LnTexture2D_Create(32, 32, &texture);
		LnObject_Retain(texture);
		LnObject_Retain(texture);
		LnObject_Release(texture);
		LnObject_Release(texture);
		LnObject_Release(texture);
	}

	LnHandle texture1;
	LnTexture2D_CreateFromFile(u"D:/tmp/lnpoi.png", LN_TEXTURE_FORMAT_RGBA8, &texture1);

	LnHandle sprite1;
	LnSprite_Create(texture1, 2, 2, &sprite1);

	// Collection のテスト.
	{
		LnHandle components1;
		LnWorldObject_GetComponents(sprite1, &components1);

		int length;
		LnComponentList_GetLength(components1, &length);
		assert(length == 1);

		LnHandle spriteComponent1;
		LnComponentList_GetItem(components1, 0, &spriteComponent1);
		assert(spriteComponent1 != LN_NULL_HANDLE);
	}



	LnBool endRequested;
	while (true)
	{
		LnEngine_Update(&endRequested);
		if (!endRequested) break;
	}

	// 明示的に release しなくても、Create したものは LnEngine_Finalize() で解放される。
	//LnObject_Release(texture1);
	//LnObject_Release(sprite1);
	LnEngine_Finalize();

	return 0;
}
