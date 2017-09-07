#include <TestConfig.h>
#include <Lumino/IO/FileSystem.h>
#include <Lumino/IO/FileStream.h>

class Test_IO_FileStream : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_IO_FileStream, getFileSize)
{
	// <Test> ファイルサイズを取得できる。
	{
		ASSERT_EQ(1036, FileSystem::getFileSize(LN_LOCALFILE("TestData/Text_SJIS_CRLF.txt")));
	}

	// <Test> ファイルが開かれていてもサイズを取得できる。
	{
		auto f = FileStream::create(LN_LOCALFILE("TestData/Text_SJIS_CRLF.txt"), FileOpenMode::write | FileOpenMode::append);
		ASSERT_EQ(1036, FileSystem::getFileSize(LN_LOCALFILE("TestData/Text_SJIS_CRLF.txt")));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_IO_FileStream, open)
{
	// Write
	{
		auto file1 = FileStream::create(LN_LOCALFILE("TestData/Test_IO_FileStream1.txt"), FileOpenMode::write);
		ASSERT_EQ(0, file1->getLength());	// 空ファイルになる
		file1->write("A", 1);
	}
	// Write | Truncate (↑と同じ動作)
	{
		auto file1 = FileStream::create(LN_LOCALFILE("TestData/Test_IO_FileStream1.txt"), FileOpenMode::write | FileOpenMode::Truncate);
		ASSERT_EQ(0, file1->getLength());	// 空ファイルになる
		file1->write("B", 1);
	}
	// Write | Append
	{
		auto file1 = FileStream::create(LN_LOCALFILE("TestData/Test_IO_FileStream1.txt"), FileOpenMode::write | FileOpenMode::append);
		ASSERT_EQ(1, file1->getLength());	// "B" が書かれている
		file1->write("C", 1);
	}
	// ReadWrite
	{
		auto file1 = FileStream::create(LN_LOCALFILE("TestData/Test_IO_FileStream1.txt"), FileOpenMode::ReadWrite);
		ASSERT_EQ(2, file1->getLength());	// "BC" が書かれている
		char buf[2];
		file1->read(buf, 2);
		ASSERT_EQ('B', buf[0]);
		ASSERT_EQ('C', buf[1]);
		file1->seek(0, SeekOrigin_Begin);
		file1->write("D", 1);
		ASSERT_EQ(2, file1->getLength());	// "DC" が書かれている
	}
	// ReadWrite | Append
	{
		auto file1 = FileStream::create(LN_LOCALFILE("TestData/Test_IO_FileStream1.txt"), FileOpenMode::ReadWrite | FileOpenMode::append);
		ASSERT_EQ(2, file1->getLength());	// "DC" が書かれている
		file1->write("E", 1);
		file1->seek(0, SeekOrigin_Begin);
		char buf[4] = { 0 };
		file1->read(buf, 3);
		ASSERT_STREQ("DCE", buf);
	}
	// ReadWrite | Truncate
	{
		auto file1 = FileStream::create(LN_LOCALFILE("TestData/Test_IO_FileStream1.txt"), FileOpenMode::ReadWrite | FileOpenMode::Truncate);
		ASSERT_EQ(0, file1->getLength());	// 空ファイルになる
		file1->write("F", 1);
		file1->seek(0, SeekOrigin_Begin);
		char buf[2] = { 0 };
		file1->read(buf, 1);
		ASSERT_STREQ("F", buf);
	}
	// Read | Truncate
	{
		auto file1 = FileStream::create(LN_LOCALFILE("TestData/Test_IO_FileStream1.txt"), FileOpenMode::read);
		char buf[2] = { 0 };
		file1->read(buf, 1);
		ASSERT_STREQ("F", buf);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_IO_FileStream, FileOpenMode_Deferring)
{
	// Write
	{
		auto file1 = FileStream::create(LN_LOCALFILE("TestData/Test_IO_FileStream1.txt"), FileOpenMode::write | FileOpenMode::Deferring);
		ASSERT_EQ(0, file1->getLength());	// 空ファイルになる
		file1->write("A", 1);
	}
	// Write | Truncate (↑と同じ動作)
	{
		auto file1 = FileStream::create(LN_LOCALFILE("TestData/Test_IO_FileStream1.txt"), FileOpenMode::write | FileOpenMode::Truncate | FileOpenMode::Deferring);
		ASSERT_EQ(0, file1->getLength());	// 空ファイルになる
		file1->write("B", 1);
	}
	// Write | Append
	{
		auto file1 = FileStream::create(LN_LOCALFILE("TestData/Test_IO_FileStream1.txt"), FileOpenMode::write | FileOpenMode::append | FileOpenMode::Deferring);
		ASSERT_EQ(1, file1->getLength());	// "B" が書かれている
		file1->write("C", 1);
	}
	// ReadWrite
	{
		auto file1 = FileStream::create(LN_LOCALFILE("TestData/Test_IO_FileStream1.txt"), FileOpenMode::ReadWrite | FileOpenMode::Deferring);
		ASSERT_EQ(2, file1->getLength());	// "BC" が書かれている
		char buf[2];
		file1->read(buf, 2);
		ASSERT_EQ('B', buf[0]);
		ASSERT_EQ('C', buf[1]);
		file1->seek(0, SeekOrigin_Begin);
		file1->write("D", 1);
		ASSERT_EQ(2, file1->getLength());	// "DC" が書かれている
	}
	// ReadWrite | Append
	{
		auto file1 = FileStream::create(LN_LOCALFILE("TestData/Test_IO_FileStream1.txt"), FileOpenMode::ReadWrite | FileOpenMode::append | FileOpenMode::Deferring);
		ASSERT_EQ(2, file1->getLength());	// "DC" が書かれている
		file1->write("E", 1);
		file1->seek(0, SeekOrigin_Begin);
		char buf[4] = { 0 };
		file1->read(buf, 3);
		ASSERT_STREQ("DCE", buf);
	}
	// ReadWrite | Truncate
	{
		auto file1 = FileStream::create(LN_LOCALFILE("TestData/Test_IO_FileStream1.txt"), FileOpenMode::ReadWrite | FileOpenMode::Truncate | FileOpenMode::Deferring);
		ASSERT_EQ(0, file1->getLength());	// 空ファイルになる
		file1->write("F", 1);
		file1->seek(0, SeekOrigin_Begin);
		char buf[2] = { 0 };
		file1->read(buf, 1);
		ASSERT_STREQ("F", buf);
	}
	// Read | Truncate
	{
		auto file1 = FileStream::create(LN_LOCALFILE("TestData/Test_IO_FileStream1.txt"), FileOpenMode::read | FileOpenMode::Deferring);
		char buf[2] = { 0 };
		file1->read(buf, 1);
		ASSERT_STREQ("F", buf);
	}
}
