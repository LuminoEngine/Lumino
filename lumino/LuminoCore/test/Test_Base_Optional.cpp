#include "Common.hpp"
#include <LuminoCore/Base/Optional.hpp>

class Test_Base_Optional : public ::testing::Test {};

TEST_F(Test_Base_Optional, Construct)
{
	//* [ ] constructors
	{
		String test1 = _T("test");

		Optional<String> opt1;
		Optional<String> opt2 = nullptr;
		Optional<String> opt3 = String(_T("test"));
		Optional<String> opt4 = std::move(test1);

		ASSERT_EQ(false, opt1.hasValue());
		ASSERT_EQ(false, opt2.hasValue());
		ASSERT_EQ(true, opt3.hasValue());
		ASSERT_EQ(true, opt4.hasValue());
		ASSERT_EQ(true, test1.isEmpty());

		Optional<String> opt5 = std::move(opt3);
		ASSERT_EQ(false, opt3.hasValue());
		ASSERT_EQ(true, opt5.hasValue());

		Optional<String> opt6 = opt5;
		ASSERT_EQ(true, opt5.hasValue());
		ASSERT_EQ(true, opt6.hasValue());
	}

	//* [ ] operator =
	{
		String test1 = _T("test");

		Optional<String> opt1;
		Optional<String> opt2;
		Optional<String> opt3;

		opt1 = std::move(test1);
		ASSERT_EQ(true, opt1.hasValue());
		ASSERT_EQ(true, test1.isEmpty());

		opt2 = opt1;
		ASSERT_EQ(true, opt1.hasValue());
		ASSERT_EQ(true, opt2.hasValue());

		opt3 = std::move(opt2);
		ASSERT_EQ(false, opt2.hasValue());
		ASSERT_EQ(true, opt3.hasValue());
		ASSERT_EQ(_T("test"), opt3.value());

		opt3 = opt3;
		ASSERT_EQ(true, opt3.hasValue());
		ASSERT_EQ(_T("test"), opt3.value());

		opt3 = nullptr;
		ASSERT_EQ(false, opt3.hasValue());
	}

	//* [ ] reset
	{
		Optional<String> opt1 = _T("test");
		opt1.reset();
		ASSERT_EQ(false, opt1.hasValue());
	}
}

TEST_F(Test_Base_Optional, Access)
{
	//* [ ] operator bool
	{
		Optional<String> opt1;
		Optional<String> opt2 = _T("test");
		bool r1 = (opt1) ? true : false;
		bool r2 = !opt2;
		ASSERT_EQ(false, r1);
		ASSERT_EQ(false, r2);
	}

	//* [ ] operator*
	{
		Optional<String> opt1 = _T("test");
		const Optional<String> opt2 = _T("test");

		String& s1 = *opt1;
		String&& s2 = *std::move(opt1);
		const String& s3 = *opt2;
		const String&& s4 = *std::move(opt2);

		ASSERT_EQ(_T("test"), s1);
		ASSERT_EQ(_T("test"), s3);

		//Optional<String> opt3;
		//String s = *opt3;
	}

	//* [ ] operator->
	{
		Optional<String> opt1 = _T("test");
		const Optional<String> opt2 = _T("test");

		ASSERT_EQ(4, opt1->length());
		ASSERT_EQ(4, opt2->length());
	}

	//* [ ] value
	{
		Optional<String> opt1 = _T("test");
		const Optional<String> opt2 = _T("test");

		String& s1 = opt1.value();
		String&& s2 = std::move(opt1).value();
		const String& s3 = opt2.value();
		const String&& s4 = std::move(opt2).value();

		ASSERT_EQ(_T("test"), s1);
		ASSERT_EQ(_T("test"), s3);
	}

	//* [ ] valueOr
	{
		Optional<String> opt1 = _T("test");
		Optional<String> opt2 = _T("test");
		Optional<String> opt3;
		Optional<String> opt4;

		ASSERT_EQ(_T("test"), opt1.valueOr(_T("a")));
		ASSERT_EQ(_T("test"), std::move(opt2).valueOr(_T("a")));
		ASSERT_EQ(_T("a"), opt3.valueOr(_T("a")));
		ASSERT_EQ(_T("a"), std::move(opt4).valueOr(_T("a")));
	}
}

TEST_F(Test_Base_Optional, Operators)
{
	//* [ ] operator==
	{
		String test = _T("test");
		Optional<String> opt1;
		Optional<String> opt2;
		Optional<String> opt3 = test;
		Optional<String> opt4 = test;

		ASSERT_EQ(true, opt1 == opt2);
		ASSERT_EQ(false, opt1 == opt3);
		ASSERT_EQ(true, opt3 == opt4);
		ASSERT_EQ(true, opt1 == nullptr);
		ASSERT_EQ(false, opt3 == nullptr);
		ASSERT_EQ(false, opt1 == test);
		ASSERT_EQ(true, opt3 == test);
	}
	//* [ ] operator!=
	{
		String test = _T("test");
		Optional<String> opt1;
		Optional<String> opt2;

		ASSERT_EQ(false, opt1 != opt2);
		ASSERT_EQ(false, opt1 != nullptr);
		ASSERT_EQ(true, opt1 != test);
	}
}
