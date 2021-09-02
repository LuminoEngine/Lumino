#include "Common.hpp"
#include <LuminoCore/IO/FileStream.hpp>
#include <LuminoCore/IO/BinaryReader.hpp>

class Test_IO_BinaryReader : public ::testing::Test {};

//------------------------------------------------------------------------------
TEST_F(Test_IO_BinaryReader, Basic)
{
	Ref<FileStream> file = FileStream::create(LN_ASSETFILE("BinaryReaderTest.dat"), FileOpenMode::Read);
	BinaryReader reader(file);

	// 1byte
	ASSERT_EQ(17, reader.readInt8());
	ASSERT_EQ(0xFF, reader.readUInt8());

	// 2byte
	ASSERT_EQ(255, reader.readInt16(ByteOrder::LittleEndian));
	ASSERT_EQ(255, reader.readInt16(ByteOrder::BigEndian));
	reader.seek(-4);
	ASSERT_EQ(0x00FF, reader.readUInt16(ByteOrder::LittleEndian));
	ASSERT_EQ(0x00FF, reader.readUInt16(ByteOrder::BigEndian));

	// 4byte
	ASSERT_EQ(285212927, reader.readInt32(ByteOrder::LittleEndian));
	ASSERT_EQ(285212927, reader.readInt32(ByteOrder::BigEndian));
	reader.seek(-8);
	ASSERT_EQ(0x110000FF, reader.readUInt32(ByteOrder::LittleEndian));
	ASSERT_EQ(0x110000FF, reader.readUInt32(ByteOrder::BigEndian));

	// テストファイルをバイナリエディタで見やすくするために2バイト余分にある
	byte_t buf[2];
	reader.read(buf, 2);
	ASSERT_EQ(0x22, buf[0]);
	ASSERT_EQ(0x22, buf[1]);

	// 16byte
	ASSERT_EQ(1224979098644775167ULL, reader.readInt64(ByteOrder::LittleEndian));
	ASSERT_EQ(1224979098644775167ULL, reader.readInt64(ByteOrder::BigEndian));
	reader.seek(-16);
	ASSERT_EQ(0x11000000000000FFULL, reader.readUInt64(ByteOrder::LittleEndian));
	ASSERT_EQ(0x11000000000000FFULL, reader.readUInt64(ByteOrder::BigEndian));
}

//------------------------------------------------------------------------------
TEST_F(Test_IO_BinaryReader, BytesSpecified)
{
	Ref<FileStream> file = FileStream::create(LN_ASSETFILE("BinaryReaderTest.dat"), FileOpenMode::Read);
	BinaryReader reader(file);

	// 1byte
	ASSERT_EQ(17, reader.readInt(1));
	ASSERT_EQ(0xFF, reader.readUInt(1));

	// 2byte
	ASSERT_EQ(255, reader.readInt(2, ByteOrder::LittleEndian));
	ASSERT_EQ(255, reader.readInt(2, ByteOrder::BigEndian));
	reader.seek(-4);
	ASSERT_EQ(0x00FF, reader.readUInt(2, ByteOrder::LittleEndian));
	ASSERT_EQ(0x00FF, reader.readUInt(2, ByteOrder::BigEndian));

	// 4byte
	ASSERT_EQ(285212927, reader.readInt(4, ByteOrder::LittleEndian));
	ASSERT_EQ(285212927, reader.readInt(4, ByteOrder::BigEndian));
	reader.seek(-8);
	ASSERT_EQ(0x110000FF, reader.readUInt(4, ByteOrder::LittleEndian));
	ASSERT_EQ(0x110000FF, reader.readUInt(4, ByteOrder::BigEndian));

	// テストファイルをバイナリエディタで見やすくするために2バイト余分にある
	byte_t buf[2];
	reader.read(buf, 2);
	ASSERT_EQ(0x22, buf[0]);
	ASSERT_EQ(0x22, buf[1]);

	// 16byte
	ASSERT_EQ(1224979098644775167ULL, reader.readInt(8, ByteOrder::LittleEndian));
	ASSERT_EQ(1224979098644775167ULL, reader.readInt(8, ByteOrder::BigEndian));
	reader.seek(-16);
	ASSERT_EQ(0x11000000000000FFULL, reader.readUInt(8, ByteOrder::LittleEndian));
	ASSERT_EQ(0x11000000000000FFULL, reader.readUInt(8, ByteOrder::BigEndian));
}

