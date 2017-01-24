#include <TestConfig.h>
#include <Lumino/Text/Encoding.h>

class Test_Base_Encoding_UTF8 : public ::testing::Test
{
protected:
	virtual void SetUp() 
	{
		setlocale(LC_ALL, "");
	}
	virtual void TearDown() {}
};

//---------------------------------------------------------------------
TEST_F(Test_Base_Encoding_UTF8, Basic)
{
	EncodingConversionOptions options;
	options.NullTerminated = false;

	// 最小バイト数
	{
		Encoding* e = Encoding::GetUTF8Encoding();
		ASSERT_EQ(1, e->GetMinByteCount());
	}

	// 最大バイト数
	{
		Encoding* e = Encoding::GetUTF8Encoding();
		ASSERT_EQ(6, e->GetMaxByteCount());
	}
	// UTF-8 → UTF-16
	{
		// "日本語" (UTF-8)
		byte_t str1[] = { 0xE6, 0x97, 0xA5, 0xE6, 0x9C, 0xAC, 0xE8, 0xAA, 0x9E };

		std::unique_ptr<Decoder> decoder(Encoding::GetUTF8Encoding()->CreateDecoder());
		std::unique_ptr<Encoder> encoder(Encoding::GetUTF16Encoding()->CreateEncoder());

		//size_t bytesUsed;
		//size_t charsUsed;
		//bool usedDefaultChar;
		EncodingConversionResult info;
		ByteBuffer buf1 = Encoding::Convert(str1, 9, decoder.get(), encoder.get(), options, &info);
		
		uint16_t* utf16str = (uint16_t*)buf1.GetData();

		ASSERT_EQ(6, info.BytesUsed);
		ASSERT_EQ(3, info.CharsUsed);
		ASSERT_EQ(false, info.UsedDefaultChar);
		ASSERT_EQ(0x65E5, utf16str[0]);	// L'日'
		ASSERT_EQ(0x672C, utf16str[1]);	// L'本'
		ASSERT_EQ(0x8A9e, utf16str[2]);	// L'語'
	}
	// UTF-16 → UTF-8
	{
		// "日本語" (UTF-16)
		uint16_t utf16str[] = { 0x65E5, 0x672C, 0x8A9e };

		std::unique_ptr<Decoder> decoder(Encoding::GetUTF16Encoding()->CreateDecoder());
		std::unique_ptr<Encoder> encoder(Encoding::GetUTF8Encoding()->CreateEncoder());

		//size_t bytesUsed;
		//size_t charsUsed;
		//bool usedDefaultChar;
		EncodingConversionResult info;
		ByteBuffer buf1 = Encoding::Convert(utf16str, 6, decoder.get(), encoder.get(), options, &info);

		uint8_t* utf8str = (uint8_t*)buf1.GetData();

		ASSERT_EQ(9, info.BytesUsed);
		ASSERT_EQ(3, info.CharsUsed);
		ASSERT_EQ(false, info.UsedDefaultChar);
		ASSERT_EQ(0xE6, utf8str[0]);	// L'日'
		ASSERT_EQ(0x97, utf8str[1]);	// L'日'
		ASSERT_EQ(0xA5, utf8str[2]);	// L'日'
		ASSERT_EQ(0xE6, utf8str[3]);	// L'本'
		ASSERT_EQ(0x9C, utf8str[4]);	// L'本'
		ASSERT_EQ(0xAC, utf8str[5]);	// L'本'
		ASSERT_EQ(0xE8, utf8str[6]);	// L'語'
		ASSERT_EQ(0xAA, utf8str[7]);	// L'語'
		ASSERT_EQ(0x9E, utf8str[8]);	// L'語'
	}

	// 不具合修正
	{
		// "テスト環境"
		uint8_t utf8str[] = { 0xE3, 0x83, 0x86, 0xE3, 0x82, 0xB9, 0xE3, 0x83, 0x88, 0xE7, 0x92, 0xB0, 0xE5, 0xA2, 0x83 };

		// 例外とか発生しないこと
		StringA str;
		str.AssignCStr((const char*)utf8str, 15);

		// 元もまま変換されていないこと
		for (int i = 0; i < 15; i++) {
			ASSERT_EQ(true, utf8str[i] == (uint8_t)str[i]);
		}
	}
}

////---------------------------------------------------------------------
//TEST_F(Test_Base_Encoding_UTF8, Issues)
//{
//
//}
