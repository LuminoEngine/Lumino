#include "Common.hpp"
#include <LuminoCore/Text/EncodingConverter.hpp>

class Test_Text_EncodingConverter : public ::testing::Test {};

TEST_F(Test_Text_EncodingConverter, AsciiCode)
{
	EncodingConversionOptions options;
	options.NullTerminated = true;
	EncodingConverter convAsciiToUtf16;
	convAsciiToUtf16.getSourceEncoding(TextEncoding::systemMultiByteEncoding());
	convAsciiToUtf16.setDestinationEncoding(TextEncoding::utf16Encoding());
	convAsciiToUtf16.setConversionOptions(options);
	const ByteBuffer& buf = convAsciiToUtf16.convert("ab", 2);
	const UTF16* utf16 = (const UTF16*)buf.data();
	ASSERT_EQ('a', utf16[0]);
	ASSERT_EQ('b', utf16[1]);
	ASSERT_EQ(0x00, utf16[2]);
}
