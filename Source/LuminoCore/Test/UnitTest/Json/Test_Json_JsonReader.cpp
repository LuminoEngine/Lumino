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
	virtual bool OnNull()							{ Text += _T("<null>"); return true; }
	virtual bool OnBool(bool value)					{ if (value) Text += _T("<true>"); else Text += _T("<false>"); return true; }
	virtual bool OnDouble(double value)				{ Text += _T("<double>"); return true; }
	virtual bool OnString(const TCHAR* str, int len){ Text += _T("<str>"); return true; }
	virtual bool OnStartArray()						{ Text += _T("<ary>"); return true; }
	virtual bool OnEndArray(int elementCount)		{ Text += _T("</ary>"); return true; }
	virtual bool OnStartObject()					{ Text += _T("<obj>"); return true; }
	virtual bool OnKey(const TCHAR* str, int len)	{ Text += _T("<key>"); return true; }
	virtual bool OnEndObject(int memberCount)		{ Text += _T("</obj>"); return true; }
};

//---------------------------------------------------------------------
TEST_F(Test_Json_JsonReader, Basic)
{
	String str = FileSystem::readAllText(LN_LOCALFILE("TestData/ReaderTest1.txt"), Encoding::GetSystemMultiByteEncoding());
	TestJsonHandler handler;
	JsonReader reader(&handler);
	reader.Parse(str);
	ASSERT_EQ(handler.Text, _T("<obj><key><null><key><true><key><false><key><double><key><str><key><ary><double><double></ary><key><obj><key><double></obj></obj>"));
}

//最後の要素の後にカンマ (,) を付けてはなりません。


//---------------------------------------------------------------------
TEST_F(Test_Json_JsonReader, Basic2)
{
	{
		JsonReader2 reader(_T("{}"));
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::StartObject, reader.GetTokenType());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::EndObject, reader.GetTokenType());
		ASSERT_EQ(false, reader.read());
	}
	// <Test> JSON のルート要素は配列も可能。
	// <Test> 先頭の空白は読み飛ばす。
	{
		JsonReader2 reader(_T(" \t[]"));
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::StartArray, reader.GetTokenType());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::EndArray, reader.GetTokenType());
		ASSERT_EQ(false, reader.read());
	}

	// <Test> オブジェクト
	// <Test> プロパティ名
	// <Test> 文字列値
	{
		JsonReader2 reader(_T("{\"name\":\"str\"}"));
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::StartObject, reader.GetTokenType());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::PropertyName, reader.GetTokenType()); ASSERT_EQ(_T("name"), reader.getValue());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::String, reader.GetTokenType()); ASSERT_EQ(_T("str"), reader.getValue());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::EndObject, reader.GetTokenType());
		ASSERT_EQ(false, reader.read());
	}
	// <Test> 複数のメンバを持つオブジェクト
	{
		JsonReader2 reader(_T("{\"name\":\"str\",\"name2\":\"str2\"}"));
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::StartObject, reader.GetTokenType());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::PropertyName, reader.GetTokenType()); ASSERT_EQ(_T("name"), reader.getValue());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::String, reader.GetTokenType()); ASSERT_EQ(_T("str"), reader.getValue());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::PropertyName, reader.GetTokenType()); ASSERT_EQ(_T("name2"), reader.getValue());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::String, reader.GetTokenType()); ASSERT_EQ(_T("str2"), reader.getValue());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::EndObject, reader.GetTokenType());
		ASSERT_EQ(false, reader.read());
	}
	// <Test> null
	// <Test> true
	// <Test> false
	{
		JsonReader2 reader(_T("[null,true,false]"));
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::StartArray, reader.GetTokenType());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::Null, reader.GetTokenType());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::Boolean, reader.GetTokenType()); ASSERT_EQ(_T("true"), reader.getValue());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::Boolean, reader.GetTokenType()); ASSERT_EQ(_T("false"), reader.getValue());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::EndArray, reader.GetTokenType());
		ASSERT_EQ(false, reader.read());
	}
	// <Test> 数値
	{
		JsonReader2 reader(_T("[10]"));
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::StartArray, reader.GetTokenType());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::Int32, reader.GetTokenType()); ASSERT_EQ(_T("10"), reader.getValue());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::EndArray, reader.GetTokenType());
		ASSERT_EQ(false, reader.read());
	}
}

//---------------------------------------------------------------------
TEST_F(Test_Json_JsonReader, getValue)
{
	// <Test> Bool の値の取得
	{
		JsonReader2 reader(_T("[true,false]"));
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::StartArray, reader.GetTokenType());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::Boolean, reader.GetTokenType()); ASSERT_EQ(true, reader.GetBoolValue());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::Boolean, reader.GetTokenType()); ASSERT_EQ(false, reader.GetBoolValue());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::EndArray, reader.GetTokenType());
		ASSERT_EQ(false, reader.read());
	}
	// <Test> Int32 と Int64 と Double の区別
	// <Test> Int32 と Int64 と Double の値の取得
	{
		JsonReader2 reader(_T("[-2147483648,2147483647,-2147483649,2147483648,1.0,-1.0]"));
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::StartArray, reader.GetTokenType());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::Int32, reader.GetTokenType()); ASSERT_EQ(-2147483648, reader.GetInt32Value());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::Int32, reader.GetTokenType()); ASSERT_EQ(2147483647, reader.GetInt32Value());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::Int64, reader.GetTokenType()); ASSERT_EQ(-2147483649LL, reader.GetInt64Value());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::Int64, reader.GetTokenType()); ASSERT_EQ(2147483648LL, reader.GetInt64Value());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::Float, reader.GetTokenType()); ASSERT_EQ(1.0, reader.GetFloatValue());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::Float, reader.GetTokenType()); ASSERT_EQ(-1.0, reader.GetFloatValue());
		ASSERT_EQ(true, reader.read()); ASSERT_EQ(JsonToken::EndArray, reader.GetTokenType());
		ASSERT_EQ(false, reader.read());
	}
}

//---------------------------------------------------------------------
TEST_F(Test_Json_JsonReader, Integrate)
{
	// http://json.org/example.html
	// <Integrate> オフィシャルページのサンプル1
	{
		StreamReader r(LN_LOCALFILE(_T("TestData/JSONExample1.json")));
		JsonReader2 jr(&r);
		ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::StartObject, jr.GetTokenType());
			ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::PropertyName, jr.GetTokenType()); ASSERT_EQ(_T("glossary"), jr.getValue());
			ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::StartObject, jr.GetTokenType());
				ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::PropertyName, jr.GetTokenType()); ASSERT_EQ(_T("title"), jr.getValue());
				ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::String, jr.GetTokenType()); ASSERT_EQ(_T("example glossary"), jr.getValue());
				ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::PropertyName, jr.GetTokenType()); ASSERT_EQ(_T("GlossDiv"), jr.getValue());
				ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::StartObject, jr.GetTokenType());
					ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::PropertyName, jr.GetTokenType()); ASSERT_EQ(_T("title"), jr.getValue());
					ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::String, jr.GetTokenType()); ASSERT_EQ(_T("S"), jr.getValue());
					ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::PropertyName, jr.GetTokenType()); ASSERT_EQ(_T("GlossList"), jr.getValue());
					ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::StartObject, jr.GetTokenType());
						ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::PropertyName, jr.GetTokenType()); ASSERT_EQ(_T("GlossEntry"), jr.getValue());
						ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::StartObject, jr.GetTokenType());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::PropertyName, jr.GetTokenType()); ASSERT_EQ(_T("ID"), jr.getValue());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::String, jr.GetTokenType()); ASSERT_EQ(_T("SGML"), jr.getValue());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::PropertyName, jr.GetTokenType()); ASSERT_EQ(_T("SortAs"), jr.getValue());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::String, jr.GetTokenType()); ASSERT_EQ(_T("SGML"), jr.getValue());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::PropertyName, jr.GetTokenType()); ASSERT_EQ(_T("GlossTerm"), jr.getValue());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::String, jr.GetTokenType()); ASSERT_EQ(_T("Standard Generalized Markup Language"), jr.getValue());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::PropertyName, jr.GetTokenType()); ASSERT_EQ(_T("Acronym"), jr.getValue());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::String, jr.GetTokenType()); ASSERT_EQ(_T("SGML"), jr.getValue());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::PropertyName, jr.GetTokenType()); ASSERT_EQ(_T("Abbrev"), jr.getValue());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::String, jr.GetTokenType()); ASSERT_EQ(_T("ISO 8879:1986"), jr.getValue());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::PropertyName, jr.GetTokenType()); ASSERT_EQ(_T("GlossDef"), jr.getValue());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::StartObject, jr.GetTokenType());
								ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::PropertyName, jr.GetTokenType()); ASSERT_EQ(_T("para"), jr.getValue());
								ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::String, jr.GetTokenType()); ASSERT_EQ(_T("A meta-markup language, used to create markup languages such as DocBook."), jr.getValue());
								ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::PropertyName, jr.GetTokenType()); ASSERT_EQ(_T("GlossSeeAlso"), jr.getValue());
								ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::StartArray, jr.GetTokenType());
									ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::String, jr.GetTokenType()); ASSERT_EQ(_T("GML"), jr.getValue());
									ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::String, jr.GetTokenType()); ASSERT_EQ(_T("XML"), jr.getValue());
								ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::EndArray, jr.GetTokenType());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::EndObject, jr.GetTokenType());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::PropertyName, jr.GetTokenType()); ASSERT_EQ(_T("GlossSee"), jr.getValue());
							ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::String, jr.GetTokenType()); ASSERT_EQ(_T("markup"), jr.getValue());
						ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::EndObject, jr.GetTokenType());
					ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::EndObject, jr.GetTokenType());
				ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::EndObject, jr.GetTokenType());
			ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::EndObject, jr.GetTokenType());
		ASSERT_EQ(true, jr.read()); ASSERT_EQ(JsonToken::EndObject, jr.GetTokenType());
		ASSERT_EQ(false, jr.read());
	}
	// <Integrate> オフィシャルページのサンプル2
	{
		StreamReader r(LN_LOCALFILE(_T("TestData/JSONExample2.json")));
		JsonReader2 jr(&r);
		while (jr.read());
		ASSERT_EQ(JsonParseError2::NoError, jr.GetError().code);
		ASSERT_EQ(true, r.isEOF());
	}
	// <Integrate> オフィシャルページのサンプル3
	{
		StreamReader r(LN_LOCALFILE(_T("TestData/JSONExample3.json")));
		JsonReader2 jr(&r);
		while (jr.read());
		ASSERT_EQ(JsonParseError2::NoError, jr.GetError().code);
		ASSERT_EQ(true, r.isEOF());
	}
	// <Integrate> オフィシャルページのサンプル4
	{
		StreamReader r(LN_LOCALFILE(_T("TestData/JSONExample4.json")));
		JsonReader2 jr(&r);
		while (jr.read());
		ASSERT_EQ(JsonParseError2::NoError, jr.GetError().code);
		ASSERT_EQ(true, r.isEOF());
	}
	// <Integrate> オフィシャルページのサンプル5
	{
		StreamReader r(LN_LOCALFILE(_T("TestData/JSONExample5.json")));
		JsonReader2 jr(&r);
		while (jr.read());
		ASSERT_EQ(JsonParseError2::NoError, jr.GetError().code);
		ASSERT_EQ(true, r.isEOF());
	}
}

//---------------------------------------------------------------------
TEST_F(Test_Json_JsonReader, Error)
{
	// <Test> UnterminatedString
	{
		JsonReader2 reader(_T("{\""));
		while (reader.TryRead());
		ASSERT_EQ(JsonParseError2::UnterminatedString, reader.GetError().code);
	}
	// <Test> InvalidStringChar
	{
		JsonReader2 reader(_T("{\"\a\"}"));
		while (reader.TryRead());
		ASSERT_EQ(JsonParseError2::InvalidStringChar, reader.GetError().code);
	}
	// <Test> InvalidStringEscape
	{
		JsonReader2 reader(_T("{\"\\a\"}"));
		while (reader.TryRead());
		ASSERT_EQ(JsonParseError2::InvalidStringEscape, reader.GetError().code);
	}
	// <Test> InvalidObjectClosing
	{
		JsonReader2 reader(_T("{\"\":\"\",}"));
		while (reader.TryRead());
		ASSERT_EQ(JsonParseError2::InvalidObjectClosing, reader.GetError().code);
		ASSERT_EQ(7, reader.GetError().column);	// } の位置
	}
	// <Test> ArrayInvalidClosing
	{
		JsonReader2 reader(_T("[\"\",]"));
		while (reader.TryRead());
		ASSERT_EQ(JsonParseError2::ArrayInvalidClosing, reader.GetError().code);
		ASSERT_EQ(4, reader.GetError().column);	// ] の位置
	}
	// <Test> ValueInvalid
	{
		JsonReader2 reader(_T("[n]"));
		while (reader.TryRead());
		ASSERT_EQ(JsonParseError2::ValueInvalid, reader.GetError().code);
		JsonReader2 reader2(_T("[t]"));
		while (reader2.TryRead());
		ASSERT_EQ(JsonParseError2::ValueInvalid, reader.GetError().code);
		JsonReader2 reader3(_T("[f]"));
		while (reader3.TryRead());
		ASSERT_EQ(JsonParseError2::ValueInvalid, reader.GetError().code);
	}
}

//---------------------------------------------------------------------
TEST_F(Test_Json_JsonReader, Issues)
{
	// <Issue> ネストされた配列の終端で解析が終了してしまう。
	{
		StringReader r(_T(R"([{"tags": ["dolor"]},"end"])"));
		JsonReader2 jr(&r);
		while (jr.read());
		ASSERT_EQ(JsonParseError2::NoError, jr.GetError().code);
		ASSERT_EQ(true, r.isEOF());
	}
	// <Issue> プロパティの値が配列だと、次のプロパティの解析で終了してしまう。
	{
		StringReader r(_T(R"({"tags": [],"friends":"10"})"));
		JsonReader2 jr(&r);
		while (jr.read());
		ASSERT_EQ(JsonParseError2::NoError, jr.GetError().code);
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
