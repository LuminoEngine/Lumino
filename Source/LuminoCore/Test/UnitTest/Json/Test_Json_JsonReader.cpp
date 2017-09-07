#include <TestConfig.h>
#include <Lumino/IO/FileSystem.h>
#include <Lumino/IO/StreamReader.h>
#include <Lumino/IO/StringReader.h>
#include <Lumino/Text/Encoding.h>
#include <Lumino/Json/JsonReader.h>
using namespace ln::tr;

class Test_Json_JsonReader : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

// 構造が正しく読めているかをチェックするための Handler
class TestJsonHandler
	: public JsonHandler
{
public:
	String Text;
	virtual bool onNull()							{ Text += _T("<null>"); return true; }
	virtual bool onBool(bool value)					{ if (value) Text += _T("<true>"); else Text += _T("<false>"); return true; }
	virtual bool onDouble(double value)				{ Text += _T("<double>"); return true; }
	virtual bool onString(const Char* str, int len)	{ Text += _T("<str>"); return true; }
	virtual bool onStartArray()						{ Text += _T("<ary>"); return true; }
	virtual bool onEndArray(int elementCount)		{ Text += _T("</ary>"); return true; }
	virtual bool onStartObject()					{ Text += _T("<obj>"); return true; }
	virtual bool onKey(const Char* str, int len)	{ Text += _T("<key>"); return true; }
	virtual bool onEndObject(int memberCount)		{ Text += _T("</obj>"); return true; }
};

//---------------------------------------------------------------------
TEST_F(Test_Json_JsonReader, Basic)
{
	String str = FileSystem::readAllText(LN_LOCALFILE("TestData/ReaderTest1.txt"), Encoding::getSystemMultiByteEncoding());
	TestJsonHandler handler;
	JsonReader reader(&handler);
	reader.parse(str);
	ASSERT_EQ(handler.Text, _T("<obj><key><null><key><true><key><false><key><double><key><str><key><ary><double><double></ary><key><obj><key><double></obj></obj>"));
}

//最後の要素の後にカンマ (,) を付けてはなりません。


//---------------------------------------------------------------------
TEST_F(Test_Json_JsonReader, Basic2)
{
	{
		JsonReader2 reader(_T("{}"));
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::StartObject, reader.getTokenType());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::EndObject, reader.getTokenType());
		ASSERT_EQ(false, reader.read());
	}
	// <Test> JSON のルート要素は配列も可能。
	// <Test> 先頭の空白は読み飛ばす。
	{
		JsonReader2 reader(_T(" \t[]"));
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::StartArray, reader.getTokenType());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::EndArray, reader.getTokenType());
		ASSERT_EQ(false, reader.read());
	}

	// <Test> オブジェクト
	// <Test> プロパティ名
	// <Test> 文字列値
	{
		JsonReader2 reader(_T("{\"name\":\"str\"}"));
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::StartObject, reader.getTokenType());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::PropertyName, reader.getTokenType()); ASSERT_EQ(_T("name"), reader.getValue());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::String, reader.getTokenType()); ASSERT_EQ(_T("str"), reader.getValue());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::EndObject, reader.getTokenType());
		ASSERT_EQ(false, reader.read());
	}
	// <Test> 複数のメンバを持つオブジェクト
	{
		JsonReader2 reader(_T("{\"name\":\"str\",\"name2\":\"str2\"}"));
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::StartObject, reader.getTokenType());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::PropertyName, reader.getTokenType()); ASSERT_EQ(_T("name"), reader.getValue());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::String, reader.getTokenType()); ASSERT_EQ(_T("str"), reader.getValue());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::PropertyName, reader.getTokenType()); ASSERT_EQ(_T("name2"), reader.getValue());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::String, reader.getTokenType()); ASSERT_EQ(_T("str2"), reader.getValue());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::EndObject, reader.getTokenType());
		ASSERT_EQ(false, reader.read());
	}
	// <Test> null
	// <Test> true
	// <Test> false
	{
		JsonReader2 reader(_T("[null,true,false]"));
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::StartArray, reader.getTokenType());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::Null, reader.getTokenType());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::Boolean, reader.getTokenType()); ASSERT_EQ(_T("true"), reader.getValue());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::Boolean, reader.getTokenType()); ASSERT_EQ(_T("false"), reader.getValue());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::EndArray, reader.getTokenType());
		ASSERT_EQ(false, reader.read());
	}
	// <Test> 数値
	{
		JsonReader2 reader(_T("[10]"));
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::StartArray, reader.getTokenType());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::Int32, reader.getTokenType()); ASSERT_EQ(_T("10"), reader.getValue());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::EndArray, reader.getTokenType());
		ASSERT_EQ(false, reader.read());
	}
}

//---------------------------------------------------------------------
TEST_F(Test_Json_JsonReader, getValue)
{
	// <Test> Bool の値の取得
	{
		JsonReader2 reader(_T("[true,false]"));
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::StartArray, reader.getTokenType());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::Boolean, reader.getTokenType()); ASSERT_EQ(true, reader.getBoolValue());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::Boolean, reader.getTokenType()); ASSERT_EQ(false, reader.getBoolValue());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::EndArray, reader.getTokenType());
		ASSERT_EQ(false, reader.read());
	}
	// <Test> Int32 と Int64 と Double の区別
	// <Test> Int32 と Int64 と Double の値の取得
	{
		JsonReader2 reader(_T("[-2147483648,2147483647,-2147483649,2147483648,1.0,-1.0]"));
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::StartArray, reader.getTokenType());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::Int32, reader.getTokenType()); ASSERT_EQ(-2147483648, reader.getInt32Value());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::Int32, reader.getTokenType()); ASSERT_EQ(2147483647, reader.getInt32Value());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::Int64, reader.getTokenType()); ASSERT_EQ(-2147483649LL, reader.getInt64Value());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::Int64, reader.getTokenType()); ASSERT_EQ(2147483648LL, reader.getInt64Value());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::Float, reader.getTokenType()); ASSERT_EQ(1.0, reader.getFloatValue());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::Float, reader.getTokenType()); ASSERT_EQ(-1.0, reader.getFloatValue());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::EndArray, reader.getTokenType());
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
		JsonReader2 jr(&r);
		ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::StartObject, jr.getTokenType());
			ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::PropertyName, jr.getTokenType()); ASSERT_EQ(_T("glossary"), jr.getValue());
			ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::StartObject, jr.getTokenType());
				ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::PropertyName, jr.getTokenType()); ASSERT_EQ(_T("title"), jr.getValue());
				ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::String, jr.getTokenType()); ASSERT_EQ(_T("example glossary"), jr.getValue());
				ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::PropertyName, jr.getTokenType()); ASSERT_EQ(_T("GlossDiv"), jr.getValue());
				ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::StartObject, jr.getTokenType());
					ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::PropertyName, jr.getTokenType()); ASSERT_EQ(_T("title"), jr.getValue());
					ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::String, jr.getTokenType()); ASSERT_EQ(_T("S"), jr.getValue());
					ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::PropertyName, jr.getTokenType()); ASSERT_EQ(_T("GlossList"), jr.getValue());
					ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::StartObject, jr.getTokenType());
						ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::PropertyName, jr.getTokenType()); ASSERT_EQ(_T("GlossEntry"), jr.getValue());
						ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::StartObject, jr.getTokenType());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::PropertyName, jr.getTokenType()); ASSERT_EQ(_T("ID"), jr.getValue());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::String, jr.getTokenType()); ASSERT_EQ(_T("SGML"), jr.getValue());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::PropertyName, jr.getTokenType()); ASSERT_EQ(_T("SortAs"), jr.getValue());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::String, jr.getTokenType()); ASSERT_EQ(_T("SGML"), jr.getValue());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::PropertyName, jr.getTokenType()); ASSERT_EQ(_T("GlossTerm"), jr.getValue());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::String, jr.getTokenType()); ASSERT_EQ(_T("Standard Generalized Markup Language"), jr.getValue());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::PropertyName, jr.getTokenType()); ASSERT_EQ(_T("Acronym"), jr.getValue());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::String, jr.getTokenType()); ASSERT_EQ(_T("SGML"), jr.getValue());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::PropertyName, jr.getTokenType()); ASSERT_EQ(_T("Abbrev"), jr.getValue());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::String, jr.getTokenType()); ASSERT_EQ(_T("ISO 8879:1986"), jr.getValue());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::PropertyName, jr.getTokenType()); ASSERT_EQ(_T("GlossDef"), jr.getValue());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::StartObject, jr.getTokenType());
								ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::PropertyName, jr.getTokenType()); ASSERT_EQ(_T("para"), jr.getValue());
								ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::String, jr.getTokenType()); ASSERT_EQ(_T("A meta-markup language, used to create markup languages such as DocBook."), jr.getValue());
								ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::PropertyName, jr.getTokenType()); ASSERT_EQ(_T("GlossSeeAlso"), jr.getValue());
								ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::StartArray, jr.getTokenType());
									ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::String, jr.getTokenType()); ASSERT_EQ(_T("GML"), jr.getValue());
									ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::String, jr.getTokenType()); ASSERT_EQ(_T("XML"), jr.getValue());
								ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::EndArray, jr.getTokenType());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::EndObject, jr.getTokenType());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::PropertyName, jr.getTokenType()); ASSERT_EQ(_T("GlossSee"), jr.getValue());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::String, jr.getTokenType()); ASSERT_EQ(_T("markup"), jr.getValue());
						ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::EndObject, jr.getTokenType());
					ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::EndObject, jr.getTokenType());
				ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::EndObject, jr.getTokenType());
			ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::EndObject, jr.getTokenType());
		ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::EndObject, jr.getTokenType());
		ASSERT_EQ(false, jr.read());
	}
	// <Integrate> オフィシャルページのサンプル2
	{
		StreamReader r(LN_LOCALFILE(_TT("TestData/JSONExample2.json")));
		JsonReader2 jr(&r);
		while (jr.read());
		ASSERT_EQ(JsonParseError2::NoError, jr.getError().code);
		ASSERT_EQ(true, r.isEOF());
	}
	// <Integrate> オフィシャルページのサンプル3
	{
		StreamReader r(LN_LOCALFILE(_TT("TestData/JSONExample3.json")));
		JsonReader2 jr(&r);
		while (jr.read());
		ASSERT_EQ(JsonParseError2::NoError, jr.getError().code);
		ASSERT_EQ(true, r.isEOF());
	}
	// <Integrate> オフィシャルページのサンプル4
	{
		StreamReader r(LN_LOCALFILE(_TT("TestData/JSONExample4.json")));
		JsonReader2 jr(&r);
		while (jr.read());
		ASSERT_EQ(JsonParseError2::NoError, jr.getError().code);
		ASSERT_EQ(true, r.isEOF());
	}
	// <Integrate> オフィシャルページのサンプル5
	{
		StreamReader r(LN_LOCALFILE(_TT("TestData/JSONExample5.json")));
		JsonReader2 jr(&r);
		while (jr.read());
		ASSERT_EQ(JsonParseError2::NoError, jr.getError().code);
		ASSERT_EQ(true, r.isEOF());
	}
}

//---------------------------------------------------------------------
TEST_F(Test_Json_JsonReader, Error)
{
	// <Test> UnterminatedString
	{
		JsonReader2 reader(_T("{\""));
		while (reader.tryRead());
		ASSERT_EQ(JsonParseError2::UnterminatedString, reader.getError().code);
	}
	// <Test> InvalidStringChar
	{
		JsonReader2 reader(_T("{\"\a\"}"));
		while (reader.tryRead());
		ASSERT_EQ(JsonParseError2::InvalidStringChar, reader.getError().code);
	}
	// <Test> InvalidStringEscape
	{
		JsonReader2 reader(_T("{\"\\a\"}"));
		while (reader.tryRead());
		ASSERT_EQ(JsonParseError2::InvalidStringEscape, reader.getError().code);
	}
	// <Test> InvalidObjectClosing
	{
		JsonReader2 reader(_T("{\"\":\"\",}"));
		while (reader.tryRead());
		ASSERT_EQ(JsonParseError2::InvalidObjectClosing, reader.getError().code);
		ASSERT_EQ(7, reader.getError().column);	// } の位置
	}
	// <Test> ArrayInvalidClosing
	{
		JsonReader2 reader(_T("[\"\",]"));
		while (reader.tryRead());
		ASSERT_EQ(JsonParseError2::ArrayInvalidClosing, reader.getError().code);
		ASSERT_EQ(4, reader.getError().column);	// ] の位置
	}
	// <Test> ValueInvalid
	{
		JsonReader2 reader(_T("[n]"));
		while (reader.tryRead());
		ASSERT_EQ(JsonParseError2::ValueInvalid, reader.getError().code);
		JsonReader2 reader2(_T("[t]"));
		while (reader2.tryRead());
		ASSERT_EQ(JsonParseError2::ValueInvalid, reader.getError().code);
		JsonReader2 reader3(_T("[f]"));
		while (reader3.tryRead());
		ASSERT_EQ(JsonParseError2::ValueInvalid, reader.getError().code);
	}
}

//---------------------------------------------------------------------
TEST_F(Test_Json_JsonReader, Issues)
{
	// <Issue> ネストされた配列の終端で解析が終了してしまう。
	{
		StringReader r(_TT(R"([{"tags": ["dolor"]},"end"])"));
		JsonReader2 jr(&r);
		while (jr.read());
		ASSERT_EQ(JsonParseError2::NoError, jr.getError().code);
		ASSERT_EQ(true, r.isEOF());
	}
	// <Issue> プロパティの値が配列だと、次のプロパティの解析で終了してしまう。
	{
		StringReader r(_TT(R"({"tags": [],"friends":"10"})"));
		JsonReader2 jr(&r);
		while (jr.read());
		ASSERT_EQ(JsonParseError2::NoError, jr.getError().code);
		ASSERT_EQ(true, r.isEOF());
	}
}

//---------------------------------------------------------------------
//TEST_F(Test_Json_JsonReader, JsonGenerator)
//{
//	StreamReader r(Test_GetTempFilePath(_T("JsonGenerator.txt")));
//	JsonReader2 jr(&r);
//
//	while (jr.Read())
//	{
//		printf("%s ", jr.GetValue().c_str());
//	}
//	ASSERT_EQ(JsonParseError2::NoError, jr.GetError().code);
//	ASSERT_EQ(true, r.IsEOF());
//}
