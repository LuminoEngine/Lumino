
#include "../Internal.h"
#include <Lumino/IO/Stream.h>
#include <Lumino/IO/BinaryReader.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// BinaryReader
//==============================================================================

//------------------------------------------------------------------------------
BinaryReader::BinaryReader(Stream* stream)
	: m_stream(NULL)
{
	if (LN_REQUIRE(stream)) return;
	LN_REFOBJ_SET(m_stream, stream);
}

//------------------------------------------------------------------------------
BinaryReader::~BinaryReader()
{
	LN_SAFE_RELEASE(m_stream);
}

//------------------------------------------------------------------------------
int8_t BinaryReader::readInt8()
{
	int8_t buffer;
	m_stream->read(&buffer, 1);
	return buffer;
}

//------------------------------------------------------------------------------
int16_t BinaryReader::readInt16(ByteOrder dataByteOrder)
{
	byte_t buffer[2];
	size_t count = m_stream->read(&buffer, 2);
	LN_THROW(count == 2, EndOfStreamException);

	// 現在の環境と同じエンディアンであればそのまま返す
	if (dataByteOrder == Environment::getByteOrder()) {
		return *((int16_t*)buffer);
	}
	// 異なるエンディアンであれば swap
	else
	{
		byte_t out[2], *p = (byte_t*)&buffer;
		out[1] = *p++;
		out[0] = *p++;
		return *((int16_t*)out);
	}
}

//------------------------------------------------------------------------------
int32_t BinaryReader::readInt32(ByteOrder dataByteOrder)
{
	byte_t buffer[4];
	size_t count = m_stream->read(&buffer, 4);
	if (LN_ENSURE(count == 4)) return;

	// 現在の環境と同じエンディアンであればそのまま返す
	if (dataByteOrder == Environment::getByteOrder()) {
		return *((int32_t*)buffer);
	}
	// 異なるエンディアンであれば swap
	else
	{
		byte_t out[4], *p = (byte_t*)&buffer;
		out[3] = *p++;
		out[2] = *p++;
		out[1] = *p++;
		out[0] = *p++;
		return *((int32_t*)out);
	}
}

//------------------------------------------------------------------------------
int64_t BinaryReader::readInt64(ByteOrder dataByteOrder)
{
	byte_t buffer[8];
	size_t count = m_stream->read(&buffer, 8);
	if (LN_ENSURE(count == 4)) return;

	// 現在の環境と同じエンディアンであればそのまま返す
	if (dataByteOrder == Environment::getByteOrder()) {
		return *((int64_t*)buffer);
	}
	// 異なるエンディアンであれば swap
	else
	{
		byte_t out[8], *p = (byte_t*)&buffer;
		out[7] = *p++;
		out[6] = *p++;
		out[5] = *p++;
		out[4] = *p++;
		out[3] = *p++;
		out[2] = *p++;
		out[1] = *p++;
		out[0] = *p++;
		return *((int64_t*)out);
	}
}

//------------------------------------------------------------------------------
uint8_t BinaryReader::readUInt8()
{
	uint8_t buffer;
	m_stream->read(&buffer, 1);
	return buffer;
}

//------------------------------------------------------------------------------
uint16_t BinaryReader::readUInt16(ByteOrder dataByteOrder)
{
	byte_t buffer[2];
	size_t count = m_stream->read(&buffer, 2);
	if (LN_ENSURE(count == 2)) return;

	// 現在の環境と同じエンディアンであればそのまま返す
	if (dataByteOrder == Environment::getByteOrder()) {
		return *((uint16_t*)buffer);
	}
	// 異なるエンディアンであれば swap
	else
	{
		byte_t out[2], *p = (byte_t*)&buffer;
		out[1] = *p++;
		out[0] = *p++;
		return *((uint16_t*)out);
	}
}

//------------------------------------------------------------------------------
uint32_t BinaryReader::readUInt32(ByteOrder dataByteOrder)
{
	byte_t buffer[4];
	size_t count = m_stream->read(&buffer, 4);
	if (LN_ENSURE(count == 4)) return;

	// 現在の環境と同じエンディアンであればそのまま返す
	if (dataByteOrder == Environment::getByteOrder()) {
		return *((uint32_t*)buffer);
	}
	// 異なるエンディアンであれば swap
	else
	{
		byte_t out[4], *p = (byte_t*)&buffer;
		out[3] = *p++;
		out[2] = *p++;
		out[1] = *p++;
		out[0] = *p++;
		return *((uint32_t*)out);
	}
}

//------------------------------------------------------------------------------
uint64_t BinaryReader::readUInt64(ByteOrder dataByteOrder)
{
	byte_t buffer[8];
	size_t count = m_stream->read(&buffer, 8);
	if (LN_ENSURE(count == 8)) return;

	// 現在の環境と同じエンディアンであればそのまま返す
	if (dataByteOrder == Environment::getByteOrder()) {
		return *((uint64_t*)buffer);
	}
	// 異なるエンディアンであれば swap
	else
	{
		byte_t out[8], *p = (byte_t*)&buffer;
		out[7] = *p++;
		out[6] = *p++;
		out[5] = *p++;
		out[4] = *p++;
		out[3] = *p++;
		out[2] = *p++;
		out[1] = *p++;
		out[0] = *p++;
		return *((uint64_t*)out);
	}
}

//------------------------------------------------------------------------------
int64_t BinaryReader::readInt(int byteCount, ByteOrder dataByteOrder)
{
	switch (byteCount)
	{
	case 1: return readInt8();
	case 2: return readInt16(dataByteOrder);
	case 4: return readInt32(dataByteOrder);
	case 8: return readInt64(dataByteOrder);
	}
	LN_UNREACHABLE();
	return 0;
}

//------------------------------------------------------------------------------
uint64_t BinaryReader::readUInt(int byteCount, ByteOrder dataByteOrder)
{
	switch (byteCount)
	{
	case 1: return readUInt8();
	case 2: return readUInt16(dataByteOrder);
	case 4: return readUInt32(dataByteOrder);
	case 8: return readUInt64(dataByteOrder);
	}
	LN_UNREACHABLE();
	return 0;
}

//------------------------------------------------------------------------------
float BinaryReader::readFloat()
{
	float buffer;
	m_stream->read(&buffer, 4);
	return buffer;
}

//------------------------------------------------------------------------------
double BinaryReader::readDouble()
{
	double buffer;
	m_stream->read(&buffer, 8);
	return buffer;
}

//------------------------------------------------------------------------------
size_t BinaryReader::read(void* buffer, size_t count)
{
	LN_ASSERT(m_stream != NULL);	// コンストラクタで必ず入力が必要なのでありえないはず
	return m_stream->read(buffer, count);
}

//------------------------------------------------------------------------------
void BinaryReader::seek(int offset)
{
	LN_ASSERT(m_stream != NULL);	// コンストラクタで必ず入力が必要なのでありえないはず
	m_stream->seek(offset, SeekOrigin_Current);
}

//------------------------------------------------------------------------------
int64_t BinaryReader::getPosition() const
{
	LN_ASSERT(m_stream != NULL);	// コンストラクタで必ず入力が必要なのでありえないはず
	return m_stream->getPosition();
}

//------------------------------------------------------------------------------
bool BinaryReader::isEOF() const
{
	LN_ASSERT(m_stream != NULL);	// コンストラクタで必ず入力が必要なのでありえないはず
	return (m_stream->getPosition() >= m_stream->getLength());
}

LN_NAMESPACE_END
