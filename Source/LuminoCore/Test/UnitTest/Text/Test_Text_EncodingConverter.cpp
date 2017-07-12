#include <TestConfig.h>
#include <Lumino/Text/EncodingConverter.h>

class Test_Text_EncodingConverter : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//---------------------------------------------------------------------
TEST_F(Test_Text_EncodingConverter, AsciiCode)
{
	EncodingConversionOptions options;
	options.NullTerminated = true;
	EncodingConverter convAsciiToUtf16;
	convAsciiToUtf16.getSourceEncoding(Encoding::getSystemMultiByteEncoding());
	convAsciiToUtf16.setDestinationEncoding(Encoding::getUTF16Encoding());
	convAsciiToUtf16.setConversionOptions(options);
	ByteBuffer buf = convAsciiToUtf16.convert("ab", 2);
	const UTF16* utf16 = (const UTF16*)buf.getConstData();
	ASSERT_EQ('a', utf16[0]);
	ASSERT_EQ('b', utf16[1]);
	ASSERT_EQ(0x00, utf16[2]);
}
