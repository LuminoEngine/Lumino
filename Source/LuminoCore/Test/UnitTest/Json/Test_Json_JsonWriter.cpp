#include <TestConfig.h>
#include <Lumino/IO/StringWriter.h>
#include <Lumino/Json/JsonWriter.h>
using namespace ln::tr;

#ifdef LN_USTRING
#else
//==============================================================================
class Test_Json_JsonWriter : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Json_JsonWriter, Example)
{
	StringWriter s;
	JsonWriter writer(&s);

	writer.writeStartObject();
	writer.writePropertyName(_T("hello"));
	writer.writeString(_T("world"));
	writer.writePropertyName(_T("t"));
	writer.writeBool(true);
	writer.writePropertyName(_T("f"));
	writer.writeBool(false);
	writer.writePropertyName(_T("n"));
	writer.writeNull();
	writer.writePropertyName(_T("i"));
	writer.writeDouble(123);
	writer.writePropertyName(_T("pi"));
	writer.writeDouble(3.1416);
	writer.writePropertyName(_T("a"));
	writer.writeStartArray();
	for (unsigned i = 0; i < 4; i++)
		writer.writeDouble(i);
	writer.writeEndArray();
	writer.writeEndObject();

	ASSERT_EQ(
		_T("{\"hello\":\"world\",\"t\":true,\"f\":false,\"n\":null,\"i\":123.000000,\"pi\":3.141600,\"a\":[0.000000,1.000000,2.000000,3.000000]}"),
		s.toString());
}

//------------------------------------------------------------------------------
TEST_F(Test_Json_JsonWriter, Formatting)
{
	// <Test> デフォルトではフォーマットされない
	{
		StringWriter s;
		JsonWriter writer(&s);

		writer.writeStartObject();
		writer.writePropertyName(_T("hello"));
		writer.writeString(_T("world"));
		writer.writeEndObject();

		ASSERT_EQ(
			_T("{\"hello\":\"world\"}"),
			s.toString());
	}
	// <Test> フォーマット有効
	{
		StringWriter s;
		s.setNewLine(_T("\n"));
		JsonWriter writer(&s);
		writer.setFormatting(JsonFormatting::Indented);

		writer.writeStartObject();
		writer.writePropertyName(_T("hello"));
		writer.writeString(_T("world"));
		writer.writeEndObject();

		ASSERT_EQ(
			_T("{\n  \"hello\": \"world\"\n}"),
			s.toString());
	}
}

#endif
