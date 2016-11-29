#include <TestConfig.h>
#include "../../../src/IO/ArchiveMaker.h"
#include "../../../src/IO/Archive.h"

#if 0//defined(LN_OS_WIN32) && defined(LN_UNICODE)

class Test_IO_ArchiveMaker : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};
//------------------------------------------------------------------------------
TEST_F(Test_IO_ArchiveMaker, Basic)
{
	// アーカイブファイルを作る
	ArchiveMaker maker;
	maker.Open(LN_LOCALFILE("Test1.lna"), "test");
	maker.AddFile(LN_LOCALFILE("TestData/ArchiveText1_0byte.txt"), L"ArchiveText1_0byte.txt");
	maker.AddFile(LN_LOCALFILE("TestData/ArchiveText1_1byte.txt"), L"ArchiveText1_1byte.txt");
	maker.AddFile(LN_LOCALFILE("TestData/ArchiveText1_15byte.txt"), L"ArchiveText1_15byte.txt");
	maker.AddFile(LN_LOCALFILE("TestData/ArchiveText1_16byte.txt"), L"ArchiveText1_16byte.txt");
	maker.AddFile(LN_LOCALFILE("TestData/ArchiveText1_17byte.txt"), L"ArchiveText1_17byte.txt");
	maker.AddFile(LN_LOCALFILE("TestData/ArchiveText1_32byte.txt"), L"ArchiveText1_32byte.txt");
	maker.AddFile(LN_LOCALFILE("TestData/ArchiveText1_33byte.txt"), L"ArchiveText1_33byte.txt");
	maker.Close();


	Archive archive;
	archive.Open(LN_LOCALFILE("Test1.lna"), _T("test"));

	// 存在チェック
	ASSERT_TRUE(archive.ExistsFile(LN_LOCALFILE("Test1/ArchiveText1_0byte.txt")));
	ASSERT_FALSE(archive.ExistsFile(LN_LOCALFILE("Test1/aaaa.txt")));

	// ArchiveText1_0byte.txt
	{
		RefPtr<Stream> file;
		archive.TryCreateStream(LN_LOCALFILE("Test1/ArchiveText1_0byte.txt"), &file, false);
		ASSERT_EQ(0, file->GetLength());
	}

	// ArchiveText1_1byte.txt
	{
		char buf[64] = { 0 };
		RefPtr<Stream> file;
		archive.TryCreateStream(LN_LOCALFILE("Test1/ArchiveText1_1byte.txt"), &file, false);
		ASSERT_EQ(1, file->GetLength());
		ASSERT_EQ(1, file->Read(buf, 1));
		ASSERT_STREQ("1", buf);
	}

	// ArchiveText1_15byte.txt
	{
		char buf[64] = { 0 };
		RefPtr<Stream> file;
		archive.TryCreateStream(LN_LOCALFILE("Test1/ArchiveText1_15byte.txt"), &file, false);
		ASSERT_EQ(15, file->GetLength());
		ASSERT_EQ(15, file->Read(buf, 15));
		ASSERT_STREQ("123456789012345", buf);
	}

	// ArchiveText1_16byte.txt
	{
		char buf[64] = { 0 };
		RefPtr<Stream> file;
		archive.TryCreateStream(LN_LOCALFILE("Test1/ArchiveText1_16byte.txt"), &file, false);
		ASSERT_EQ(16, file->GetLength());
		ASSERT_EQ(16, file->Read(buf, 16));
		ASSERT_STREQ("1234567890123456", buf);
	}

	// ArchiveText1_17byte.txt
	{
		char buf[64] = { 0 };
		RefPtr<Stream> file;
		archive.TryCreateStream(LN_LOCALFILE("Test1/ArchiveText1_17byte.txt"), &file, false);
		ASSERT_EQ(17, file->GetLength());
		ASSERT_EQ(17, file->Read(buf, 17));
		ASSERT_STREQ("12345678901234567", buf);
	}

	// ArchiveText1_32byte.txt
	{
		char buf[64] = { 0 };
		RefPtr<Stream> file;
		archive.TryCreateStream(LN_LOCALFILE("Test1/ArchiveText1_32byte.txt"), &file, false);
		ASSERT_EQ(32, file->GetLength());
		ASSERT_EQ(32, file->Read(buf, 32));
		ASSERT_STREQ("12345678901234567890123456789012", buf);
	}

	// ArchiveText1_33byte.txt
	{
		char buf[64] = { 0 };
		RefPtr<Stream> file;
		archive.TryCreateStream(LN_LOCALFILE("Test1/ArchiveText1_33byte.txt"), &file, false);
		ASSERT_EQ(33, file->GetLength());
		ASSERT_EQ(33, file->Read(buf, 33));
		ASSERT_STREQ("123456789012345678901234567890123", buf);
	}

	// ArchiveText1_33byte.txt  1 → 32 の分割
	{
		char buf[64] = { 0 };
		RefPtr<Stream> file;
		archive.TryCreateStream(LN_LOCALFILE("Test1/ArchiveText1_33byte.txt"), &file, false);
		ASSERT_EQ(1, file->Read(buf, 1));
		ASSERT_STREQ("1", buf);

		ASSERT_EQ(32, file->Read(buf, 32));
		ASSERT_STREQ("23456789012345678901234567890123", buf);
	}

	// ArchiveText1_33byte.txt  1byte ずつ
	{
		char buf[64] = { 0 };
		char* pos = buf;
		RefPtr<Stream> file;
		archive.TryCreateStream(LN_LOCALFILE("Test1/ArchiveText1_33byte.txt"), &file, false);

		for (int i = 0; i < 33; i++)
		{
			file->Read(pos, 1);
			++pos;
		}
		ASSERT_STREQ("123456789012345678901234567890123", buf);
	}
}

#endif
