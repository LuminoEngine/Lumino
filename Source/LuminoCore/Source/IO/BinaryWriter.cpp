
#include "../Internal.h"
#include <Lumino/IO/Stream.h>
#include <Lumino/IO/BinaryWriter.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// BinaryWriter
//==============================================================================

//------------------------------------------------------------------------------
BinaryWriter::BinaryWriter(Stream* stream)
	: m_stream(nullptr)
{
	if (LN_REQUIRE(stream != nullptr)) return;
	LN_REFOBJ_SET(m_stream, stream);
}

//------------------------------------------------------------------------------
BinaryWriter::~BinaryWriter()
{
	LN_SAFE_RELEASE(m_stream);
}

//------------------------------------------------------------------------------
void BinaryWriter::writeInt8(int8_t value)
{
	m_stream->write(&value, 1);
}

//------------------------------------------------------------------------------
void BinaryWriter::writeInt16(int16_t value, ByteOrder byteOrder)
{
	// 現在の環境と同じエンディアンであればそのまま
	if (byteOrder == Environment::getByteOrder()) {
		m_stream->write(&value, 2);
	}
	// 異なるエンディアンであれば swap
	else
	{
		byte_t out[2], *p = (byte_t*)&value;
		out[1] = *p++;
		out[0] = *p++;
		m_stream->write(out, 2);
	}
}

//------------------------------------------------------------------------------
void BinaryWriter::writeInt32(int32_t value, ByteOrder byteOrder)
{
	// 現在の環境と同じエンディアンであればそのまま
	if (byteOrder == Environment::getByteOrder()) {
		m_stream->write(&value, 4);
	}
	// 異なるエンディアンであれば swap
	else
	{
		byte_t out[4], *p = (byte_t*)&value;
		out[3] = *p++;
		out[2] = *p++;
		out[1] = *p++;
		out[0] = *p++;
		m_stream->write(out, 4);
	}
}

//------------------------------------------------------------------------------
void BinaryWriter::writeInt64(int64_t value, ByteOrder byteOrder)
{
	// 現在の環境と同じエンディアンであればそのまま
	if (byteOrder == Environment::getByteOrder()) {
		m_stream->write(&value, 8);
	}
	// 異なるエンディアンであれば swap
	else
	{
		byte_t out[8], *p = (byte_t*)&value;
		out[7] = *p++;
		out[6] = *p++;
		out[5] = *p++;
		out[4] = *p++;
		out[3] = *p++;
		out[2] = *p++;
		out[1] = *p++;
		out[0] = *p++;
		m_stream->write(out, 8);
	}
}

//------------------------------------------------------------------------------
void BinaryWriter::writeUInt8(uint8_t value)
{
	m_stream->write(&value, 1);
}

//------------------------------------------------------------------------------
void BinaryWriter::writeUInt16(uint16_t value, ByteOrder byteOrder)
{
	// 現在の環境と同じエンディアンであればそのまま
	if (byteOrder == Environment::getByteOrder()) {
		m_stream->write(&value, 2);
	}
	// 異なるエンディアンであれば swap
	else
	{
		byte_t out[2], *p = (byte_t*)&value;
		out[1] = *p++;
		out[0] = *p++;
		m_stream->write(out, 2);
	}
}

//------------------------------------------------------------------------------
void BinaryWriter::writeUInt32(uint32_t value, ByteOrder byteOrder)
{
	// 現在の環境と同じエンディアンであればそのまま
	if (byteOrder == Environment::getByteOrder()) {
		m_stream->write(&value, 4);
	}
	// 異なるエンディアンであれば swap
	else
	{
		byte_t out[4], *p = (byte_t*)&value;
		out[3] = *p++;
		out[2] = *p++;
		out[1] = *p++;
		out[0] = *p++;
		m_stream->write(out, 4);
	}
}

//------------------------------------------------------------------------------
void BinaryWriter::writeUInt64(uint64_t value, ByteOrder byteOrder)
{
	// 現在の環境と同じエンディアンであればそのまま
	if (byteOrder == Environment::getByteOrder()) {
		m_stream->write(&value, 8);
	}
	// 異なるエンディアンであれば swap
	else
	{
		byte_t out[8], *p = (byte_t*)&value;
		out[7] = *p++;
		out[6] = *p++;
		out[5] = *p++;
		out[4] = *p++;
		out[3] = *p++;
		out[2] = *p++;
		out[1] = *p++;
		out[0] = *p++;
		m_stream->write(out, 8);
	}
}

//------------------------------------------------------------------------------
void BinaryWriter::writeFloat(float value)
{
	m_stream->write(&value, 4);
}

//------------------------------------------------------------------------------
void BinaryWriter::writeDouble(double value)
{
	m_stream->write(&value, 8);
}

//------------------------------------------------------------------------------
void BinaryWriter::write(const void* buffer, size_t count)
{
	m_stream->write(buffer, count);
}

//------------------------------------------------------------------------------
void BinaryWriter::seek(int offset, SeekOrigin origin)
{
	m_stream->seek(offset, origin);
}

//------------------------------------------------------------------------------
int64_t BinaryWriter::getPosition() const
{
	return m_stream->getPosition();
}

LN_NAMESPACE_END
