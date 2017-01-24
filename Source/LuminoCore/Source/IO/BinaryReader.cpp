
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
	LN_THROW(stream != NULL, ArgumentException);
	LN_REFOBJ_SET(m_stream, stream);
}

//------------------------------------------------------------------------------
BinaryReader::~BinaryReader()
{
	LN_SAFE_RELEASE(m_stream);
}

//------------------------------------------------------------------------------
int8_t BinaryReader::ReadInt8()
{
	int8_t buffer;
	m_stream->Read(&buffer, 1);
	return buffer;
}

//------------------------------------------------------------------------------
int16_t BinaryReader::ReadInt16(ByteOrder dataByteOrder)
{
	byte_t buffer[2];
	size_t count = m_stream->Read(&buffer, 2);
	LN_THROW(count == 2, EndOfStreamException);

	// 現在の環境と同じエンディアンであればそのまま返す
	if (dataByteOrder == Environment::GetByteOrder()) {
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
int32_t BinaryReader::ReadInt32(ByteOrder dataByteOrder)
{
	byte_t buffer[4];
	size_t count = m_stream->Read(&buffer, 4);
	LN_THROW(count == 4, EndOfStreamException);

	// 現在の環境と同じエンディアンであればそのまま返す
	if (dataByteOrder == Environment::GetByteOrder()) {
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
int64_t BinaryReader::ReadInt64(ByteOrder dataByteOrder)
{
	byte_t buffer[8];
	size_t count = m_stream->Read(&buffer, 8);
	LN_THROW(count == 8, EndOfStreamException);

	// 現在の環境と同じエンディアンであればそのまま返す
	if (dataByteOrder == Environment::GetByteOrder()) {
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
uint8_t BinaryReader::ReadUInt8()
{
	uint8_t buffer;
	m_stream->Read(&buffer, 1);
	return buffer;
}

//------------------------------------------------------------------------------
uint16_t BinaryReader::ReadUInt16(ByteOrder dataByteOrder)
{
	byte_t buffer[2];
	size_t count = m_stream->Read(&buffer, 2);
	LN_THROW(count == 2, EndOfStreamException);

	// 現在の環境と同じエンディアンであればそのまま返す
	if (dataByteOrder == Environment::GetByteOrder()) {
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
uint32_t BinaryReader::ReadUInt32(ByteOrder dataByteOrder)
{
	byte_t buffer[4];
	size_t count = m_stream->Read(&buffer, 4);
	LN_THROW(count == 4, EndOfStreamException);

	// 現在の環境と同じエンディアンであればそのまま返す
	if (dataByteOrder == Environment::GetByteOrder()) {
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
uint64_t BinaryReader::ReadUInt64(ByteOrder dataByteOrder)
{
	byte_t buffer[8];
	size_t count = m_stream->Read(&buffer, 8);
	LN_THROW(count == 8, EndOfStreamException);

	// 現在の環境と同じエンディアンであればそのまま返す
	if (dataByteOrder == Environment::GetByteOrder()) {
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
int64_t BinaryReader::ReadInt(int byteCount, ByteOrder dataByteOrder)
{
	switch (byteCount)
	{
	case 1: return ReadInt8();
	case 2: return ReadInt16(dataByteOrder);
	case 4: return ReadInt32(dataByteOrder);
	case 8: return ReadInt64(dataByteOrder);
	}
	LN_THROW(0, ArgumentException);
	return 0;
}

//------------------------------------------------------------------------------
uint64_t BinaryReader::ReadUInt(int byteCount, ByteOrder dataByteOrder)
{
	switch (byteCount)
	{
	case 1: return ReadUInt8();
	case 2: return ReadUInt16(dataByteOrder);
	case 4: return ReadUInt32(dataByteOrder);
	case 8: return ReadUInt64(dataByteOrder);
	}
	LN_THROW(0, ArgumentException);
	return 0;
}

//------------------------------------------------------------------------------
float BinaryReader::ReadFloat()
{
	float buffer;
	m_stream->Read(&buffer, 4);
	return buffer;
}

//------------------------------------------------------------------------------
double BinaryReader::ReadDouble()
{
	double buffer;
	m_stream->Read(&buffer, 8);
	return buffer;
}

//------------------------------------------------------------------------------
size_t BinaryReader::Read(void* buffer, size_t count)
{
	LN_ASSERT(m_stream != NULL);	// コンストラクタで必ず入力が必要なのでありえないはず
	return m_stream->Read(buffer, count);
}

//------------------------------------------------------------------------------
void BinaryReader::Seek(int offset)
{
	LN_ASSERT(m_stream != NULL);	// コンストラクタで必ず入力が必要なのでありえないはず
	m_stream->Seek(offset, SeekOrigin_Current);
}

//------------------------------------------------------------------------------
int64_t BinaryReader::GetPosition() const
{
	LN_ASSERT(m_stream != NULL);	// コンストラクタで必ず入力が必要なのでありえないはず
	return m_stream->GetPosition();
}

//------------------------------------------------------------------------------
bool BinaryReader::IsEOF() const
{
	LN_ASSERT(m_stream != NULL);	// コンストラクタで必ず入力が必要なのでありえないはず
	return (m_stream->GetPosition() >= m_stream->GetLength());
}

LN_NAMESPACE_END
