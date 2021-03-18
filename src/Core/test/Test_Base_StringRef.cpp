#include "Common.hpp"
#include <LuminoCore/Base/String.hpp>

class Test_Base_StringRef : public ::testing::Test {};

TEST_F(Test_Base_StringRef, Issues)
{
	{
		const char astr0[] = "abc";
		const wchar_t wstr0[] = L"abc";

		//* [ ] char/wchar_t ref copy
		{
			StringRef astr1 = astr0;
			StringRef astr2 = astr1;
			StringRef wstr1 = wstr0;
			StringRef wstr2 = wstr1;
			ASSERT_EQ(_T("abc"), String(astr1));
			ASSERT_EQ(_T("abc"), String(astr2));
			ASSERT_EQ(_T("abc"), String(wstr1));
			ASSERT_EQ(_T("abc"), String(wstr2));
		}

		//* [ ] char/wchar_t ref assign
		{
			StringRef astr1;
			StringRef astr2;
			StringRef wstr1;
			StringRef wstr2;
			astr1 = astr0;
			astr2 = astr1;
			wstr1 = wstr0;
			wstr2 = wstr1;

			StringRef astr3 = astr0;
			astr3 = astr3;

			ASSERT_EQ(_T("abc"), String(astr1));
			ASSERT_EQ(_T("abc"), String(astr2));
			ASSERT_EQ(_T("abc"), String(wstr1));
			ASSERT_EQ(_T("abc"), String(wstr2));
			ASSERT_EQ(_T("abc"), String(astr3));
		}

		//* [ ] char/wchar_t ref assign
		{
			StringRef astr1;
			StringRef astr2;
			StringRef wstr1;
			StringRef wstr2;
			astr1 = astr0;
			astr2 = std::move(astr1);
			wstr1 = wstr0;
			wstr2 = std::move(wstr1);

			ASSERT_EQ(true, astr1.isEmpty());
			ASSERT_EQ(_T("abc"), String(astr2));
			ASSERT_EQ(true, wstr1.isEmpty());
			ASSERT_EQ(_T("abc"), String(wstr2));
		}
	}
}

