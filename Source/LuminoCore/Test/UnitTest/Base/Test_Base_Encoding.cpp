#include <TestConfig.h>
#include "../../../Source/Text/UTF16Encoding.h"
#include "../../../Source/Text/UTF32Encoding.h"

class Test_Base_Encoding : public ::testing::Test
{
protected:
	virtual void SetUp() 
	{
		setlocale(LC_ALL, "");
	}
	virtual void TearDown() {}
};

//---------------------------------------------------------------------
TEST_F(Test_Base_Encoding, Basic)
{
	// Char エンコーディング
	{
#ifdef LN_UNICODE
#ifdef _WIN32
		ASSERT_TRUE(dynamic_cast<UTF16Encoding*>(Encoding::getTCharEncoding()) != NULL);
#else
		ASSERT_TRUE(dynamic_cast<UTF32Encoding*>(Encoding::getTCharEncoding()) != NULL);
#endif
#else
		// TODO
		//ASSERT_TRUE(dynamic_cast<SystemMultiByteEncoding*>(Encoding::getTCharEncoding()) != NULL);
#endif
	}
	// UTF16Encoding
	{
		ASSERT_NE((UTF16Encoding*)NULL, dynamic_cast<UTF16Encoding*>(Encoding::getUTF16Encoding()));
	}
}

//---------------------------------------------------------------------
TEST_F(Test_Base_Encoding, convert)
{
	EncodingConversionOptions options;
	options.NullTerminated = false;

	const char* buf1 = "123";
	std::unique_ptr<Decoder> decoder(Encoding::getSystemMultiByteEncoding()->createDecoder());
	std::unique_ptr<Encoder> encoder(Encoding::getWideCharEncoding()->createEncoder());

	// src が無い
	ASSERT_THROW(
		Encoding::convert(NULL, 0, (Decoder*)NULL, NULL, options, NULL),
		Exception);
	// decoder が無い
	ASSERT_THROW(
		Encoding::convert(buf1, 0, (Decoder*)NULL, NULL, options, NULL),
		Exception);
	// encoder が無い
	ASSERT_THROW(
		Encoding::convert(buf1, 0, decoder.get(), NULL, options, NULL),
		Exception);

	// ↓ストリーミング用状態記憶対応のため、許可する
	// ×最低でも必要なバイト数が無い
	//ASSERT_THROW(
	//	Encoding::convert(buf1, 0, decoder, encoder, NULL),
	//	ArgumentException);
}

//---------------------------------------------------------------------
TEST_F(Test_Base_Encoding, SystemEncodingTest)
{
	// "日本語"
#ifdef _WIN32
	byte_t str1[] = { 0x93, 0xFA, 0x96, 0x7B, 0x8C, 0xEA, 0x00 };	// sjis
#else
	byte_t str1[] = { 0xE6, 0x97, 0xA5, 0xE6, 0x9C, 0xAC, 0xE8, 0xAA, 0x9E, 0x00 };	// utf8
#endif

	// std::wstring resize テスト


	//std::wstring s;// = L"あいうえお";
	////s.reserve(15);
	//s.resize(15);
	////s = L"あいうえおあいうえおあいうえお";
	//wchar_t* p = &(s[0]);
	//memcpy(p, L"あいうえおあいうえおあいうえお", 15 * sizeof(wchar_t));
	//printf("%d\n", s.size());
	//printf("%d\n", s.capacity());
	//wprintf(L"%s\n", s.c_str());

	// 最小バイト数
	{
		Encoding* e = Encoding::getSystemMultiByteEncoding();
		ASSERT_EQ(1, e->getMinByteCount());
	}

	// 最大バイト数
	{
		//setlocale(LC_ALL, "en-US");
		//char* a = setlocale(LC_ALL, ".65001");
		//char* a = setlocale(LC_ALL, "ko-KR.949");
		//char* a = setlocale(LC_ALL, ".949");
		//printf(a);
		//printf("%d\n", _getmbcp());
		Encoding* e = Encoding::getSystemMultiByteEncoding();
		ASSERT_TRUE(1 <= e->getMaxByteCount() && e->getMaxByteCount() <= 6);
	}

	// Multi → Wide
	{
		auto str2 = String::fromCString((const char*)str1, strlen((const char*)str1));
		ASSERT_EQ(3, str2.getLength());
		ASSERT_EQ(0x65E5, str2[0]);	// L'日'
		ASSERT_EQ(0x672C, str2[1]);	// L'本'
		ASSERT_EQ(0x8A9e, str2[2]);	// L'語'

		// 1文字だけ
		auto str3 = String::fromCString("A");
		ASSERT_EQ(1, str3.getLength());
		ASSERT_EQ('A', str3[0]);
	}

	// Wide → Multi 
	{
		// "日本語"
		wchar_t wstr1[] = { 0x65E5, 0x672C, 0x8A9e, 0x0000 };

		auto str2 = String::fromCString(wstr1).toStdString();
		ASSERT_EQ(strlen((char*)str1), str2.length());
		//ASSERT_EQ(0x93, (byte_t)str2.at(0));	// '日'	※ unsingned char で比較しないと一致が取れない
		//ASSERT_EQ(0xFA, (byte_t)str2.at(1));	// '日'
		//ASSERT_EQ(0x96, (byte_t)str2.at(2));	// '本'
		//ASSERT_EQ(0x7B, (byte_t)str2.at(3));	// '本'
		//ASSERT_EQ(0x8C, (byte_t)str2.at(4));	// '語'
		//ASSERT_EQ(0xEA, (byte_t)str2.at(5));	// '語'
		ASSERT_TRUE(TestHelper::checkArrays(str2.c_str(), str1, strlen((char*)str1)));

		// 1文字だけ
		auto str3 = String::fromCString(L"A");
		ASSERT_EQ(1, str3.getLength());
		ASSERT_EQ('A', str3[0]);
	}

#if 0
	// Multi に変換できない文字があった
	{
		//wchar_t str1[] = L"";
		wchar_t str1[] = { 0xD867, 0xDE3D, 0x0000 };	// ほっけ
		StringA str2;
		ASSERT_THROW(
			str2.ConvertFrom(str1, wcslen(str1) * sizeof(wchar_t), Encoding::GetWideCharEncoding()),
			EncodingFallbackException);
	}

	// Wide に変換できない文字があった
	{
		byte_t str1[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0x00 };
		StringW str2;
		ASSERT_THROW(
			str2.ConvertFrom(str1, strlen((const char*)str1), Encoding::GetSystemMultiByteEncoding()),
			EncodingFallbackException);
	}
#endif
}

//---------------------------------------------------------------------
TEST_F(Test_Base_Encoding, UTF16Encoding)
{
	//ASSERT_EQ(0xFF, Encoding::getUTF16Encoding()->getPreamble()[0]);
	//ASSERT_EQ(0xFE, Encoding::getUTF16Encoding()->getPreamble()[1]);
	//ASSERT_EQ(0x00, Encoding::getUTF16Encoding()->getPreamble()[2]);
	
}
