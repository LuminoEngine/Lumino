#include <TestConfig.h>
#include <Lumino/Base/Regex.h>

class Test_Base_Regex : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Base_Regex, Integrate)
{
	// <Test> 部分一致の検索
	{
		MatchResult mr;
		ASSERT_EQ(true, Regex::Search(_T("xxx123456yyy"), _T("[0-9]+"), &mr));
	}
	// <Test> 完全一致の確認
	// <Test> グループ
	{
		MatchResult mr;
		ASSERT_EQ(true, Regex::Match(_T("123 456 789"), _T("(\\d+) (\\d+) (\\d+)"), &mr));
		ASSERT_EQ(_T("123 456 789"), mr.GetValue());
		ASSERT_EQ(4, mr.GetGroupCount());
		ASSERT_EQ(_T("123 456 789"), mr.GetGroup(0));
		ASSERT_EQ(_T("123"), mr.GetGroup(1));
		ASSERT_EQ(_T("456"), mr.GetGroup(2));
		ASSERT_EQ(_T("789"), mr.GetGroup(3));
	}

	//std::regex re("[0-9]+");
	//std::match_results<const char *> results2;

	//if (!std::regex_search("xxx123456yyy", results2, re, std::regex_constants::match_default)) {
	//	return;
	//}
	//auto uu = results2[0];
	//auto nn = uu.first;

	//std::match_results<const char *> results = results2;

	//std::cout << "prefix: " << results.prefix().str() << std::endl;
	//std::cout << "suffix: " << results.suffix().str() << std::endl;
	//std::cout << "posision: " << results.position() << std::endl;
	//std::cout << "str: " << results.str() << std::endl;

	//std::match_results<const char *>::const_iterator it = results.begin();
	//while (it != results.end()) {
	//	if (it->matched) {
	//		std::cout << "sub_match: " << it->str() << std::endl;
	//	}
	//	it++;
	//}
}

