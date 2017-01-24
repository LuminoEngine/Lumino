
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
	LN_CHECK_ARG(stream != nullptr);
	LN_REFOBJ_SET(m_stream, stream);
}

//------------------------------------------------------------------------------
BinaryWriter::~BinaryWriter()
{
	LN_SAFE_RELEASE(m_stream);
}

//------------------------------------------------------------------------------
void BinaryWriter::WriteInt8(int8_t value)
{
	m_stream->Write(&value, 1);
}

//------------------------------------------------------------------------------
void BinaryWriter::WriteInt16(int16_t value, ByteOrder byteOrder)
{
	// 現在の環境と同じエンディアンであればそのまま
	if (byteOrder == Environment::GetByteOrder()) {
		m_stream->Write(&value, 2);
	}
	// 異なるエンディアンであれば swap
	else
	{
		byte_t out[2], *p = (byte_t*)&value;
		out[1] = *p++;
		out[0] = *p++;
		m_stream->Write(out, 2);
	}
}

//------------------------------------------------------------------------------
void BinaryWriter::WriteInt32(int32_t value, ByteOrder byteOrder)
{
	// 現在の環境と同じエンディアンであればそのまま
	if (byteOrder == Environment::GetByteOrder()) {
		m_stream->Write(&value, 4);
	}
	// 異なるエンディアンであれば swap
	else
	{
		byte_t out[4], *p = (byte_t*)&value;
		out[3] = *p++;
		out[2] = *p++;
		out[1] = *p++;
		out[0] = *p++;
		m_stream->Write(out, 4);
	}
}

//------------------------------------------------------------------------------
void BinaryWriter::WriteInt64(int64_t value, ByteOrder byteOrder)
{
	// 現在の環境と同じエンディアンであればそのまま
	if (byteOrder == Environment::GetByteOrder()) {
		m_stream->Write(&value, 8);
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
		m_stream->Write(out, 8);
	}
}

//------------------------------------------------------------------------------
void BinaryWriter::WriteUInt8(uint8_t value)
{
	m_stream->Write(&value, 1);
}

//------------------------------------------------------------------------------
void BinaryWriter::WriteUInt16(uint16_t value, ByteOrder byteOrder)
{
	// 現在の環境と同じエンディアンであればそのまま
	if (byteOrder == Environment::GetByteOrder()) {
		m_stream->Write(&value, 2);
	}
	// 異なるエンディアンであれば swap
	else
	{
		byte_t out[2], *p = (byte_t*)&value;
		out[1] = *p++;
		out[0] = *p++;
		m_stream->Write(out, 2);
	}
}

//------------------------------------------------------------------------------
void BinaryWriter::WriteUInt32(uint32_t value, ByteOrder byteOrder)
{
	// 現在の環境と同じエンディアンであればそのまま
	if (byteOrder == Environment::GetByteOrder()) {
		m_stream->Write(&value, 4);
	}
	// 異なるエンディアンであれば swap
	else
	{
		byte_t out[4], *p = (byte_t*)&value;
		out[3] = *p++;
		out[2] = *p++;
		out[1] = *p++;
		out[0] = *p++;
		m_stream->Write(out, 4);
	}
}

//------------------------------------------------------------------------------
void BinaryWriter::WriteUInt64(uint64_t value, ByteOrder byteOrder)
{
	// 現在の環境と同じエンディアンであればそのまま
	if (byteOrder == Environment::GetByteOrder()) {
		m_stream->Write(&value, 8);
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
		m_stream->Write(out, 8);
	}
}

//------------------------------------------------------------------------------
void BinaryWriter::WriteFloat(float value)
{
	m_stream->Write(&value, 4);
}

//------------------------------------------------------------------------------
void BinaryWriter::WriteDouble(double value)
{
	m_stream->Write(&value, 8);
}

//------------------------------------------------------------------------------
void BinaryWriter::Write(const void* buffer, size_t count)
{
	m_stream->Write(buffer, count);
}

//------------------------------------------------------------------------------
void BinaryWriter::Seek(int offset, SeekOrigin origin)
{
	m_stream->Seek(offset, origin);
}

//------------------------------------------------------------------------------
int64_t BinaryWriter::GetPosition() const
{
	return m_stream->GetPosition();
}

LN_NAMESPACE_END
