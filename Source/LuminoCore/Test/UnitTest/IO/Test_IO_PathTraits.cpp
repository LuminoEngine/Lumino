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
	Char result[LN_MAX_PATH];
	// <Test> 拡張子を取り除いたファイル名を取得できること。
	{
		PathTraits::getFileNameWithoutExtension(_LT("a.b"), result);
		ASSERT_EQ(String(_LT("a")), result);
	}
	// <Test> 拡張子が無ければファイル名だけ取得できること。
	{
		PathTraits::getFileNameWithoutExtension(_LT("a"), result);
		ASSERT_EQ(String(_LT("a")), result);
		PathTraits::getFileNameWithoutExtension(_LT("a."), result);
		ASSERT_EQ(String(_LT("a")), result);
	}
	// <Test> .で始まる場合は空文字列が返ること。
	{
		PathTraits::getFileNameWithoutExtension(_LT(".b"), result);
		ASSERT_EQ(String(_LT("")), result);
		PathTraits::getFileNameWithoutExtension(_LT("."), result);
		ASSERT_EQ(String(_LT("")), result);
	}
	// <Test> ディレクトリパスは除外されること。
	{
		PathTraits::getFileNameWithoutExtension(_LT("d/a.b"), result);
		ASSERT_EQ(String(_LT("a")), result);
		PathTraits::getFileNameWithoutExtension(_LT("d/a"), result);
		ASSERT_EQ(String(_LT("a")), result);
		PathTraits::getFileNameWithoutExtension(_LT("/"), result);
		ASSERT_EQ(String(_LT("")), result);
		PathTraits::getFileNameWithoutExtension(_LT("/a"), result);
		ASSERT_EQ(String(_LT("a")), result);
	}
	// <Test> NULL は空文字列として出力されること。
	{
		PathTraits::getFileNameWithoutExtension((Char*)NULL, result);
		ASSERT_EQ(String(_LT("")), result);
	}
	// <Test> 空文字列は空文字列として出力されること。
	{
		PathTraits::getFileNameWithoutExtension(_LT(""), result);
		ASSERT_EQ(String(_LT("")), result);
	}
}
