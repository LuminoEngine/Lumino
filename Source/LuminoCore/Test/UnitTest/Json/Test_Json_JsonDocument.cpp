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
	doc.addMemberBool(_T("b1"), true);
	doc.addMemberBool(_T("b2"), false);
	doc.addMemberInt32(_T("i32"), 1);
	doc.addMemberInt64(_T("i64"), 2147483648LL);
	doc.addMemberFloat(_T("f"), 1.0f);
	doc.addMemberDouble(_T("dbl"), 1.0);
	doc.addMemberString(_T("str"), _T("test"));

	auto* ary = doc.addMemberArray(_T("ary"));
	ary->addBool(true);
	ary->addInt32(200);

	auto* obj = doc.addMemberObject(_T("obj"));
	obj->addMemberInt32(_T("i32_2"), 100);

	auto* aryary = doc.addMemberArray(_T("aryary"));
	aryary->addArray()->addInt32(111);
	aryary->addArray()->addInt32(222);

	auto* objary = doc.addMemberArray(_T("objary"));
	objary->addObject()->addMemberInt32(_T("int32"), 300);
	objary->addObject()->addMemberString(_T("str1"), _T("test2"));

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

