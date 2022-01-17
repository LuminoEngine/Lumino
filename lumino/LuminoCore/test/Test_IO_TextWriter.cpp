#include "Common.hpp"
#include <LuminoCore/IO/TextWriter.hpp>
#include <LuminoCore/IO/StringWriter.hpp>

//==============================================================================
//# TextWriter
class Test_IO_TextWriter : public ::testing::Test { };

//# writing test
TEST_F(Test_IO_TextWriter, write)
{
	//* [ ] test for write()
	{
		int16_t v_int16_t = INT16_MAX;
		int32_t v_int32_t = INT32_MAX;
		int64_t v_int64_t = INT64_MAX;
		uint16_t v_uint16_t = UINT16_MAX;
		uint32_t v_uint32_t = UINT32_MAX;
		uint64_t v_uint64_t = UINT64_MAX;
		float v_float = 100;
		double v_double = 100;

		StringWriter writer;
		writer.write(_LT("ab"));
		writer.write(String(_TT("def")));
		writer.write(_LT('g'));
		writer.write(v_int16_t);
		writer.write(v_int32_t);
		writer.write(v_int64_t);
		writer.write(v_uint16_t);
		writer.write(v_uint32_t);
		writer.write(v_uint64_t);
		writer.write(v_float);
		writer.write(v_double);

		String s = writer.toString();
		ASSERT_EQ(_LT("abdefg327672147483647922337203685477580765535429496729518446744073709551615100.000000100.000000"), s);
	}
	//* [ ] test for writeLine()
	{
		int16_t v_int16_t = INT16_MAX;
		int32_t v_int32_t = INT32_MAX;
		int64_t v_int64_t = INT64_MAX;
		uint16_t v_uint16_t = UINT16_MAX;
		uint32_t v_uint32_t = UINT32_MAX;
		uint64_t v_uint64_t = UINT64_MAX;
		float v_float = 100;
		double v_double = 100;

		StringWriter writer;
		writer.setNewLine(_LT("\n"));
		writer.writeLine(StringView(_LT("abc"), 2));
		writer.writeLine(String(_TT("def")));
		writer.writeLine(_LT('g'));
		writer.writeLine(v_int16_t);
		writer.writeLine(v_int32_t);
		writer.writeLine(v_int64_t);
		writer.writeLine(v_uint16_t);
		writer.writeLine(v_uint32_t);
		writer.writeLine(v_uint64_t);
		writer.writeLine(v_float);
		writer.writeLine(v_double);

		String s = writer.toString();
		ASSERT_EQ(_LT("ab\ndef\ng\n32767\n2147483647\n9223372036854775807\n65535\n4294967295\n18446744073709551615\n100.000000\n100.000000\n"), s);
	}
	//* [ ] format string test
	{
		StringWriter writer;
		writer.setNewLine(_T("\n"));
		writer.writeFormat(_T("{0}-{1}"), 10, _T("A"));
		writer.writeLineFormat(_T("{0}:{1}"), 20, _T("B"));
		String s = writer.toString();
		ASSERT_EQ(_LT("10-A20:B\n"), s);
	}
}

