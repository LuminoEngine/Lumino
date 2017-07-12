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
	RefPtr<FileStream> file = FileStream::create(LN_LOCALFILE("TestData/BinaryReaderTest.dat"), FileOpenMode::read);
	BinaryReader reader(file);

	// 1byte
	ASSERT_EQ(17, reader.readInt8());
	ASSERT_EQ(0xFF, reader.readUInt8());

	// 2byte
	ASSERT_EQ(255, reader.readInt16(ByteOrder::Little));
	ASSERT_EQ(255, reader.readInt16(ByteOrder::Big));
	reader.seek(-4);
	ASSERT_EQ(0x00FF, reader.readUInt16(ByteOrder::Little));
	ASSERT_EQ(0x00FF, reader.readUInt16(ByteOrder::Big));

	// 4byte
	ASSERT_EQ(285212927, reader.readInt32(ByteOrder::Little));
	ASSERT_EQ(285212927, reader.readInt32(ByteOrder::Big));
	reader.seek(-8);
	ASSERT_EQ(0x110000FF, reader.readUInt32(ByteOrder::Little));
	ASSERT_EQ(0x110000FF, reader.readUInt32(ByteOrder::Big));

	// テストファイルをバイナリエディタで見やすくするために2バイト余分にある
	byte_t buf[2];
	reader.read(buf, 2);
	ASSERT_EQ(0x22, buf[0]);
	ASSERT_EQ(0x22, buf[1]);
	ASSERT_EQ(16, reader.getPosition());
	ASSERT_FALSE(reader.isEOF());

	// 16byte
	ASSERT_EQ(1224979098644775167ULL, reader.readInt64(ByteOrder::Little));
	ASSERT_EQ(1224979098644775167ULL, reader.readInt64(ByteOrder::Big));
	reader.seek(-16);
	ASSERT_EQ(0x11000000000000FFULL, reader.readUInt64(ByteOrder::Little));
	ASSERT_EQ(0x11000000000000FFULL, reader.readUInt64(ByteOrder::Big));

	// ファイル終端
	ASSERT_TRUE(reader.isEOF());
}

//------------------------------------------------------------------------------
TEST_F(Test_IO_BinaryReader, BytesSpecified)
{
	RefPtr<FileStream> file = FileStream::create(LN_LOCALFILE("TestData/BinaryReaderTest.dat"), FileOpenMode::read);
	BinaryReader reader(file);

	// 1byte
	ASSERT_EQ(17, reader.readInt(1));
	ASSERT_EQ(0xFF, reader.readUInt(1));

	// 2byte
	ASSERT_EQ(255, reader.readInt(2, ByteOrder::Little));
	ASSERT_EQ(255, reader.readInt(2, ByteOrder::Big));
	reader.seek(-4);
	ASSERT_EQ(0x00FF, reader.readUInt(2, ByteOrder::Little));
	ASSERT_EQ(0x00FF, reader.readUInt(2, ByteOrder::Big));

	// 4byte
	ASSERT_EQ(285212927, reader.readInt(4, ByteOrder::Little));
	ASSERT_EQ(285212927, reader.readInt(4, ByteOrder::Big));
	reader.seek(-8);
	ASSERT_EQ(0x110000FF, reader.readUInt(4, ByteOrder::Little));
	ASSERT_EQ(0x110000FF, reader.readUInt(4, ByteOrder::Big));

	// テストファイルをバイナリエディタで見やすくするために2バイト余分にある
	byte_t buf[2];
	reader.read(buf, 2);
	ASSERT_EQ(0x22, buf[0]);
	ASSERT_EQ(0x22, buf[1]);
	ASSERT_EQ(16, reader.getPosition());
	ASSERT_FALSE(reader.isEOF());

	// 16byte
	ASSERT_EQ(1224979098644775167ULL, reader.readInt(8, ByteOrder::Little));
	ASSERT_EQ(1224979098644775167ULL, reader.readInt(8, ByteOrder::Big));
	reader.seek(-16);
	ASSERT_EQ(0x11000000000000FFULL, reader.readUInt(8, ByteOrder::Little));
	ASSERT_EQ(0x11000000000000FFULL, reader.readUInt(8, ByteOrder::Big));

	// ファイル終端
	ASSERT_TRUE(reader.isEOF());
}