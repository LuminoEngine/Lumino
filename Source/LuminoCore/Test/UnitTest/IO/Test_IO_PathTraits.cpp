#include <TestConfig.h>

class Test_IO_PathTraits : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_IO_PathTraits, getFileNameWithoutExtension)
{
	TCHAR result[LN_MAX_PATH];
	// <Test> 拡張子を取り除いたファイル名を取得できること。
	{
		PathTraits::getFileNameWithoutExtension(_T("a.b"), result);
		ASSERT_STREQ(_T("a"), result);
	}
	// <Test> 拡張子が無ければファイル名だけ取得できること。
	{
		PathTraits::getFileNameWithoutExtension(_T("a"), result);
		ASSERT_STREQ(_T("a"), result);
		PathTraits::getFileNameWithoutExtension(_T("a."), result);
		ASSERT_STREQ(_T("a"), result);
	}
	// <Test> .で始まる場合は空文字列が返ること。
	{
		PathTraits::getFileNameWithoutExtension(_T(".b"), result);
		ASSERT_STREQ(_T(""), result);
		PathTraits::getFileNameWithoutExtension(_T("."), result);
		ASSERT_STREQ(_T(""), result);
	}
	// <Test> ディレクトリパスは除外されること。
	{
		PathTraits::getFileNameWithoutExtension(_T("d/a.b"), result);
		ASSERT_STREQ(_T("a"), result);
		PathTraits::getFileNameWithoutExtension(_T("d/a"), result);
		ASSERT_STREQ(_T("a"), result);
		PathTraits::getFileNameWithoutExtension(_T("/"), result);
		ASSERT_STREQ(_T(""), result);
		PathTraits::getFileNameWithoutExtension(_T("/a"), result);
		ASSERT_STREQ(_T("a"), result);
	}
	// <Test> NULL は空文字列として出力されること。
	{
		PathTraits::getFileNameWithoutExtension((TCHAR*)NULL, result);
		ASSERT_STREQ(_T(""), result);
	}
	// <Test> 空文字列は空文字列として出力されること。
	{
		PathTraits::getFileNameWithoutExtension(_T(""), result);
		ASSERT_STREQ(_T(""), result);
	}
}
