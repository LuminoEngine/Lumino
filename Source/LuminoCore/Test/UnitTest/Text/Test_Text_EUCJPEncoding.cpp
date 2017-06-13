#include <TestConfig.h>
#include <Lumino/Text/Encoding.h>

class IntegrateTest_Text_EUCJPEncoding : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//---------------------------------------------------------------------
TEST_F(IntegrateTest_Text_EUCJPEncoding, ConvertEUCJPToUTF16)
{
	size_t bytesUsed, charsUsed;
	std::unique_ptr<Decoder> decoder(Encoding::getEncoding(EncodingType::EUCJP)->createDecoder());

	// <Test> ASCII と制御文字
	{
		char asciiBuf[] = "a\nb";
		UTF16 utf16Buf[3] = { 0, 0, 0 };

		decoder->convertToUTF16((byte_t*)asciiBuf, 3, utf16Buf, 3, &bytesUsed, &charsUsed);
		ASSERT_EQ(3, bytesUsed);
		ASSERT_EQ(3, charsUsed);
		ASSERT_EQ('a', utf16Buf[0]);
		ASSERT_EQ('\n', utf16Buf[1]);
		ASSERT_EQ('b', utf16Buf[2]);
	}
	// <Test> 2Byte文字
	{
		byte_t eucBuf[] = { 0xA4, 0xA2 };	// "あ"
		UTF16 utf16Buf[3] = { 0, 0, 0 };

		decoder->convertToUTF16(eucBuf, 2, utf16Buf, 1, &bytesUsed, &charsUsed);
		ASSERT_EQ(2, bytesUsed);
		ASSERT_EQ(1, charsUsed);
		ASSERT_EQ(0x3042, utf16Buf[0]);
	}
	// <Test> 半角カタカナ
	{
		byte_t eucBuf[] = { 0x8E, 0xB1 };	// "ｱ"
		UTF16 utf16Buf[1];

		decoder->convertToUTF16(eucBuf, 2, utf16Buf, 1, &bytesUsed, &charsUsed);
		ASSERT_EQ(2, bytesUsed);
		ASSERT_EQ(1, charsUsed);
		ASSERT_EQ(0xFF71, utf16Buf[0]);
	}
	// <Test> 3Byte文字
	{
		byte_t eucBuf[] = { 0x8F, 0xB0, 0xA1 };	// "丂"
		UTF16 utf16Buf[1];

		decoder->convertToUTF16(eucBuf, 3, utf16Buf, 1, &bytesUsed, &charsUsed);
		ASSERT_EQ(3, bytesUsed);
		ASSERT_EQ(1, charsUsed);
		ASSERT_EQ(0x4E02, utf16Buf[0]);
	}
}

//---------------------------------------------------------------------
TEST_F(IntegrateTest_Text_EUCJPEncoding, ConvertUTF16ToEUFJP)
{
	size_t bytesUsed, charsUsed;
	std::unique_ptr<Encoder> encoder(Encoding::getEncoding(EncodingType::EUCJP)->createEncoder());

	// <Test> ASCII と制御文字
	{
		UTF16 utf16Buf[3] = { 'a', '\n', 'b' };
		byte_t eucBuf[3];

		encoder->convertFromUTF16(utf16Buf, 3, eucBuf, 3, &bytesUsed, &charsUsed);
		ASSERT_EQ(3, bytesUsed);
		ASSERT_EQ(3, charsUsed);
		ASSERT_EQ('a', eucBuf[0]);
		ASSERT_EQ('\n', eucBuf[1]);
		ASSERT_EQ('b', eucBuf[2]);
	}
	// <Test> 2Byte文字
	{
		UTF16 utf16Buf[1] = { 0x3042 };	// "あ"
		byte_t eucBuf[2];

		encoder->convertFromUTF16(utf16Buf, 1, eucBuf, 2, &bytesUsed, &charsUsed);
		ASSERT_EQ(2, bytesUsed);
		ASSERT_EQ(1, charsUsed);
		ASSERT_EQ(0xA4, eucBuf[0]);
		ASSERT_EQ(0xA2, eucBuf[1]);
	}
	// <Test> 半角カタカナ
	{
		UTF16 utf16Buf[1] = { 0xFF71 };	// "ｱ"
		byte_t eucBuf[2];

		encoder->convertFromUTF16(utf16Buf, 1, eucBuf, 2, &bytesUsed, &charsUsed);
		ASSERT_EQ(2, bytesUsed);
		ASSERT_EQ(1, charsUsed);
		ASSERT_EQ(0x8E, eucBuf[0]);
		ASSERT_EQ(0xB1, eucBuf[1]);
	}
	// <Test> 3Byte文字
	{
		UTF16 utf16Buf[1] = { 0x4E02 };	// "丂"
		byte_t eucBuf[3];

		encoder->convertFromUTF16(utf16Buf, 1, eucBuf, 3, &bytesUsed, &charsUsed);
		ASSERT_EQ(3, bytesUsed);
		ASSERT_EQ(1, charsUsed);
		ASSERT_EQ(0x8F, eucBuf[0]);
		ASSERT_EQ(0xB0, eucBuf[1]);
		ASSERT_EQ(0xA1, eucBuf[2]);
	}
}

