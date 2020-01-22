#include "Common.hpp"
#include <LuminoEngine/Runtime/Runtime.hpp>
#include <LuminoEngine/Runtime/Lumino.FlatC.generated.h>

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

static int g_value = 0;
static LnHandle g_otherObject = LN_NULL_HANDLE;

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
