#include "Common.hpp"
#include <LuminoEngine/Runtime/Runtime.hpp>
#include <LuminoEngine/Runtime/Lumino.FlatC.generated.h>
#include "../src/Runtime/BindingValidation.hpp"

#define LN_ZV_CHECK(f) \
{ \
    LnResult r = (f); \
	ASSERT_EQ(LN_SUCCESS, r); \
}

//==============================================================================
class Test_Binding : public LuminoSceneTest
{
public:
};

static volatile int g_value = 0;
static volatile LnHandle g_otherObject = LN_NULL_HANDLE;

TEST_F(Test_Binding, ReferenceCount)
{
	LnHandle obj1;
	LN_ZV_CHECK(LnZVTestClass1_Create(&obj1));
	ASSERT_EQ(1, LnObject_GetReferenceCount(obj1));
	LnObject_Release(obj1);
}

static LnResult LnZVTestDelegate1_Callback(LnHandle selfDelegate, int a)
{
	g_value = a;
	return LN_SUCCESS;
}

static LnResult LnZVTestDelegate2_Callback(LnHandle selfDelegate, int a, int b, int* outReturn)
{
	g_value++;
	*outReturn = a + b;
	return LN_SUCCESS;
}

static LnResult LnZVTestDelegate3_Callback(LnHandle selfDelegate, LnHandle otherObject)
{
	g_value = LnObject_GetReferenceCount(otherObject);
	g_otherObject = otherObject;
	return LN_SUCCESS;
}

TEST_F(Test_Binding, Delegate)
{
	LnHandle delegate1;
	LnHandle delegate2;
	LnHandle delegate3;
	LN_ZV_CHECK(LnZVTestDelegate1_Create(LnZVTestDelegate1_Callback, &delegate1));
	LN_ZV_CHECK(LnZVTestDelegate2_Create(LnZVTestDelegate2_Callback, &delegate2));
	LN_ZV_CHECK(LnZVTestDelegate3_Create(LnZVTestDelegate3_Callback, &delegate3));

	LnHandle obj1;
	LN_ZV_CHECK(LnZVTestClass1_Create(&obj1));
	LN_ZV_CHECK(LnZVTestClass1_SetTestDelegate1(obj1, delegate1));
	LN_ZV_CHECK(LnZVTestClass1_SetTestDelegate2(obj1, delegate2));
	LN_ZV_CHECK(LnZVTestClass1_SetTestDelegate3(obj1, delegate3));

	// recive int.
	g_value = 0;
	LN_ZV_CHECK(LnZVTestClass1_CallTestDelegate1(obj1, 5));
	ASSERT_EQ(5, g_value);

	// get return value.
	int ret = 0;
	g_value = 0;
	LN_ZV_CHECK(LnZVTestClass1_CallTestDelegate2(obj1, 1, 2, &ret));
	ASSERT_EQ(1, g_value);
	ASSERT_EQ(3, ret);

	// get internal created object. (for Promise)
	g_value = 0;
	g_otherObject = LN_NULL_HANDLE;
	LN_ZV_CHECK(LnZVTestClass1_CallTestDelegate3(obj1));
	ASSERT_NE(LN_NULL_HANDLE, g_otherObject);
	ASSERT_EQ(3, g_value);	// 作成元の stack の分と、Delegate に Ref<> で渡された分と、コールバックに強参照で渡された分
	ASSERT_EQ(1, LnObject_GetReferenceCount(g_otherObject));	// コールバックに強参照で渡されたオブジェクトは自分で Release する必要がある。

	LnObject_Release(g_otherObject);
	LnObject_Release(obj1);
	LnObject_Release(delegate3);
	LnObject_Release(delegate2);
	LnObject_Release(delegate1);
}

TEST_F(Test_Binding, Promise)
{
	auto t1 = TypeInfo::getTypeInfo<ZVTestPromise1>();
	auto t2 = TypeInfo::getTypeInfo<ZVTestPromise2>();
	ASSERT_NE(t1, t2);


	LnHandle delegate3;
	LN_ZV_CHECK(LnZVTestDelegate3_Create(LnZVTestDelegate3_Callback, &delegate3));

	// Create objects asynchronously.
	LnHandle promise1;
	LN_ZV_CHECK(LnZVTestClass1_LoadAsync(u"test", &promise1));

	// Wait creation ending.
	g_otherObject = LN_NULL_HANDLE;
	LN_ZV_CHECK(LnZVTestPromise1_ThenWith(promise1, delegate3));
	while (g_otherObject == LN_NULL_HANDLE) TestEnv::updateFrame();
	LnHandle obj1 = g_otherObject;

	const Char* filePath;
	LN_ZV_CHECK(LnZVTestClass1_GetFilePath(obj1, &filePath));
	ASSERT_EQ(u"test", String(filePath));

	LnObject_Release(obj1);
	LnObject_Release(promise1);
	LnObject_Release(delegate3);
}

static LnResult LnZVPromiseFailureDelegate_Callback(LnHandle selfDelegate)
{
	g_value = -1;
	return LN_SUCCESS;
}

TEST_F(Test_Binding, Promise_Failure)
{
	LnHandle delegate3;
	LN_ZV_CHECK(LnZVTestDelegate3_Create(LnZVTestDelegate3_Callback, &delegate3));

	LnHandle delegateFailure;
	LN_ZV_CHECK(LnPromiseFailureDelegate_Create(LnZVPromiseFailureDelegate_Callback, &delegateFailure));

	// Create objects asynchronously.
	LnHandle promise1;
	LN_ZV_CHECK(LnZVTestClass1_LoadAsync(nullptr, &promise1));
	LN_ZV_CHECK(LnZVTestPromise1_ThenWith(promise1, delegate3));
	LN_ZV_CHECK(LnZVTestPromise1_CatchWith(promise1, delegateFailure));

	// Wait creation ending.
	g_value = 0;
	while (g_value != -1) TestEnv::updateFrame();

	LnObject_Release(promise1);
	LnObject_Release(delegateFailure);
	LnObject_Release(delegate3);
}

static LnResult LnZVTestEventHandler1_Callback(LnHandle selfDelegate)
{
	g_value = 555;
	return LN_SUCCESS;
}

static LnResult LnZVTestEventHandler2_Callback(LnHandle selfDelegate, LnHandle e)
{
	int v;
	LnZVTestEventArgs1_GetValue(e, &v);
	g_value = v;
	return LN_SUCCESS;
}

TEST_F(Test_Binding, Event)
{
	// void(void)
	{
		LnHandle delegate4;
		LN_ZV_CHECK(LnZVTestEventHandler1_Create(LnZVTestEventHandler1_Callback, &delegate4));

		LnHandle obj1;
		LN_ZV_CHECK(LnZVTestClass1_Create(&obj1));

		LnHandle connection;
		LN_ZV_CHECK(LnZVTestClass1_ConnectOnEvent1(obj1, delegate4, &connection));

		g_value = 0;
		LN_ZV_CHECK(LnZVTestClass1_RaiseEvent1(obj1));
		ASSERT_EQ(555, g_value);

		LnObject_Release(connection);
		LnObject_Release(obj1);
		LnObject_Release(delegate4);
	}

	// Omit connection
	{
		LnHandle delegate4;
		LN_ZV_CHECK(LnZVTestEventHandler1_Create(LnZVTestEventHandler1_Callback, &delegate4));

		LnHandle obj1;
		LN_ZV_CHECK(LnZVTestClass1_Create(&obj1));

		LN_ZV_CHECK(LnZVTestClass1_ConnectOnEvent1(obj1, delegate4, nullptr));

		g_value = 0;
		LN_ZV_CHECK(LnZVTestClass1_RaiseEvent1(obj1));
		ASSERT_EQ(555, g_value);

		LnObject_Release(obj1);
		LnObject_Release(delegate4);
	}

	// void(ZVTestEventHandler2*)
	{
		LnHandle delegate1;
		LN_ZV_CHECK(LnZVTestEventHandler2_Create(LnZVTestEventHandler2_Callback, &delegate1));

		LnHandle obj1;
		LN_ZV_CHECK(LnZVTestClass1_Create(&obj1));

		LN_ZV_CHECK(LnZVTestClass1_ConnectOnEvent2(obj1, delegate1, nullptr));

		g_value = 0;
		LN_ZV_CHECK(LnZVTestClass1_RaiseEvent2(obj1));
		ASSERT_EQ(1024, g_value);

		LnObject_Release(obj1);
		LnObject_Release(delegate1);
	}

}
