#include <TestConfig.h>

class Test_Base_Exception : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

static int g_value = 0;
bool Test_NotifyVerificationHandler(const Exception& e)
{
	if (g_value == 0)
	{
		g_value++;
		return true;
	}
	else
	{
		g_value++;
		return false;
	}
}

//---------------------------------------------------------------------
TEST_F(Test_Base_Exception, Check)
{
	int err = 0;

#ifdef LN_DO_CHECK_THROW

	try
	{
		if (LN_CHECK_ARG(0)) err += 1;	// NG
		FAIL();
	}
	catch (ArgumentException& e)
	{
	}
	ASSERT_EQ(0, err);

	if (LN_CHECK_ARG(1)) err += 10;	// OK
	ASSERT_EQ(0, err);

#else
	if (LN_CHECK_ARG(0)) err += 1;	// NG
	if (LN_CHECK_ARG(1)) err += 10;	// OK
#endif
}

//---------------------------------------------------------------------
TEST_F(Test_Base_Exception, Assertion)
{
	ASSERT_THROW(
		LN_VERIFY(0, ArgumentException),
		ArgumentException);

	Assertion::SetNotifyVerificationHandler(Test_NotifyVerificationHandler);

	// ユーザーハンドラ・handled
	LN_VERIFY(0, ArgumentException);
	ASSERT_EQ(1, g_value);

	// ユーザーハンドラ・unhandled
	ASSERT_THROW(
		LN_VERIFY(0, ArgumentException),
		ArgumentException);
	ASSERT_EQ(2, g_value);

	Assertion::SetNotifyVerificationHandler(nullptr);

	ASSERT_THROW(
		LN_VERIFY(0, ArgumentException),
		ArgumentException);
}


//---------------------------------------------------------------------
TEST_F(Test_Base_Exception, Basic)
{
	// 
	{
		try
		{
			LN_THROW(0, ArgumentException);
		}
		catch (ArgumentException& e)
		{
			ASSERT_TRUE(_tcslen(e.GetMessage()) > 0);	// 何かメッセージが入っているはず
		}

		try
		{
			LN_THROW(0, ArgumentException, "test");
		}
		catch (ArgumentException& e)
		{
			ASSERT_TRUE(_tcsstr(e.GetMessage(), _T("test")) != NULL);
		}

		try
		{
			LN_THROW(0, ArgumentException, "param:%s", "p1");
		}
		catch (ArgumentException& e)
		{
			ASSERT_TRUE(_tcsstr(e.GetMessage(), _T("param:p1")) != NULL);
		}
	}


	// char 可変長 message
	try
	{
		LN_THROW(0, IOException, "test%d", 1);
	}
	catch (IOException& e)
	{
		ASSERT_TRUE(_tcsstr(e.GetMessage(), _T("test1")) != NULL);
	}
	// wchar_t 可変長 message
	try
	{
#ifdef LN_OS_WIN32
		LN_THROW(0, IOException, L"test%s", L"test");
#else	// 現在 Unix 系の vswprintf は動作保証していないので別の例外になる。
		try {
			LN_THROW(0, IOException, L"test%s", L"test");
		}
		catch (NotImplementedException& e) {
		}
#endif
	}
	catch (IOException& e)
	{
		ASSERT_TRUE(_tcsstr(e.GetMessage(), _T("testtest")) != NULL);
	}
}

//---------------------------------------------------------------------
//TEST_F(Test_Base_Exception, AssertionMacros)
//{
//	int x = 0;
//
//	try
//	{
//		if (LN_CHECKEQ_ARG(x != 0)) {
//			x = 1;
//		}
//
//		if (LN_CHECKEQ_ARG(x == 0)) {
//			x = 1;
//		}
//		else {
//			x = 2;
//		}
//	}
//	catch (ArgumentException& e)
//	{
//	}
//
//	ASSERT_EQ(0, x);
//}
