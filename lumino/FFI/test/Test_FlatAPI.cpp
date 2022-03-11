#include "Common.hpp"
#include <LuminoFFI/FFI.hpp>
#include <LuminoFFI/Lumino.FlatC.generated.h>
#include <LuminoEngine/Runtime/detail/BindingValidation.hpp>

#define LN_ZV_CHECK(f) \
{ \
    LNResult r = (f); \
	ASSERT_EQ(LN_OK, r); \
}

//==============================================================================
class Test_FlatAPI : public LuminoSceneTest
{
public:
	void SetUp() override {
	}
};

static volatile int g_value = 0;
static volatile LNHandle g_otherObject = LN_NULL_HANDLE;

TEST_F(Test_FlatAPI, ReferenceCount)
{
	LNHandle obj1;
	LN_ZV_CHECK(LNZVTestClass1_Create(&obj1));
	ASSERT_EQ(1, _LNObject_GetReferenceCount(obj1));
	LNObject_Release(obj1);
}

static LNResult LNZVTestDelegate1_Callback(LNHandle selfDelegate, int a)
{
	g_value = a;
	return LN_OK;
}

static LNResult LNZVTestDelegate2_Callback(LNHandle selfDelegate, int a, int b, int* outReturn)
{
	g_value++;
	*outReturn = a + b;
	return LN_OK;
}

static LNResult LNZVTestDelegate3_Callback(LNHandle selfDelegate, LNHandle otherObject)
{
	g_value = _LNObject_GetReferenceCount(otherObject);
	g_otherObject = otherObject;
	return LN_OK;
}

TEST_F(Test_FlatAPI, Delegate)
{
	LNHandle delegate1;
	LNHandle delegate2;
	LNHandle delegate3;
	LN_ZV_CHECK(LNZVTestDelegate1_Create(LNZVTestDelegate1_Callback, &delegate1));
	LN_ZV_CHECK(LNZVTestDelegate2_Create(LNZVTestDelegate2_Callback, &delegate2));
	LN_ZV_CHECK(LNZVTestDelegate3_Create(LNZVTestDelegate3_Callback, &delegate3));

	LNHandle obj1;
	LN_ZV_CHECK(LNZVTestClass1_Create(&obj1));
	LN_ZV_CHECK(LNZVTestClass1_SetTestDelegate1(obj1, delegate1));
	LN_ZV_CHECK(LNZVTestClass1_SetTestDelegate2(obj1, delegate2));
	LN_ZV_CHECK(LNZVTestClass1_SetTestDelegate3(obj1, delegate3));

	// recive int.
	g_value = 0;
	LN_ZV_CHECK(LNZVTestClass1_CallTestDelegate1(obj1, 5));
	ASSERT_EQ(5, g_value);

	// get return value.
	int ret = 0;
	g_value = 0;
	LN_ZV_CHECK(LNZVTestClass1_CallTestDelegate2(obj1, 1, 2, &ret));
	ASSERT_EQ(1, g_value);
	ASSERT_EQ(3, ret);

	// get internal created object. (for Promise)
	g_value = 0;
	g_otherObject = LN_NULL_HANDLE;
	LN_ZV_CHECK(LNZVTestClass1_CallTestDelegate3(obj1));
	ASSERT_NE(LN_NULL_HANDLE, g_otherObject);
	ASSERT_EQ(3, g_value);	// 作成元の stack の分と、Delegate に Ref<> で渡された分と、コールバックに強参照で渡された分
	ASSERT_EQ(1, _LNObject_GetReferenceCount(g_otherObject));	// コールバックに強参照で渡されたオブジェクトは自分で Release する必要がある。

	LNObject_Release(g_otherObject);
	LNObject_Release(obj1);
	LNObject_Release(delegate3);
	LNObject_Release(delegate2);
	LNObject_Release(delegate1);
}

TEST_F(Test_FlatAPI, Promise)
{
	auto t1 = TypeInfo::getTypeInfo<ZVTestPromise1>();
	auto t2 = TypeInfo::getTypeInfo<ZVTestPromise2>();
	ASSERT_NE(t1, t2);


	LNHandle delegate3;
	LN_ZV_CHECK(LNZVTestDelegate3_Create(LNZVTestDelegate3_Callback, &delegate3));

	// Create objects asynchronously.
	LNHandle promise1;
	LN_ZV_CHECK(LNZVTestClass1_LoadAsync(_TT("test"), &promise1));

	// Wait creation ending.
	g_otherObject = LN_NULL_HANDLE;
	LN_ZV_CHECK(LNZVTestPromise1_ThenWith(promise1, delegate3));
	while (g_otherObject == LN_NULL_HANDLE) TestEnv::updateFrame();
	LNHandle obj1 = g_otherObject;

	const Char* filePath;
	LN_ZV_CHECK(LNZVTestClass1_GetFilePath(obj1, &filePath));
	ASSERT_EQ(_TT("test"), String(filePath));

	LNObject_Release(obj1);
	LNObject_Release(promise1);
	LNObject_Release(delegate3);
}

static LNResult LNZVPromiseFailureDelegate_Callback(LNHandle selfDelegate)
{
	g_value = -1;
	return LN_OK;
}

TEST_F(Test_FlatAPI, Promise_Failure)
{
	LNHandle delegate3;
	LN_ZV_CHECK(LNZVTestDelegate3_Create(LNZVTestDelegate3_Callback, &delegate3));

	LNHandle delegateFailure;
	LN_ZV_CHECK(LNPromiseFailureDelegate_Create(LNZVPromiseFailureDelegate_Callback, &delegateFailure));

	// Create objects asynchronously.
	LNHandle promise1;
	LN_ZV_CHECK(LNZVTestClass1_LoadAsync(nullptr, &promise1));
	LN_ZV_CHECK(LNZVTestPromise1_ThenWith(promise1, delegate3));
	LN_ZV_CHECK(LNZVTestPromise1_CatchWith(promise1, delegateFailure));

	// Wait creation ending.
	g_value = 0;
	while (g_value != -1) TestEnv::updateFrame();

	LNObject_Release(promise1);
	LNObject_Release(delegateFailure);
	LNObject_Release(delegate3);
}

static LNResult LNZVTestEventHandler1_Callback(LNHandle selfDelegate)
{
	g_value = 555;
	return LN_OK;
}

static LNResult LNZVTestEventHandler2_Callback(LNHandle selfDelegate, LNHandle e)
{
	int v;
	LNZVTestEventArgs1_GetValue(e, &v);
	g_value = v;
	return LN_OK;
}

TEST_F(Test_FlatAPI, Event)
{
	// void(void)
	{
		LNHandle delegate4;
		LN_ZV_CHECK(LNZVTestEventHandler1_Create(LNZVTestEventHandler1_Callback, &delegate4));

		LNHandle obj1;
		LN_ZV_CHECK(LNZVTestClass1_Create(&obj1));

		LNHandle connection;
		LN_ZV_CHECK(LNZVTestClass1_ConnectOnEvent1(obj1, delegate4, &connection));

		g_value = 0;
		LN_ZV_CHECK(LNZVTestClass1_RaiseEvent1(obj1));
		ASSERT_EQ(555, g_value);

		LNObject_Release(connection);
		LNObject_Release(obj1);
		LNObject_Release(delegate4);
	}

	// Omit connection
	{
		LNHandle delegate4;
		LN_ZV_CHECK(LNZVTestEventHandler1_Create(LNZVTestEventHandler1_Callback, &delegate4));

		LNHandle obj1;
		LN_ZV_CHECK(LNZVTestClass1_Create(&obj1));

		LN_ZV_CHECK(LNZVTestClass1_ConnectOnEvent1(obj1, delegate4, nullptr));

		g_value = 0;
		LN_ZV_CHECK(LNZVTestClass1_RaiseEvent1(obj1));
		ASSERT_EQ(555, g_value);

		LNObject_Release(obj1);
		LNObject_Release(delegate4);
	}

	// void(ZVTestEventHandler2*)
	{
		LNHandle delegate1;
		LN_ZV_CHECK(LNZVTestEventHandler2_Create(LNZVTestEventHandler2_Callback, &delegate1));

		LNHandle obj1;
		LN_ZV_CHECK(LNZVTestClass1_Create(&obj1));

		LN_ZV_CHECK(LNZVTestClass1_ConnectOnEvent2(obj1, delegate1, nullptr));

		g_value = 0;
		LN_ZV_CHECK(LNZVTestClass1_RaiseEvent2(obj1));
		ASSERT_EQ(1024, g_value);

		LNObject_Release(obj1);
		LNObject_Release(delegate1);
	}

}


//------------------------------------------------------------------------------
static int g_count = 0;

static LNResult Sprite_OnUpdate(LNHandle delegate, LNHandle self, float elapsedSeconds)
{
	g_count++;
	return LN_OK;
}

TEST_F(Test_FlatAPI, VirtualProtoType)
{
	LNHandle world;
	LNEngine_GetWorld(&world);

	LNHandle texture;
	LNTexture2D_Load(LN_ASSETFILE("Sprite1.png").c_str(), &texture);

	LNHandle delegate;
	LNSpriteUpdateHandler_Create(Sprite_OnUpdate, &delegate);

	LNHandle sprite;
	LNSprite_CreateWithTexture(texture, &sprite);
	LNWorldObject_SetScaleS(sprite, 5);
	LNWorld_Add(world, sprite);
	LNSprite_SetPrototype_OnUpdate(sprite, delegate);

	g_count = 0;

	Engine::mainLight()->setEnabled(false);
	TestEnv::updateFrame();
	ASSERT_SCREEN(LN_ASSETFILE("FlatAPI/Expects/Basic-1.png").c_str());
	LN_TEST_CLEAN_SCENE;

	LNObject_Release(sprite);
	LNObject_Release(delegate);
	LNObject_Release(texture);

	ASSERT_EQ(g_count, 1);
}

//------------------------------------------------------------------------------
TEST_F(Test_FlatAPI, GetStringA)
{
	LNHandle obj;
	LNInterpreter_Create(&obj);
	LNInterpreter_SetWaitModeA(obj, "ABC");

	const char* str;
	LNInterpreter_GetWaitModeA(obj, &str);

	ASSERT_STREQ("ABC", str);

	LNObject_Release(obj);
}
