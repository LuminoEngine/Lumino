#include "Common.hpp"
#include <LuminoCore/IO/StreamReader.hpp>
#include <LuminoCore/IO/StringReader.hpp>
#include <LuminoCore/Json/JsonReader.hpp>

class Test_Json_JsonReader : public ::testing::Test {};

//---------------------------------------------------------------------
TEST_F(Test_Json_JsonReader, Basic2)
{
	{
		JsonReader reader(_LT("{}"));
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::StartObject, reader.nodeType());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::EndObject, reader.nodeType());
		ASSERT_EQ(false, reader.read());
	}
	// <Test> JSON のルート要素は配列も可能。
	// <Test> 先頭の空白は読み飛ばす。
	{
		JsonReader reader(_LT(" \t[]"));
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::StartArray, reader.nodeType());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::EndArray, reader.nodeType());
		ASSERT_EQ(false, reader.read());
	}

	// <Test> オブジェクト
	// <Test> プロパティ名
	// <Test> 文字列値
	{
		JsonReader reader(_LT("{\"name\":\"str\"}"));
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::StartObject, reader.nodeType());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::PropertyName, reader.nodeType()); ASSERT_EQ(_LT("name"), reader.value());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::String, reader.nodeType()); ASSERT_EQ(_LT("str"), reader.value());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::EndObject, reader.nodeType());
		ASSERT_EQ(false, reader.read());
	}
	// <Test> 複数のメンバを持つオブジェクト
	{
		JsonReader reader(_LT("{\"name\":\"str\",\"name2\":\"str2\"}"));
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::StartObject, reader.nodeType());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::PropertyName, reader.nodeType()); ASSERT_EQ(_LT("name"), reader.value());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::String, reader.nodeType()); ASSERT_EQ(_LT("str"), reader.value());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::PropertyName, reader.nodeType()); ASSERT_EQ(_LT("name2"), reader.value());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::String, reader.nodeType()); ASSERT_EQ(_LT("str2"), reader.value());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::EndObject, reader.nodeType());
		ASSERT_EQ(false, reader.read());
	}
	// <Test> null
	// <Test> true
	// <Test> false
	{
		JsonReader reader(_LT("[null,true,false]"));
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::StartArray, reader.nodeType());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::Null, reader.nodeType());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::Boolean, reader.nodeType()); ASSERT_EQ(_LT("true"), reader.value());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::Boolean, reader.nodeType()); ASSERT_EQ(_LT("false"), reader.value());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::EndArray, reader.nodeType());
		ASSERT_EQ(false, reader.read());
	}
	// <Test> 数値
	{
		JsonReader reader(_LT("[10]"));
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::StartArray, reader.nodeType());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::Int32, reader.nodeType()); ASSERT_EQ(_LT("10"), reader.value());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::EndArray, reader.nodeType());
		ASSERT_EQ(false, reader.read());
	}
}

//---------------------------------------------------------------------
TEST_F(Test_Json_JsonReader, value)
{
	// <Test> Bool の値の取得
	{
		JsonReader reader(_LT("[true,false]"));
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::StartArray, reader.nodeType());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::Boolean, reader.nodeType()); ASSERT_EQ(true, reader.boolValue());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::Boolean, reader.nodeType()); ASSERT_EQ(false, reader.boolValue());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::EndArray, reader.nodeType());
		ASSERT_EQ(false, reader.read());
	}
	// <Test> Int32 と Int64 と Double の区別
	// <Test> Int32 と Int64 と Double の値の取得
	{
		JsonReader reader(_LT("[-2147483648,2147483647,-2147483649,2147483648,1.0,-1.0]"));
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::StartArray, reader.nodeType());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::Int32, reader.nodeType()); ASSERT_EQ(-2147483648, reader.int32Value());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::Int32, reader.nodeType()); ASSERT_EQ(2147483647, reader.int32Value());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::Int64, reader.nodeType()); ASSERT_EQ(-2147483649LL, reader.int64Value());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::Int64, reader.nodeType()); ASSERT_EQ(2147483648LL, reader.int64Value());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::Float, reader.nodeType()); ASSERT_EQ(1.0, reader.floatValue());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::Float, reader.nodeType()); ASSERT_EQ(-1.0, reader.floatValue());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonNode::EndArray, reader.nodeType());
		ASSERT_EQ(false, reader.read());
	}
}

//---------------------------------------------------------------------
TEST_F(Test_Json_JsonReader, Integrate)
{
	// http://json.org/example.html
	// <Integrate> オフィシャルページのサンプル1
	{
		StreamReader r(LN_LOCALFILE(_TT("TestData/JSONExample1.json")));
		JsonReader jr(&r);
		ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::StartObject, jr.nodeType());
			ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::PropertyName, jr.nodeType()); ASSERT_EQ(_LT("glossary"), jr.value());
			ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::StartObject, jr.nodeType());
				ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::PropertyName, jr.nodeType()); ASSERT_EQ(_LT("title"), jr.value());
				ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::String, jr.nodeType()); ASSERT_EQ(_LT("example glossary"), jr.value());
				ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::PropertyName, jr.nodeType()); ASSERT_EQ(_LT("GlossDiv"), jr.value());
				ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::StartObject, jr.nodeType());
					ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::PropertyName, jr.nodeType()); ASSERT_EQ(_LT("title"), jr.value());
					ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::String, jr.nodeType()); ASSERT_EQ(_LT("S"), jr.value());
					ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::PropertyName, jr.nodeType()); ASSERT_EQ(_LT("GlossList"), jr.value());
					ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::StartObject, jr.nodeType());
						ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::PropertyName, jr.nodeType()); ASSERT_EQ(_LT("GlossEntry"), jr.value());
						ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::StartObject, jr.nodeType());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::PropertyName, jr.nodeType()); ASSERT_EQ(_LT("ID"), jr.value());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::String, jr.nodeType()); ASSERT_EQ(_LT("SGML"), jr.value());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::PropertyName, jr.nodeType()); ASSERT_EQ(_LT("SortAs"), jr.value());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::String, jr.nodeType()); ASSERT_EQ(_LT("SGML"), jr.value());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::PropertyName, jr.nodeType()); ASSERT_EQ(_LT("GlossTerm"), jr.value());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::String, jr.nodeType()); ASSERT_EQ(_LT("Standard Generalized Markup Language"), jr.value());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::PropertyName, jr.nodeType()); ASSERT_EQ(_LT("Acronym"), jr.value());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::String, jr.nodeType()); ASSERT_EQ(_LT("SGML"), jr.value());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::PropertyName, jr.nodeType()); ASSERT_EQ(_LT("Abbrev"), jr.value());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::String, jr.nodeType()); ASSERT_EQ(_LT("ISO 8879:1986"), jr.value());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::PropertyName, jr.nodeType()); ASSERT_EQ(_LT("GlossDef"), jr.value());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::StartObject, jr.nodeType());
								ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::PropertyName, jr.nodeType()); ASSERT_EQ(_LT("para"), jr.value());
								ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::String, jr.nodeType()); ASSERT_EQ(_LT("A meta-markup language, used to create markup languages such as DocBook."), jr.value());
								ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::PropertyName, jr.nodeType()); ASSERT_EQ(_LT("GlossSeeAlso"), jr.value());
								ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::StartArray, jr.nodeType());
									ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::String, jr.nodeType()); ASSERT_EQ(_LT("GML"), jr.value());
									ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::String, jr.nodeType()); ASSERT_EQ(_LT("XML"), jr.value());
								ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::EndArray, jr.nodeType());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::EndObject, jr.nodeType());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::PropertyName, jr.nodeType()); ASSERT_EQ(_LT("GlossSee"), jr.value());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::String, jr.nodeType()); ASSERT_EQ(_LT("markup"), jr.value());
						ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::EndObject, jr.nodeType());
					ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::EndObject, jr.nodeType());
				ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::EndObject, jr.nodeType());
			ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::EndObject, jr.nodeType());
		ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonNode::EndObject, jr.nodeType());
		ASSERT_EQ(false, jr.read());
	}
	// <Integrate> オフィシャルページのサンプル2
	{
		StreamReader r(LN_LOCALFILE(_TT("TestData/JSONExample2.json")));
		JsonReader jr(&r);
		while (jr.read());
		ASSERT_EQ(JsonDiagCode::None, jr.diag().code);
		ASSERT_EQ(true, r.isEOF());
	}
	// <Integrate> オフィシャルページのサンプル3
	{
		StreamReader r(LN_LOCALFILE(_TT("TestData/JSONExample3.json")));
		JsonReader jr(&r);
		while (jr.read());
		ASSERT_EQ(JsonDiagCode::None, jr.diag().code);
		ASSERT_EQ(true, r.isEOF());
	}
	// <Integrate> オフィシャルページのサンプル4
	{
		StreamReader r(LN_LOCALFILE(_TT("TestData/JSONExample4.json")));
		JsonReader jr(&r);
		while (jr.read());
		ASSERT_EQ(JsonDiagCode::None, jr.diag().code);
		ASSERT_EQ(true, r.isEOF());
	}
	// <Integrate> オフィシャルページのサンプル5
	{
		StreamReader r(LN_LOCALFILE(_TT("TestData/JSONExample5.json")));
		JsonReader jr(&r);
		while (jr.read());
		ASSERT_EQ(JsonDiagCode::None, jr.diag().code);
		ASSERT_EQ(true, r.isEOF());
	}
}

//---------------------------------------------------------------------
TEST_F(Test_Json_JsonReader, Error)
{
	// <Test> UnterminatedString
	{
		JsonReader reader(_LT("{\""));
		while (reader.read());
		ASSERT_EQ(JsonDiagCode::UnterminatedString, reader.diag().code);
	}
	// <Test> InvalidStringChar
	{
		JsonReader reader(_LT("{\"\a\"}"));
		while (reader.read());
		ASSERT_EQ(JsonDiagCode::InvalidStringChar, reader.diag().code);
	}
	// <Test> InvalidStringEscape
	{
		JsonReader reader(_LT("{\"\\a\"}"));
		while (reader.read());
		ASSERT_EQ(JsonDiagCode::InvalidStringEscape, reader.diag().code);
	}
	// <Test> InvalidObjectClosing
	{
		JsonReader reader(_LT("{\"\":\"\",}"));
		while (reader.read());
		ASSERT_EQ(JsonDiagCode::InvalidObjectClosing, reader.diag().code);
		ASSERT_EQ(7, reader.diag().column);	// } の位置
	}
	// <Test> ArrayInvalidClosing
	{
		JsonReader reader(_LT("[\"\",]"));
		while (reader.read());
		ASSERT_EQ(JsonDiagCode::ArrayInvalidClosing, reader.diag().code);
		ASSERT_EQ(4, reader.diag().column);	// ] の位置
	}
	// <Test> ValueInvalid
	{
		JsonReader reader(_LT("[n]"));
		while (reader.read());
		ASSERT_EQ(JsonDiagCode::ValueInvalid, reader.diag().code);
		JsonReader reader2(_LT("[t]"));
		while (reader2.read());
		ASSERT_EQ(JsonDiagCode::ValueInvalid, reader.diag().code);
		JsonReader reader3(_LT("[f]"));
		while (reader3.read());
		ASSERT_EQ(JsonDiagCode::ValueInvalid, reader.diag().code);
	}
}

//---------------------------------------------------------------------
TEST_F(Test_Json_JsonReader, Issues)
{
	// <Issue> ネストされた配列の終端で解析が終了してしまう。
	{
		StringReader r(_LT(R"([{"tags": ["dolor"]},"end"])"));
		JsonReader jr(&r);
		while (jr.read());
		ASSERT_EQ(JsonDiagCode::None, jr.diag().code);
		ASSERT_EQ(true, r.isEOF());
	}
	// <Issue> プロパティの値が配列だと、次のプロパティの解析で終了してしまう。
	{
		StringReader r(_LT(R"({"tags": [],"friends":"10"})"));
		JsonReader jr(&r);
		while (jr.read());
		ASSERT_EQ(JsonDiagCode::None, jr.diag().code);
		ASSERT_EQ(true, r.isEOF());
	}
}

//---------------------------------------------------------------------
//TEST_F(Test_Json_JsonReader, JsonGenerator)
//{
//	StreamReader r(Test_GetTempFilePath(_LT("JsonGenerator.txt")));
//	JsonReader jr(&r);
//
//	while (jr.Read())
//	{
//		printf("%s ", jr.value().c_str());
//	}
//	ASSERT_EQ(JsonDiagCode::NoError, jr.GetError().code);
//	ASSERT_EQ(true, r.IsEOF());
//}
