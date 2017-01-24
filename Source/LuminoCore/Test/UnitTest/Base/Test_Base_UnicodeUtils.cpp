#include <TestConfig.h>
#include <Lumino/Text/UnicodeUtils.h>

class Test_Base_UnicodeUtils : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Base_UnicodeUtils, GetUTF16CharCount)
{
	// UTF16 文字数チェック
	{
		// "Aｱ𩸽A"
		UnicodeUtils::UTF16 utf16Buf[] = {
			0x0041,			// 'A'
			0xFF71,			// 'ｱ'
			0xD867, 0xDE3D, // '𩸽' (正常なサロゲートペア)
			0x0041,			// 'A'
		};
		int count;
		UTFConversionResult r = UnicodeUtils::GetUTF16CharCount(utf16Buf, sizeof(utf16Buf) / sizeof(UnicodeUtils::UTF16), true, &count);
		ASSERT_EQ(UTFConversionResult_Success, r);
		ASSERT_EQ(4, count);
	}

	// UTF16 文字数チェック (不正サロゲートが含まれている)
	{
		// "Aｱ𩸽?A" (末尾が不正サロゲートペア)
		UnicodeUtils::UTF16 utf16Buf[] = {
			0x0041,			// 'A'
			0xFF71,			// 'ｱ'
			0xD867, 0xDE3D, // '𩸽' (正常なサロゲートペア)
			0xD867,			// 上位サロゲートだけ

			0x0041,			// 'A'
		};
		int count;

		// 不正チェックなし

		UTFConversionResult r = UnicodeUtils::GetUTF16CharCount(utf16Buf, sizeof(utf16Buf) / sizeof(UnicodeUtils::UTF16), false, &count);
		ASSERT_EQ(UTFConversionResult_Success, r);
		ASSERT_EQ(5, count);

		// 不正チェックあり
		r = UnicodeUtils::GetUTF16CharCount(utf16Buf, sizeof(utf16Buf) / sizeof(UnicodeUtils::UTF16), true, &count);
		ASSERT_EQ(UTFConversionResult_SourceIllegal, r);
	}

	// UTF16 文字数チェック (不正サロゲートが含まれている)
	{
		UnicodeUtils::UTF16 utf16Buf[] = {
			0xD867,			// 上位サロゲートだけでバッファ終端
		};
		int count;
		UTFConversionResult r = UnicodeUtils::GetUTF16CharCount(utf16Buf, sizeof(utf16Buf) / sizeof(UnicodeUtils::UTF16), true, &count);
		ASSERT_EQ(UTFConversionResult_SourceExhausted, r);
	}

	// UTF16 文字数チェック (不正サロゲートが含まれている)
	{
		UnicodeUtils::UTF16 utf16Buf[] = {
			0xDE3D,			// いきなり下位サロゲート

		};
		int count;
		UTFConversionResult r = UnicodeUtils::GetUTF16CharCount(utf16Buf, sizeof(utf16Buf) / sizeof(UnicodeUtils::UTF16), true, &count);
		ASSERT_EQ(UTFConversionResult_SourceIllegal, r);
	}
}

