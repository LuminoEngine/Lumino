
#include "Internal.hpp"
#include <LuminoCore/IO/Stream.hpp>
#include <LuminoCore/IO/BinaryReader.hpp>

namespace ln {

//==============================================================================
// BinaryReader

BinaryReader::BinaryReader(Stream* stream)
	: m_stream(stream)
{
	if (LN_REQUIRE(stream)) return;
}

BinaryReader::~BinaryReader()
{
}

int8_t BinaryReader::readInt8()
{
	int8_t buffer;
	m_stream->read(&buffer, 1);
	return buffer;
}

int16_t BinaryReader::readInt16(ByteOrder dataByteOrder)
{
	byte_t buffer[2];
	size_t count = m_stream->read(&buffer, 2);
	if (LN_ENSURE(count == 2)) return 0;

	if (dataByteOrder == Environment::byteOrder()) {
		return *((int16_t*)buffer);
	}
	else
	{
		byte_t out[2], *p = (byte_t*)&buffer;
		out[1] = *p++;
		out[0] = *p++;
		return *((int16_t*)out);
	}
}

int32_t BinaryReader::readInt32(ByteOrder dataByteOrder)
{
	byte_t buffer[4];
	size_t count = m_stream->read(&buffer, 4);
	if (LN_ENSURE(count == 4)) return 0;

	if (dataByteOrder == Environment::byteOrder()) {
		return *((int32_t*)buffer);
	}
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

int64_t BinaryReader::readInt64(ByteOrder dataByteOrder)
{
	byte_t buffer[8];
	size_t count = m_stream->read(&buffer, 8);
	if (LN_ENSURE(count == 8)) return 0;

	if (dataByteOrder == Environment::byteOrder()) {
		return *((int64_t*)buffer);
	}
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

uint8_t BinaryReader::readUInt8()
{
	uint8_t buffer;
	m_stream->read(&buffer, 1);
	return buffer;
}

uint16_t BinaryReader::readUInt16(ByteOrder dataByteOrder)
{
	byte_t buffer[2];
	size_t count = m_stream->read(&buffer, 2);
	if (LN_ENSURE(count == 2)) return 0;

	if (dataByteOrder == Environment::byteOrder()) {
		return *((uint16_t*)buffer);
	}
	else
	{
		byte_t out[2], *p = (byte_t*)&buffer;
		out[1] = *p++;
		out[0] = *p++;
		return *((uint16_t*)out);
	}
}

uint32_t BinaryReader::readUInt32(ByteOrder dataByteOrder)
{
	byte_t buffer[4];
	size_t count = m_stream->read(&buffer, 4);
	if (LN_ENSURE(count == 4)) return 0;

	if (dataByteOrder == Environment::byteOrder()) {
		return *((uint32_t*)buffer);
	}
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

uint64_t BinaryReader::readUInt64(ByteOrder dataByteOrder)
{
	byte_t buffer[8];
	size_t count = m_stream->read(&buffer, 8);
	if (LN_ENSURE(count == 8)) return 0;

	if (dataByteOrder == Environment::byteOrder()) {
		return *((uint64_t*)buffer);
	}
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

float BinaryReader::readFloat()
{
	float buffer;
	m_stream->read(&buffer, 4);
	return buffer;
}

double BinaryReader::readDouble()
{
	double buffer;
	m_stream->read(&buffer, 8);
	return buffer;
}

String BinaryReader::readString(size_t byteCount, ln::TextEncoding* encoding)
{
	if (encoding) {
		LN_NOTIMPLEMENTED();
		return String::Empty;
	}
	if (ln::Environment::byteOrder() == ByteOrder::BigEndian) {
		LN_NOTIMPLEMENTED();
		return String::Empty;
	}

	m_stringCache.resize(byteCount);
	read(m_stringCache.data(), m_stringCache.size());
	return String((const Char*)m_stringCache.data(), m_stringCache.size() / sizeof(Char));
}

size_t BinaryReader::read(void* buffer, size_t count)
{
	if (LN_REQUIRE(m_stream)) return 0;
	return m_stream->read(buffer, count);
}

void BinaryReader::seek(int offset)
{
	if (LN_REQUIRE(m_stream)) return;
	m_stream->seek(offset, SeekOrigin::Current);
}

bool BinaryReader::isEof() const
{
	if (LN_REQUIRE(m_stream)) return true;
	return (m_stream->position() >= m_stream->length());
}

} // namespace ln
