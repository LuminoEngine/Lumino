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
TEST_F(Test_IO_FileStream, GetFileSize)
{
	// <Test> ファイルサイズを取得できる。
	{
		ASSERT_EQ(1036, FileSystem::GetFileSize(LN_LOCALFILE("TestData/Text_SJIS_CRLF.txt")));
	}

	// <Test> ファイルが開かれていてもサイズを取得できる。
	{
		auto f = FileStream::Create(LN_LOCALFILE("TestData/Text_SJIS_CRLF.txt"), FileOpenMode::Write | FileOpenMode::Append);
		ASSERT_EQ(1036, FileSystem::GetFileSize(LN_LOCALFILE("TestData/Text_SJIS_CRLF.txt")));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_IO_FileStream, Open)
{
	// Write
	{
		auto file1 = FileStream::Create(LN_LOCALFILE("TestData/Test_IO_FileStream1.txt"), FileOpenMode::Write);
		ASSERT_EQ(0, file1->GetLength());	// 空ファイルになる
		file1->Write("A", 1);
	}
	// Write | Truncate (↑と同じ動作)
	{
		auto file1 = FileStream::Create(LN_LOCALFILE("TestData/Test_IO_FileStream1.txt"), FileOpenMode::Write | FileOpenMode::Truncate);
		ASSERT_EQ(0, file1->GetLength());	// 空ファイルになる
		file1->Write("B", 1);
	}
	// Write | Append
	{
		auto file1 = FileStream::Create(LN_LOCALFILE("TestData/Test_IO_FileStream1.txt"), FileOpenMode::Write | FileOpenMode::Append);
		ASSERT_EQ(1, file1->GetLength());	// "B" が書かれている
		file1->Write("C", 1);
	}
	// ReadWrite
	{
		auto file1 = FileStream::Create(LN_LOCALFILE("TestData/Test_IO_FileStream1.txt"), FileOpenMode::ReadWrite);
		ASSERT_EQ(2, file1->GetLength());	// "BC" が書かれている
		char buf[2];
		file1->Read(buf, 2);
		ASSERT_EQ('B', buf[0]);
		ASSERT_EQ('C', buf[1]);
		file1->Seek(0, SeekOrigin_Begin);
		file1->Write("D", 1);
		ASSERT_EQ(2, file1->GetLength());	// "DC" が書かれている
	}
	// ReadWrite | Append
	{
		auto file1 = FileStream::Create(LN_LOCALFILE("TestData/Test_IO_FileStream1.txt"), FileOpenMode::ReadWrite | FileOpenMode::Append);
		ASSERT_EQ(2, file1->GetLength());	// "DC" が書かれている
		file1->Write("E", 1);
		file1->Seek(0, SeekOrigin_Begin);
		char buf[4] = { 0 };
		file1->Read(buf, 3);
		ASSERT_STREQ("DCE", buf);
	}
	// ReadWrite | Truncate
	{
		auto file1 = FileStream::Create(LN_LOCALFILE("TestData/Test_IO_FileStream1.txt"), FileOpenMode::ReadWrite | FileOpenMode::Truncate);
		ASSERT_EQ(0, file1->GetLength());	// 空ファイルになる
		file1->Write("F", 1);
		file1->Seek(0, SeekOrigin_Begin);
		char buf[2] = { 0 };
		file1->Read(buf, 1);
		ASSERT_STREQ("F", buf);
	}
	// Read | Truncate
	{
		auto file1 = FileStream::Create(LN_LOCALFILE("TestData/Test_IO_FileStream1.txt"), FileOpenMode::Read);
		char buf[2] = { 0 };
		file1->Read(buf, 1);
		ASSERT_STREQ("F", buf);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_IO_FileStream, FileOpenMode_Deferring)
{
	// Write
	{
		auto file1 = FileStream::Create(LN_LOCALFILE("TestData/Test_IO_FileStream1.txt"), FileOpenMode::Write | FileOpenMode::Deferring);
		ASSERT_EQ(0, file1->GetLength());	// 空ファイルになる
		file1->Write("A", 1);
	}
	// Write | Truncate (↑と同じ動作)
	{
		auto file1 = FileStream::Create(LN_LOCALFILE("TestData/Test_IO_FileStream1.txt"), FileOpenMode::Write | FileOpenMode::Truncate | FileOpenMode::Deferring);
		ASSERT_EQ(0, file1->GetLength());	// 空ファイルになる
		file1->Write("B", 1);
	}
	// Write | Append
	{
		auto file1 = FileStream::Create(LN_LOCALFILE("TestData/Test_IO_FileStream1.txt"), FileOpenMode::Write | FileOpenMode::Append | FileOpenMode::Deferring);
		ASSERT_EQ(1, file1->GetLength());	// "B" が書かれている
		file1->Write("C", 1);
	}
	// ReadWrite
	{
		auto file1 = FileStream::Create(LN_LOCALFILE("TestData/Test_IO_FileStream1.txt"), FileOpenMode::ReadWrite | FileOpenMode::Deferring);
		ASSERT_EQ(2, file1->GetLength());	// "BC" が書かれている
		char buf[2];
		file1->Read(buf, 2);
		ASSERT_EQ('B', buf[0]);
		ASSERT_EQ('C', buf[1]);
		file1->Seek(0, SeekOrigin_Begin);
		file1->Write("D", 1);
		ASSERT_EQ(2, file1->GetLength());	// "DC" が書かれている
	}
	// ReadWrite | Append
	{
		auto file1 = FileStream::Create(LN_LOCALFILE("TestData/Test_IO_FileStream1.txt"), FileOpenMode::ReadWrite | FileOpenMode::Append | FileOpenMode::Deferring);
		ASSERT_EQ(2, file1->GetLength());	// "DC" が書かれている
		file1->Write("E", 1);
		file1->Seek(0, SeekOrigin_Begin);
		char buf[4] = { 0 };
		file1->Read(buf, 3);
		ASSERT_STREQ("DCE", buf);
	}
	// ReadWrite | Truncate
	{
		auto file1 = FileStream::Create(LN_LOCALFILE("TestData/Test_IO_FileStream1.txt"), FileOpenMode::ReadWrite | FileOpenMode::Truncate | FileOpenMode::Deferring);
		ASSERT_EQ(0, file1->GetLength());	// 空ファイルになる
		file1->Write("F", 1);
		file1->Seek(0, SeekOrigin_Begin);
		char buf[2] = { 0 };
		file1->Read(buf, 1);
		ASSERT_STREQ("F", buf);
	}
	// Read | Truncate
	{
		auto file1 = FileStream::Create(LN_LOCALFILE("TestData/Test_IO_FileStream1.txt"), FileOpenMode::Read | FileOpenMode::Deferring);
		char buf[2] = { 0 };
		file1->Read(buf, 1);
		ASSERT_STREQ("F", buf);
	}
}
