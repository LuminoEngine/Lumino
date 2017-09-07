#include <TestConfig.h>
#include <Lumino/IO/StreamWriter.h>
#include <Lumino/IO/StringWriter.h>
#include <Lumino/IO/FileSystem.h>
#include <Lumino/IO/TextReader.h>

class Test_IO_TextWriter : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_IO_TextWriter, Basic)
{
	{
		StreamWriter writer(TEMPFILE("Test1.txt"), Encoding::getUTF8Encoding());
		writer.write(32);
	}

	String str = FileSystem::readAllText(TEMPFILE("Test1.txt"));
	ASSERT_EQ(_T("32"), str);
}


//------------------------------------------------------------------------------
TEST_F(Test_IO_TextWriter, write)
{
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
		writer.write(_T("ab"));
		writer.write(String("def"));
		writer.write(_T('g'));
		writer.write(v_int16_t);
		writer.write(v_int32_t);
		writer.write(v_int64_t);
		writer.write(v_uint16_t);
		writer.write(v_uint32_t);
		writer.write(v_uint64_t);
		writer.write(v_float);
		writer.write(v_double);

		String s = writer.toString();
		ASSERT_EQ(_T("abdefg327672147483647922337203685477580765535429496729518446744073709551615100.000000100.000000"), s);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_IO_TextWriter, writeLine)
{
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
		writer.setNewLine(_T("\n"));
		writer.writeLine(StringRef(_T("abc"), 2));
		writer.writeLine(String("def"));
		writer.writeLine(_T('g'));
		writer.writeLine(v_int16_t);
		writer.writeLine(v_int32_t);
		writer.writeLine(v_int64_t);
		writer.writeLine(v_uint16_t);
		writer.writeLine(v_uint32_t);
		writer.writeLine(v_uint64_t);
		writer.writeLine(v_float);
		writer.writeLine(v_double);

		String s = writer.toString();
		ASSERT_EQ(_T("ab\ndef\ng\n32767\n2147483647\n9223372036854775807\n65535\n4294967295\n18446744073709551615\n100.000000\n100.000000\n"), s);
	}
}

