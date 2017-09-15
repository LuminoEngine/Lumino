#include <TestConfig.h>
#include <Lumino/IO/StringWriter.h>
#include <Lumino/Json/JsonWriter.h>
using namespace ln::tr;

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
	writer.writePropertyName(_LT("hello"));
	writer.writeString(_LT("world"));
	writer.writePropertyName(_LT("t"));
	writer.writeBool(true);
	writer.writePropertyName(_LT("f"));
	writer.writeBool(false);
	writer.writePropertyName(_LT("n"));
	writer.writeNull();
	writer.writePropertyName(_LT("i"));
	writer.writeDouble(123);
	writer.writePropertyName(_LT("pi"));
	writer.writeDouble(3.1416);
	writer.writePropertyName(_LT("a"));
	writer.writeStartArray();
	for (unsigned i = 0; i < 4; i++)
		writer.writeDouble(i);
	writer.writeEndArray();
	writer.writeEndObject();

	ASSERT_EQ(
		_LT("{\"hello\":\"world\",\"t\":true,\"f\":false,\"n\":null,\"i\":123.000000,\"pi\":3.141600,\"a\":[0.000000,1.000000,2.000000,3.000000]}"),
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
		writer.writePropertyName(_LT("hello"));
		writer.writeString(_LT("world"));
		writer.writeEndObject();

		ASSERT_EQ(
			_LT("{\"hello\":\"world\"}"),
			s.toString());
	}
	// <Test> フォーマット有効
	{
		StringWriter s;
		s.setNewLine(_LT("\n"));
		JsonWriter writer(&s);
		writer.setFormatting(JsonFormatting::Indented);

		writer.writeStartObject();
		writer.writePropertyName(_LT("hello"));
		writer.writeString(_LT("world"));
		writer.writeEndObject();

		ASSERT_EQ(
			_LT("{\n  \"hello\": \"world\"\n}"),
			s.toString());
	}
}
