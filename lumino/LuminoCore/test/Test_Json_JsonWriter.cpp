#include "Common.hpp"
#include <LuminoCore/Math/Math.hpp>
#include <LuminoCore/IO/StringWriter.hpp>
#include <LuminoCore/Json/JsonWriter.hpp>

//==============================================================================
class Test_Json_JsonWriter : public ::testing::Test {};

//## check example
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

//## JSON Text formatting test
TEST_F(Test_Json_JsonWriter, Formatting)
{
	//* [ ] デフォルトではフォーマットされない
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
	//* [ ] フォーマット有効
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

TEST_F(Test_Json_JsonWriter, NaN_Inf)
{
    // TODO: いまは Sprite のシリアライズとかで使いたいので、nan として保存したりしている。
    // 本来は Json としては nan, inf はサポートしていないのでエラーにするべき。
    // ただ Lumino としては使いたいので、オプションとする設定をどこかに入れておきたい。

    StringWriter s;
    JsonWriter writer(&s);
    writer.setFormatting(JsonFormatting::None);
    writer.writeStartObject();
    writer.writePropertyName(u"v1");
    writer.writeFloat(Math::NaN);
    writer.writePropertyName(u"v2");
    writer.writeFloat(Math::Inf);
    writer.writeEndObject();
    auto json = s.toString();
    ASSERT_EQ(0, String::compare(_LT("{\"v1\":\"NaN\",\"v2\":\"Inf\"}"), json, CaseSensitivity::CaseInsensitive));
}

//## generate escape sequence test
TEST_F(Test_Json_JsonWriter, Issues_Escape)
{
	StringWriter s;
	JsonWriter writer(&s);

	writer.writeStartObject();
	writer.writePropertyName(_T("prop"));
	writer.writeString(_T("\" \\ \b \f \n \r \t"));
	writer.writeEndObject();

	auto ss = s.toString();

	auto actual = _T(R"({"prop":"\" \\ \b \f \n \r \t"})");
	ASSERT_EQ(actual, s.toString());

	//{

	//	StringReader r(ss);
	//	JsonReader jr(&r);
	//	jr.read();	// StartObject
	//	jr.read();	// PropName
	//	jr.read();	// String
	//	auto str = jr.getValue();

	//	auto actual = _T("\" \\ / \b \f \n \r \t");
	//	ASSERT_EQ(actual, str);
	//}
}
