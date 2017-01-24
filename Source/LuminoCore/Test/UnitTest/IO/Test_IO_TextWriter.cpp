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
		StreamWriter writer(TEMPFILE("Test1.txt"), Encoding::GetUTF8Encoding());
		writer.Write(32);
	}

	String str = FileSystem::ReadAllText(TEMPFILE("Test1.txt"));
	ASSERT_EQ(_T("32"), str);
}


//------------------------------------------------------------------------------
TEST_F(Test_IO_TextWriter, Write)
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
		writer.Write(_T("ab"));
		writer.Write(String("def"));
		writer.Write(_T('g'));
		writer.Write(v_int16_t);
		writer.Write(v_int32_t);
		writer.Write(v_int64_t);
		writer.Write(v_uint16_t);
		writer.Write(v_uint32_t);
		writer.Write(v_uint64_t);
		writer.Write(v_float);
		writer.Write(v_double);

		String s = writer.ToString();
		ASSERT_EQ(_T("abdefg327672147483647922337203685477580765535429496729518446744073709551615100.000000100.000000"), s);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_IO_TextWriter, WriteLine)
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
		writer.SetNewLine(_T("\n"));
		writer.WriteLine(StringRef(_T("abc"), 2));
		writer.WriteLine(String("def"));
		writer.WriteLine(_T('g'));
		writer.WriteLine(v_int16_t);
		writer.WriteLine(v_int32_t);
		writer.WriteLine(v_int64_t);
		writer.WriteLine(v_uint16_t);
		writer.WriteLine(v_uint32_t);
		writer.WriteLine(v_uint64_t);
		writer.WriteLine(v_float);
		writer.WriteLine(v_double);

		String s = writer.ToString();
		ASSERT_EQ(_T("ab\ndef\ng\n32767\n2147483647\n9223372036854775807\n65535\n4294967295\n18446744073709551615\n100.000000\n100.000000\n"), s);
	}
}
