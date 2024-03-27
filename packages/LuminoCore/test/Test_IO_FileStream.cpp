#include "Common.hpp"
#include <LuminoCore/IO/FileSystem.hpp>
#include <LuminoCore/IO/FileStream.hpp>

//==============================================================================
//# fileIO test by FileStream
class Test_IO_FileStream : public ::testing::Test { };

//------------------------------------------------------------------------------
//## check conflict with FileSystem function
TEST_F(Test_IO_FileStream, getFileSize)
{
	Path filePath = LN_TEMPFILE("Test_IO_FileStream1");

	byte_t data[] = { 1, 2, 3 };
	FileSystem::writeAllBytes(filePath, data, 3);

	//* [ ] ファイルサイズを取得できる。
	{
		ASSERT_EQ(3, FileSystem::getFileSize(filePath));
	}

	//* [ ] ファイルが開かれていてもサイズを取得できる。
	{
		auto f = FileStream::create(filePath, FileOpenMode::Write | FileOpenMode::Append);
		ASSERT_EQ(3, FileSystem::getFileSize(filePath));
	}
}

//------------------------------------------------------------------------------
//## open mode test
TEST_F(Test_IO_FileStream, open)
{
	Path filePath = LN_TEMPFILE("Test_IO_FileStream1");

	byte_t data[] = { 1, 2, 3 };
	FileSystem::writeAllBytes(filePath, data, 3);

	// Write
	{
		auto file1 = FileStream::create(filePath, FileOpenMode::Write);
		ASSERT_EQ(true, valid(file1));
		ASSERT_EQ(0, file1->length());	// 空ファイルになる
		file1->write("A", 1);
		ASSERT_EQ(1, file1->length());
	}
	// Write | Truncate (↑と同じ動作)
	{
		auto file1 = FileStream::create(filePath, FileOpenMode::Write | FileOpenMode::Truncate);
		ASSERT_EQ(0, file1->length());	// 空ファイルになる
		file1->write("B", 1);
		ASSERT_EQ(1, file1->length());
	}
	// Write | Append
	{
		auto file1 = FileStream::create(filePath, FileOpenMode::Write | FileOpenMode::Append);
		ASSERT_EQ(1, file1->length());	// "B" が書かれている
		file1->write("C", 1);
		ASSERT_EQ(2, file1->length());
		file1->close();

		auto data = FileSystem::readAllBytes(filePath).unwrap();
		ASSERT_EQ(2, data.size());
		ASSERT_EQ('B', data[0]);
		ASSERT_EQ('C', data[1]);
	}
	// ReadWrite
	{
		auto file1 = FileStream::create(filePath, FileOpenMode::ReadWrite);
		ASSERT_EQ(2, file1->length());	// "BC" が書かれている
		char buf[2];
		file1->read(buf, 2);
		ASSERT_EQ('B', buf[0]);
		ASSERT_EQ('C', buf[1]);
		file1->seek(0, SeekOrigin::Begin);
		file1->write("D", 1);
		ASSERT_EQ(2, file1->length());	// "DC" が書かれている
	}
	// ReadWrite | Append
	{
		auto file1 = FileStream::create(filePath, FileOpenMode::ReadWrite | FileOpenMode::Append);
		ASSERT_EQ(2, file1->length());	// "DC" が書かれている
		file1->write("E", 1);
		file1->seek(0, SeekOrigin::Begin);
		char buf[4] = { 0 };
		file1->read(buf, 3);
		ASSERT_STREQ("DCE", buf);
	}
	// ReadWrite | Truncate
	{
		auto file1 = FileStream::create(filePath, FileOpenMode::ReadWrite | FileOpenMode::Truncate);
		ASSERT_EQ(0, file1->length());	// 空ファイルになる
		file1->write("F", 1);
		file1->seek(0, SeekOrigin::Begin);
		char buf[2] = { 0 };
		file1->read(buf, 1);
		ASSERT_STREQ("F", buf);
	}
	// Read | Truncate
	{
		auto file1 = FileStream::create(filePath, FileOpenMode::Read);
		char buf[2] = { 0 };
		file1->read(buf, 1);
		ASSERT_STREQ("F", buf);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_IO_FileStream, FileOpenMode_Deferring)
{
	Path filePath = LN_TEMPFILE("Test_IO_FileStream1");

	// Write
	{
		auto file1 = FileStream::create(filePath, FileOpenMode::Write | FileOpenMode::Deferring);
		ASSERT_EQ(0, file1->length());	// 空ファイルになる
		file1->write("A", 1);
	}
	// Write | Truncate (↑と同じ動作)
	{
		auto file1 = FileStream::create(filePath, FileOpenMode::Write | FileOpenMode::Truncate | FileOpenMode::Deferring);
		ASSERT_EQ(0, file1->length());	// 空ファイルになる
		file1->write("B", 1);
	}
	// Write | Append
	{
		auto file1 = FileStream::create(filePath, FileOpenMode::Write | FileOpenMode::Append | FileOpenMode::Deferring);
		ASSERT_EQ(1, file1->length());	// "B" が書かれている
		file1->write("C", 1);
	}
	// ReadWrite
	{
		auto file1 = FileStream::create(filePath, FileOpenMode::ReadWrite | FileOpenMode::Deferring);
		ASSERT_EQ(2, file1->length());	// "BC" が書かれている
		char buf[2];
		file1->read(buf, 2);
		ASSERT_EQ('B', buf[0]);
		ASSERT_EQ('C', buf[1]);
		file1->seek(0, SeekOrigin::Begin);
		file1->write("D", 1);
		ASSERT_EQ(2, file1->length());	// "DC" が書かれている
	}
	// ReadWrite | Append
	{
		auto file1 = FileStream::create(filePath, FileOpenMode::ReadWrite | FileOpenMode::Append | FileOpenMode::Deferring);
		ASSERT_EQ(2, file1->length());	// "DC" が書かれている
		file1->write("E", 1);
		file1->seek(0, SeekOrigin::Begin);
		char buf[4] = { 0 };
		file1->read(buf, 3);
		ASSERT_STREQ("DCE", buf);
	}
	// ReadWrite | Truncate
	{
		auto file1 = FileStream::create(filePath, FileOpenMode::ReadWrite | FileOpenMode::Truncate | FileOpenMode::Deferring);
		ASSERT_EQ(0, file1->length());	// 空ファイルになる
		file1->write("F", 1);
		file1->seek(0, SeekOrigin::Begin);
		char buf[2] = { 0 };
		file1->read(buf, 1);
		ASSERT_STREQ("F", buf);
	}
	// Read | Truncate
	{
		auto file1 = FileStream::create(filePath, FileOpenMode::Read | FileOpenMode::Deferring);
		char buf[2] = { 0 };
		file1->read(buf, 1);
		ASSERT_STREQ("F", buf);
	}
}
