
#include <stdio.h>
#include <Lumino.FlatC.generated.h>

#ifdef _WIN32
#include <crtdbg.h>
#endif

class MyObject
{
public:
	LnHandle handle;
	//int managedIndex = -1;

private:
};

MyObject* g_MyObjectList[10];


// 自動生成
class MySprite : public MyObject
{
public:


	virtual void onUpdate(float elapsedSeconds)
	{
		printf("base s\n");
		//LnSprite_CallBase_OnUpdate(handle, elapsedSeconds);
		printf("base e\n");
	}

	// internal
	static void ManagedCaller_onUpdate(LnHandle self, float elapsedSeconds)
	{
		auto id = LnRuntime_GetManagedObjectId(self);
		static_cast<MySprite*>(g_MyObjectList[id])->onUpdate(elapsedSeconds);
	}

private:

};

// ユーザーが拡張
class MySpriteEx : public MySprite
{
public:
	virtual void onUpdate(float elapsedSeconds) override
	{
		printf("sub s\n");
		MySprite::onUpdate(elapsedSeconds);
		printf("sub e\n");
	}




private:

};




int main()
{
#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	LnEngine_Initialize();


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
#if 0
	LnEngine_Initialize();


	LnHandle texture1;
	LnTexture2D_CreateFromFile(u"C:/Proj/LN/Lumino/src/LuminoEngine/test/Assets/Graphics/Numbers1.png", LN_TEXTURE_FORMAT_RGBA8, &texture1);

	LnHandle sprite1;
	LnSprite_Create(texture1, 2, 2, &sprite1);

	// register type
	{
		LnSprite_SetCaller_OnUpdate(MySprite::ManagedCaller_onUpdate);
	}

	auto mySprite1 = new MySpriteEx();
	mySprite1->handle = sprite1;
	//mySprite1->managedIndex = 0;
	g_MyObjectList[0] = mySprite1;
	LnRuntime_SetManagedObjectId(sprite1, 0);


	LnBool endRequested;
	while (true)
	{
		LnEngine_Update(&endRequested);
		if (!endRequested) break;

	}

	LnObject_Release(texture1);

	LnEngine_Finalize();
#endif

	return 0;
}
