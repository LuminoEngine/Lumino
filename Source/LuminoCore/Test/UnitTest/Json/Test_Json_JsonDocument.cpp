#include <TestConfig.h>
#include <Lumino/Json/JsonDocument.h>

class Test_Json_JsonDocument : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//---------------------------------------------------------------------
TEST_F(Test_Json_JsonDocument, Basic)
{
	//const char* json = R"( { "hello" : "world", "t" : true , "f" : false, "n": null, "i":123, "pi": 3.1416, "a":[1, 2, 3, 4] } )";
	////String str = FileSystem::ReadAllText(LN_LOCALFILE("TestData/ReaderTest1.txt"), Encoding::getSystemMultiByteEncoding());
	//tr::JsonDocument2 doc;

	//////ElapsedTimer t;
	//////t.Start();
	//doc.Parse(json);

	////printf("%d", t.getElapsedTime());

	//printf("");
}

//---------------------------------------------------------------------
TEST_F(Test_Json_JsonDocument, save)
{
	tr::JsonDocument2 doc;
	doc.addMemberBool(_TT("b1"), true);
	doc.addMemberBool(_TT("b2"), false);
	doc.addMemberInt32(_TT("i32"), 1);
	doc.addMemberInt64(_TT("i64"), 2147483648LL);
	doc.addMemberFloat(_TT("f"), 1.0f);
	doc.addMemberDouble(_TT("dbl"), 1.0);
	doc.addMemberString(_TT("str"), _TT("test"));

	auto* ary = doc.addMemberArray(_TT("ary"));
	ary->addBool(true);
	ary->addInt32(200);

	auto* obj = doc.addMemberObject(_TT("obj"));
	obj->addMemberInt32(_TT("i32_2"), 100);

	auto* aryary = doc.addMemberArray(_TT("aryary"));
	aryary->addArray()->addInt32(111);
	aryary->addArray()->addInt32(222);

	auto* objary = doc.addMemberArray(_TT("objary"));
	objary->addObject()->addMemberInt32(_TT("int32"), 300);
	objary->addObject()->addMemberString(_TT("str1"), _TT("test2"));

	doc.save(TEMPFILE("json.txt"), tr::JsonFormatting::Indented);

	ASSERT_TRUE(TestHelper::equalFiles(TEMPFILE("json.txt"), LN_LOCALFILE("TestData/Test_Json_JsonDocument_Save1.json")));

}

//---------------------------------------------------------------------
TEST_F(Test_Json_JsonDocument, load)
{
	tr::JsonDocument2 doc;
	doc.load(LN_LOCALFILE("TestData/Test_Json_JsonDocument_Save1.json"));
	doc.save(TEMPFILE("json2.txt"), tr::JsonFormatting::Indented);
}

