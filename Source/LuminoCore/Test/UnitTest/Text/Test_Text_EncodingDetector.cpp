#include <TestConfig.h>
#include <Lumino/IO/FileSystem.h>
#include <Lumino/Text/EncodingDetector.h>

class Test_Text_EncodingDetector : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//---------------------------------------------------------------------
TEST_F(Test_Text_EncodingDetector, UTF8BOM)
{
	EncodingDetector detector;
	ByteBuffer buf1(FileSystem::readAllBytes(LN_LOCALFILE("TestData/UTF8BOM.txt")));
	ByteBuffer buf2(FileSystem::readAllBytes(LN_LOCALFILE("TestData/UTF16LittleBOM.txt")));
	ByteBuffer buf3(FileSystem::readAllBytes(LN_LOCALFILE("TestData/UTF16BigBOM.txt")));
	ByteBuffer buf4(FileSystem::readAllBytes(LN_LOCALFILE("TestData/UTF32LittleBOM.txt")));
	ByteBuffer buf5(FileSystem::readAllBytes(LN_LOCALFILE("TestData/UTF32BigBOM.txt")));
	ASSERT_EQ(EncodingType::UTF8, detector.Detect(buf1.getData(), buf1.getSize()));
	ASSERT_EQ(EncodingType::UTF16L, detector.Detect(buf2.getData(), buf2.getSize()));
	ASSERT_EQ(EncodingType::UTF16B, detector.Detect(buf3.getData(), buf3.getSize()));
	ASSERT_EQ(EncodingType::UTF32L, detector.Detect(buf4.getData(), buf4.getSize()));
	ASSERT_EQ(EncodingType::UTF32B, detector.Detect(buf5.getData(), buf5.getSize()));
}

//---------------------------------------------------------------------
TEST_F(Test_Text_EncodingDetector, SJIS)
{
	EncodingDetector detector;
	ByteBuffer buf1(FileSystem::readAllBytes(LN_LOCALFILE("TestData/ConvertTable_SJIS_test.txt")));
	detector.Detect(buf1.getData(), buf1.getSize());
	ASSERT_EQ(EncodingType::SJIS, detector.GetEncodingType());

}
