
class Test_Struct : public ::testing::Test {};

#define TEST_ROOT u"C:/Proj/LN/Lumino/"

TEST_F(Test_Struct, Basic)
{
    auto diag = ln::makeObject<ln::DiagnosticsManager>();
    auto db = ln::makeRef<SymbolDatabase>(diag);
    HeaderParser parser;
    parser.addIncludePath(TEST_ROOT "include");
    parser.addIncludePath(TEST_ROOT "include");
    parser.addForceIncludeFile(TEST_ROOT "include/LuminoCore/Base/Common.hpp");
    parser.parse(LN_LOCALFILE("Assets/Struct/TestStruct.hpp"), db, diag);
    db->Link();

	auto struct1 = db->findSymbol(u"ln::TestStruct1");
    ASSERT_EQ(u"float", struct1->declaredFields[0]->type->fullName());
    ASSERT_EQ(u"x", struct1->declaredFields[0]->name);
    ASSERT_EQ(u"float", struct1->declaredFields[1]->type->fullName());
    ASSERT_EQ(u"y", struct1->declaredFields[1]->name);
}
