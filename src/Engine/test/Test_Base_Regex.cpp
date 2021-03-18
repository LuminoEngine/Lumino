#include "Common.hpp"

class Test_Base_Regex : public ::testing::Test {};

TEST_F(Test_Base_Regex, Integrate)
{
	// * [ ] full match
	{
		ASSERT_EQ(true, Regex::match(u"123456", u"[0-9]+"));
		ASSERT_EQ(false, Regex::match(u"123456xxx", u"[0-9]+"));
		ASSERT_EQ(false, Regex::match(u"xxx123456", u"[0-9]+"));
		ASSERT_EQ(false, Regex::match(u"xxx123456xxxx", u"[0-9]+"));
	}
	// * [ ] partial match
	{
		ASSERT_EQ(true, Regex::search(u"123456", u"[0-9]+"));
		ASSERT_EQ(true, Regex::search(u"123456xxx", u"[0-9]+"));
		ASSERT_EQ(true, Regex::search(u"xxx123456", u"[0-9]+"));
		ASSERT_EQ(true, Regex::search(u"xxx123456xxxx", u"[0-9]+"));
	}
	// * [ ] group
	{
		MatchResult mr;
		//ASSERT_EQ(true, Regex::match(_T("123 456 789"), _T("(\\d+) (\\d+) (\\d+)"), &mr));	// std::regex (ECMAScript)
		ASSERT_EQ(true, Regex::match(_T("123 456 789"), _T("(\\d+)\\s(\\d+)\\s(\\d+)"), &mr)); // PCRE
		ASSERT_EQ(0, mr.position());
		ASSERT_EQ(11, mr.length());
		ASSERT_EQ(4, mr.groupCount());
		ASSERT_EQ(_T("123 456 789"), mr.value());
		ASSERT_EQ(_T("123 456 789"), mr.groupValue(0));
		ASSERT_EQ(_T("123"), mr.groupValue(1));
		ASSERT_EQ(_T("456"), mr.groupValue(2));
		ASSERT_EQ(_T("789"), mr.groupValue(3));
	}
	// * [ ] Issues (. がサロゲートペアの片方にマッチしてしまう)
	{
		Char utf16Buf[] = {
			0x0041,			// 'A'
			0xD867, 0xDE3D, // '𩸽' (正常なサロゲートペア)
			0x0041,			// 'A'
			0x0000,
		};
		MatchResult mr;
		ASSERT_EQ(true, Regex::match(utf16Buf, _T("A.A"), &mr));
		ASSERT_EQ(false, Regex::match(utf16Buf, _T("A..A"), &mr));
	}
}

