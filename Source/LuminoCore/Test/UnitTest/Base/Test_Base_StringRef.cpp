#define LN_INTERNAL_ACCESS public
#include <TestConfig.h>
#include <Lumino/Base/SortedArray.h>


class Test_Base_StringRef : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

static int Func1(const StringRef& str) { return str.getLength(); }

//------------------------------------------------------------------------------
TEST_F(Test_Base_StringRef, Constructor)
{
	// <Test> 文字列リテラルと String でオーバーロードできること。
	{
		int c = 0;
		c += Func1(_T("abc"));
		c += Func1(String(_T("d")));
		ASSERT_EQ(4, c);
	}

	// <Test> メモリ確保を伴わずに配列の検索キーにできること。
	{
		SortedArray<String, int> d;
		d.add(_T("a"), 1);
		d.add(_T("b"), 2);
		auto* v = d.find(StringRef(_T("b")));
		ASSERT_EQ(2, *v);
	}
}
