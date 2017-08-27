#include <TestConfig.h>
#include <Lumino/IO/FileSystem.h>
#include <Lumino/Text/EncodingDetector.h>

#ifdef LN_USTRING
#else
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
	ASSERT_EQ(EncodingType::UTF8, detector.detect(buf1.getData(), buf1.getSize()));
	ASSERT_EQ(EncodingType::UTF16L, detector.detect(buf2.getData(), buf2.getSize()));
	ASSERT_EQ(EncodingType::UTF16B, detector.detect(buf3.getData(), buf3.getSize()));
	ASSERT_EQ(EncodingType::UTF32L, detector.detect(buf4.getData(), buf4.getSize()));
	ASSERT_EQ(EncodingType::UTF32B, detector.detect(buf5.getData(), buf5.getSize()));
}

//---------------------------------------------------------------------
TEST_F(Test_Text_EncodingDetector, SJIS)
{
	EncodingDetector detector;
	ByteBuffer buf1(FileSystem::readAllBytes(LN_LOCALFILE("TestData/ConvertTable_SJIS_test.txt")));
	detector.detect(buf1.getData(), buf1.getSize());
	ASSERT_EQ(EncodingType::SJIS, detector.getEncodingType());

}
#endif
