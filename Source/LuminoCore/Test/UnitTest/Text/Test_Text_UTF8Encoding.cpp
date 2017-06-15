#include <TestConfig.h>
#include <Lumino/Text/Encoding.h>

class Test_Text_UTF8Encoding_Unit : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

static UTF8 g_utf8Hokke[] = { 0xF0, 0xA9, 0xB8, 0xBD };	// ほっけ
static UTF16 g_utf16Hokke[2] = { 0xD867, 0xDE3D };		// ほっけ

//---------------------------------------------------------------------
TEST_F(Test_Text_UTF8Encoding_Unit, convertToUTF16)
{
	std::unique_ptr<Decoder> decoder(Encoding::getUTF8Encoding()->createDecoder());

	UTF8 utf8Buf[] = {
		0xE6, 0x97, 0xA5,	// '日'
		0xE6, 0x9C, 0xAC,	// '本'
		0xE8, 0xAA, 0x9E,	// '語'
	};
	UTF16 utf16Buf[3] = { 0, 0, 0 };

	size_t bytesUsed, charsUsed;

	// '日' を 1byte ずつ状態を保持しながら変換
	decoder->convertToUTF16(&utf8Buf[0], 1, &utf16Buf[0], 1, &bytesUsed, &charsUsed);
	ASSERT_EQ(0x0000, utf16Buf[0]);	// まだ文字は出ない
	ASSERT_EQ(0, bytesUsed);		// まだバイトも書き込まれない
	ASSERT_EQ(0, charsUsed);		// まだ文字は出ない
	decoder->convertToUTF16(&utf8Buf[1], 1, &utf16Buf[0], 1, &bytesUsed, &charsUsed);
	ASSERT_EQ(0x0000, utf16Buf[0]);	// まだ文字は出ない
	ASSERT_EQ(0, bytesUsed);		// まだバイトも書き込まれない
	ASSERT_EQ(0, charsUsed);		// まだ文字は出ない
	decoder->convertToUTF16(&utf8Buf[2], 1, &utf16Buf[0], 1, &bytesUsed, &charsUsed);
	ASSERT_EQ(0x65E5, utf16Buf[0]);	// '日'
	ASSERT_EQ(2, bytesUsed);		// 2バイト書き込まれた
	ASSERT_EQ(1, charsUsed);		// 1文字出てきた

	// 1文字変換 (サロゲート)
	decoder->convertToUTF16(&g_utf8Hokke[0], 4, &utf16Buf[0], 2, &bytesUsed, &charsUsed);
	ASSERT_EQ(0, memcmp(g_utf16Hokke, utf16Buf, 4));	// ほっけ
	ASSERT_EQ(4, bytesUsed);		// 4バイト書き込まれた
	ASSERT_EQ(1, charsUsed);		// 1文字出てきた

	// '本' を一度に変換
	decoder->convertToUTF16(&utf8Buf[3], 3, &utf16Buf[1], 1, &bytesUsed, &charsUsed);
	ASSERT_EQ(0x672C, utf16Buf[1]);	// '本'
	ASSERT_EQ(2, bytesUsed);		// 2バイト書き込まれた
	ASSERT_EQ(1, charsUsed);		// 1文字出てきた

	// '語' を 2, 1byte に分けて変換
	decoder->convertToUTF16(&utf8Buf[6], 2, &utf16Buf[2], 1, &bytesUsed, &charsUsed);
	ASSERT_EQ(0x0000, utf16Buf[2]);	// まだ文字は出ない
	ASSERT_EQ(0, bytesUsed);		// まだバイトも書き込まれない
	ASSERT_EQ(0, charsUsed);		// まだ文字は出ない
	decoder->convertToUTF16(&utf8Buf[8], 1, &utf16Buf[2], 1, &bytesUsed, &charsUsed);
	ASSERT_EQ(0x8A9E, utf16Buf[2]);	// '語'
	ASSERT_EQ(2, bytesUsed);		// 2バイト書き込まれた
	ASSERT_EQ(1, charsUsed);		// 1文字出てきた

}

//---------------------------------------------------------------------
TEST_F(Test_Text_UTF8Encoding_Unit, convertFromUTF16)
{
	std::unique_ptr<Encoder> encoder(Encoding::getUTF8Encoding()->createEncoder());

	UTF16 utf16Buf[3] = { 0x65E5, 0x672C, 0x8A9E };	// "日本語"
	UTF8 utf8Buf[6];
	size_t bytesUsed, charsUsed;

	// 1文字変換
	encoder->convertFromUTF16(&utf16Buf[0], 1, utf8Buf, 6, &bytesUsed, &charsUsed);
	ASSERT_EQ(0xE6, utf8Buf[0]); ASSERT_EQ(0x97, utf8Buf[1]); ASSERT_EQ(0xA5, utf8Buf[2]);	// '日'
	ASSERT_EQ(3, bytesUsed);		// 3バイト書き込まれた
	ASSERT_EQ(1, charsUsed);		// 1文字出てきた

	// 1文字変換 (サロゲート)
	encoder->convertFromUTF16(&g_utf16Hokke[0], 2, utf8Buf, 6, &bytesUsed, &charsUsed);
	ASSERT_EQ(0, memcmp(utf8Buf, g_utf8Hokke, 4));	// ほっけ
	ASSERT_EQ(4, bytesUsed);		// 4バイト書き込まれた
	ASSERT_EQ(1, charsUsed);		// 1文字出てきた

	// 2文字変換
	encoder->convertFromUTF16(&utf16Buf[1], 2, utf8Buf, 6, &bytesUsed, &charsUsed);
	ASSERT_EQ(0xE6, utf8Buf[0]); ASSERT_EQ(0x9C, utf8Buf[1]); ASSERT_EQ(0xAC, utf8Buf[2]);	// '本'
	ASSERT_EQ(0xE8, utf8Buf[3]); ASSERT_EQ(0xAA, utf8Buf[4]); ASSERT_EQ(0x9E, utf8Buf[5]);	// '語'
	ASSERT_EQ(6, bytesUsed);		// 6バイト書き込まれた
	ASSERT_EQ(2, charsUsed);		// 2文字出てきた

}
