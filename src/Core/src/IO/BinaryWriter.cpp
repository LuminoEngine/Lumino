
#include "Internal.hpp"
#include <LuminoCore/IO/Stream.hpp>
#include <LuminoCore/IO/BinaryWriter.hpp>

namespace ln {

//==============================================================================
// BinaryWriter

BinaryWriter::BinaryWriter(Stream* stream)
	: m_stream(stream)
{
	if (LN_REQUIRE(stream)) return;
}

BinaryWriter::~BinaryWriter()
{
}

void BinaryWriter::writeInt8(int8_t value)
{
	m_stream->write(&value, 1);
}

void BinaryWriter::writeInt16(int16_t value, ByteOrder byteOrder)
{
	if (byteOrder == Environment::byteOrder()) {
		m_stream->write(&value, 2);
	}
	else
	{
		byte_t out[2], *p = (byte_t*)&value;
		out[1] = *p++;
		out[0] = *p++;
		m_stream->write(out, 2);
	}
}

void BinaryWriter::writeInt32(int32_t value, ByteOrder byteOrder)
{
	if (byteOrder == Environment::byteOrder()) {
		m_stream->write(&value, 4);
	}
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

void BinaryWriter::writeInt64(int64_t value, ByteOrder byteOrder)
{
	if (byteOrder == Environment::byteOrder()) {
		m_stream->write(&value, 8);
	}
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

void BinaryWriter::writeUInt8(uint8_t value)
{
	m_stream->write(&value, 1);
}

void BinaryWriter::writeUInt16(uint16_t value, ByteOrder byteOrder)
{
	if (byteOrder == Environment::byteOrder()) {
		m_stream->write(&value, 2);
	}
	else
	{
		byte_t out[2], *p = (byte_t*)&value;
		out[1] = *p++;
		out[0] = *p++;
		m_stream->write(out, 2);
	}
}

void BinaryWriter::writeUInt32(uint32_t value, ByteOrder byteOrder)
{
	if (byteOrder == Environment::byteOrder()) {
		m_stream->write(&value, 4);
	}
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

void BinaryWriter::writeUInt64(uint64_t value, ByteOrder byteOrder)
{
	if (byteOrder == Environment::byteOrder()) {
		m_stream->write(&value, 8);
	}
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

void BinaryWriter::writeFloat(float value)
{
	m_stream->write(&value, 4);
}

void BinaryWriter::writeDouble(double value)
{
	m_stream->write(&value, 8);
}

void BinaryWriter::writeString(const StringRef& str, ln::TextEncoding* encoding)
{
	if (encoding) {
		LN_NOTIMPLEMENTED();
		return;
	}
	if (ln::Environment::byteOrder() == ByteOrder::BigEndian) {
		LN_NOTIMPLEMENTED();
		return;
	}

	write(str.data(), str.length() * sizeof(Char));
}

void BinaryWriter::write(const void* buffer, size_t count)
{
	if (LN_REQUIRE(m_stream)) return;
	m_stream->write(buffer, count);
}

void BinaryWriter::seek(int offset, SeekOrigin origin)
{
	if (LN_REQUIRE(m_stream)) return;
	m_stream->seek(offset, origin);
}

} // namespace ln
