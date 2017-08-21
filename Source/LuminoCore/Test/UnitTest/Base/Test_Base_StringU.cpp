#include <TestConfig.h>
#include <unordered_map>
#include <Lumino/Base/StringU.h>

class Test_Base_UString : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

#define NSSO_STR	u"abcdefghij1234567890"

//------------------------------------------------------------------------------
TEST_F(Test_Base_UString, Constructor)
{
	// <Test> SSO
	{
		{
			UString s1(u"abc");
			UString s2(s1, 1);
			UString s3(s1, 1, 1);
			ASSERT_EQ(u"bc", s2);
			ASSERT_EQ(u"b", s3);
			UString s4(UString(u""));
			ASSERT_EQ(u"", s4);
		}
		{
			UString s1;
			UString s2(u"abc");
			UString s3(u"abc", 1);
			UString s4(u"abc", 1, 1);
			UString s5(u"");
			ASSERT_EQ(true, s1.isEmpty());
			ASSERT_EQ(u"abc", s2);
			ASSERT_EQ(u"bc", s3);
			ASSERT_EQ(u"b", s4);
			ASSERT_EQ(u"", s5);
		}
		{
			UString s1(3, u'a');
			UString s2(0, u'a');
			ASSERT_EQ(u"aaa", s1);
			ASSERT_EQ(u"", s2);
		}
	}
	// <Test> NonSSO
	{
		{
			UString s1(NSSO_STR);
			UString s2(s1, 1);
			UString s3(s1, 1, 1);
			ASSERT_EQ(u"bc", s2);
			ASSERT_EQ(u"b", s3);
		}
		{
			UString s1;
			UString s2(NSSO_STR);
			UString s3(NSSO_STR, 1);
			UString s4(NSSO_STR, 1, 1);
			UString s5(u"");
			ASSERT_EQ(true, s1.isEmpty());
			ASSERT_EQ(u"abc", s2);
			ASSERT_EQ(u"bc", s3);
			ASSERT_EQ(u"b", s4);
			ASSERT_EQ(u"", s5);
		}
		{
			UString s1(20, u'a');
			ASSERT_EQ(u"aaaaaaaaaaaaaaaaaaaa", s1);
		}
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_UString, CopyFramework)
{
	// <Test> copy (SSO)
	{
		UString s1;
		UString s2(s1);
		UString s3(u"abc");
		UString s4(s3);
		ASSERT_EQ(true, s1.isEmpty());
		ASSERT_EQ(true, s2.isEmpty());
		ASSERT_EQ(u"abc", s3);
		ASSERT_EQ(u"abc", s4);

		s1 = s4;
		ASSERT_EQ(u"abc", s1);
		ASSERT_EQ(u"abc", s4);
	}
	// <Test> copy (Non SSO)
	{
		UString s1;
		UString s2(s1);
		UString s3(NSSO_STR);
		UString s4(s3);
		ASSERT_EQ(true, s1.isEmpty());
		ASSERT_EQ(true, s2.isEmpty());
		ASSERT_EQ(NSSO_STR, s3);
		ASSERT_EQ(NSSO_STR, s4);

		s1 = s4;
		ASSERT_EQ(NSSO_STR, s1);
		ASSERT_EQ(NSSO_STR, s4);
	}
	// <Test> move (SSO)
	{
		UString s1;
		UString s2(std::move(s1));
		UString s3(u"abc");
		UString s4(std::move(s3));
		ASSERT_EQ(true, s1.isEmpty());
		ASSERT_EQ(true, s2.isEmpty());
		ASSERT_EQ(true, s3.isEmpty());
		ASSERT_EQ(u"abc", s4);

		s1 = std::move(s4);
		ASSERT_EQ(u"abc", s1);
		ASSERT_EQ(true, s4.isEmpty());
	}
	// <Test> move (Non SSO)
	{
		UString s1;
		UString s2(std::move(s1));
		UString s3(NSSO_STR);
		UString s4(std::move(s3));
		ASSERT_EQ(true, s1.isEmpty());
		ASSERT_EQ(true, s2.isEmpty());
		ASSERT_EQ(true, s3.isEmpty());
		ASSERT_EQ(NSSO_STR, s4);

		s1 = std::move(s4);
		ASSERT_EQ(NSSO_STR, s1);
		ASSERT_EQ(true, s4.isEmpty());
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_UString, Operators)
{
	// <Test> =
	{
		UString s1, s2, s3(u"abc");
		s1 = s2;
		s2 = s3;
		s3 = u'a';
		ASSERT_EQ(true, s1.isEmpty());
		ASSERT_EQ(u"abc", s2);
		ASSERT_EQ(u"a", s3); ASSERT_EQ(1, s3.getLength());
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_UString, assignFromCStr)
{
	// <Test> コンストラクタへ const char*, char 指定
	{
		UString s1("abc");
		UString s2(2, 'a');
		ASSERT_EQ(3, s1.getLength());
		ASSERT_EQ(u"abc", s1);
		ASSERT_EQ(u"aa", s2);
	}
	// <Test> 処理系関数へ const char*, char 指定
	{
		UString s1("abc");
		ASSERT_EQ(1, s1.indexOf("b"));
		ASSERT_EQ(1, s1.indexOf('b'));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_UString, clear)
{
	// <Test> (SSO)
	{
		UString s1, s2(u"abc");
		s1.clear();
		s2.clear();
		ASSERT_EQ(true, s1.isEmpty());
		ASSERT_EQ(true, s2.isEmpty());
	}
	// <Test> (NonSSO)
	{
		UString s1, s2(NSSO_STR);
		s1.clear();
		s2.clear();
		ASSERT_EQ(true, s1.isEmpty());
		ASSERT_EQ(true, s2.isEmpty());
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_UString, indexOf)
{
	UString s1(u"abcdef");
	ASSERT_EQ(1, s1.indexOf(u"bcd"));
	ASSERT_EQ(-1, s1.indexOf(u"a", 1));
	ASSERT_EQ(0, s1.indexOf(u"A", 0, CaseSensitivity::CaseInsensitive));
	ASSERT_EQ(4, s1.indexOf(u'e'));
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_UString, Issue)
{
}



