#include <TestConfig.h>
#include <Lumino/IO/FileStream.h>
#include <Lumino/IO/BinaryReader.h>

class Test_IO_BinaryReader : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_IO_BinaryReader, Basic)
{
	RefPtr<FileStream> file = FileStream::Create(LN_LOCALFILE("TestData/BinaryReaderTest.dat"), FileOpenMode::Read);
	BinaryReader reader(file);

	// 1byte
	ASSERT_EQ(17, reader.ReadInt8());
	ASSERT_EQ(0xFF, reader.ReadUInt8());

	// 2byte
	ASSERT_EQ(255, reader.ReadInt16(ByteOrder::Little));
	ASSERT_EQ(255, reader.ReadInt16(ByteOrder::Big));
	reader.Seek(-4);
	ASSERT_EQ(0x00FF, reader.ReadUInt16(ByteOrder::Little));
	ASSERT_EQ(0x00FF, reader.ReadUInt16(ByteOrder::Big));

	// 4byte
	ASSERT_EQ(285212927, reader.ReadInt32(ByteOrder::Little));
	ASSERT_EQ(285212927, reader.ReadInt32(ByteOrder::Big));
	reader.Seek(-8);
	ASSERT_EQ(0x110000FF, reader.ReadUInt32(ByteOrder::Little));
	ASSERT_EQ(0x110000FF, reader.ReadUInt32(ByteOrder::Big));

	// テストファイルをバイナリエディタで見やすくするために2バイト余分にある
	byte_t buf[2];
	reader.Read(buf, 2);
	ASSERT_EQ(0x22, buf[0]);
	ASSERT_EQ(0x22, buf[1]);
	ASSERT_EQ(16, reader.GetPosition());
	ASSERT_FALSE(reader.IsEOF());

	// 16byte
	ASSERT_EQ(1224979098644775167ULL, reader.ReadInt64(ByteOrder::Little));
	ASSERT_EQ(1224979098644775167ULL, reader.ReadInt64(ByteOrder::Big));
	reader.Seek(-16);
	ASSERT_EQ(0x11000000000000FFULL, reader.ReadUInt64(ByteOrder::Little));
	ASSERT_EQ(0x11000000000000FFULL, reader.ReadUInt64(ByteOrder::Big));

	// ファイル終端
	ASSERT_TRUE(reader.IsEOF());
}

//------------------------------------------------------------------------------
TEST_F(Test_IO_BinaryReader, BytesSpecified)
{
	RefPtr<FileStream> file = FileStream::Create(LN_LOCALFILE("TestData/BinaryReaderTest.dat"), FileOpenMode::Read);
	BinaryReader reader(file);

	// 1byte
	ASSERT_EQ(17, reader.ReadInt(1));
	ASSERT_EQ(0xFF, reader.ReadUInt(1));

	// 2byte
	ASSERT_EQ(255, reader.ReadInt(2, ByteOrder::Little));
	ASSERT_EQ(255, reader.ReadInt(2, ByteOrder::Big));
	reader.Seek(-4);
	ASSERT_EQ(0x00FF, reader.ReadUInt(2, ByteOrder::Little));
	ASSERT_EQ(0x00FF, reader.ReadUInt(2, ByteOrder::Big));

	// 4byte
	ASSERT_EQ(285212927, reader.ReadInt(4, ByteOrder::Little));
	ASSERT_EQ(285212927, reader.ReadInt(4, ByteOrder::Big));
	reader.Seek(-8);
	ASSERT_EQ(0x110000FF, reader.ReadUInt(4, ByteOrder::Little));
	ASSERT_EQ(0x110000FF, reader.ReadUInt(4, ByteOrder::Big));

	// テストファイルをバイナリエディタで見やすくするために2バイト余分にある
	byte_t buf[2];
	reader.Read(buf, 2);
	ASSERT_EQ(0x22, buf[0]);
	ASSERT_EQ(0x22, buf[1]);
	ASSERT_EQ(16, reader.GetPosition());
	ASSERT_FALSE(reader.IsEOF());

	// 16byte
	ASSERT_EQ(1224979098644775167ULL, reader.ReadInt(8, ByteOrder::Little));
	ASSERT_EQ(1224979098644775167ULL, reader.ReadInt(8, ByteOrder::Big));
	reader.Seek(-16);
	ASSERT_EQ(0x11000000000000FFULL, reader.ReadUInt(8, ByteOrder::Little));
	ASSERT_EQ(0x11000000000000FFULL, reader.ReadUInt(8, ByteOrder::Big));

	// ファイル終端
	ASSERT_TRUE(reader.IsEOF());
}