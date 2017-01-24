#include <TestConfig.h>
#include <Lumino/IO/FileSystem.h>
#include <Lumino/Text/Encoding.h>

class Test_Text_DBCSEncoding : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//---------------------------------------------------------------------
TEST_F(Test_Text_DBCSEncoding, SJIS)
{
	EncodingConversionOptions options;
	options.NullTerminated = false;

	Encoding* sjisEnc = Encoding::GetEncoding(EncodingType::SJIS);
	ByteBuffer sjisBuf = FileSystem::ReadAllBytes(LN_LOCALFILE("TestData/ConvertTable_SJIS_test.txt"));
	ByteBuffer utf16Buf = FileSystem::ReadAllBytes(LN_LOCALFILE("TestData/ConvertTable_SJIS_test_UTF16.txt"));

	// MBCS → UTF16
	EncodingConversionResult result;
	ByteBuffer utf16Result = Encoding::Convert(sjisBuf.GetData(), sjisBuf.GetSize(), sjisEnc, Encoding::GetUTF16Encoding(), options, &result);

	int cmp = memcmp(utf16Buf.GetData(), utf16Result.GetData(), utf16Buf.GetSize());
	ASSERT_EQ(0, cmp);											// 内容一致
	ASSERT_TRUE(utf16Buf.GetSize() == utf16Result.GetSize());	// バッファサイズ一致

	// UTF16 → MBCS
	ByteBuffer sjisResult = Encoding::Convert(utf16Result.GetData(), utf16Result.GetSize(), Encoding::GetUTF16Encoding(), sjisEnc, options, &result);

	cmp = memcmp(sjisBuf.GetData(), sjisResult.GetData(), sjisBuf.GetSize());
	ASSERT_EQ(0, cmp);											// 内容一致
	ASSERT_TRUE(sjisBuf.GetSize() == sjisResult.GetSize());	// バッファサイズ一致
}
