
#include "Internal.hpp"
#include <LuminoEngine/Base/MixHash.hpp>
#include "RHIBitmap.hpp"

namespace ln {
namespace detail {

//==============================================================================
// RHIBitmap

RHIBitmap::RHIBitmap()
	: m_data()
	, m_ptr(nullptr)
	, m_elementSize(0)
	, m_width(0)
	, m_height(0)
{
}

bool RHIBitmap::init(int32_t elementSize, int32_t width, int32_t height)
{
	if (width < 0 || height < 0) {
		LN_ERROR("RHIBitmap invalid size.");
		return false;
	}
	m_width = width;
	m_elementSize = elementSize;
	m_height = height;
	m_data.resize(m_elementSize * m_width * m_height);
	m_ptr = m_data.data();
	m_writablePtr = m_data.data();
	return true;
}

void RHIBitmap::initWrap(const void* data, int32_t elementSize, int32_t width, int32_t height)
{
	m_width = width;
	m_elementSize = elementSize;
	m_height = height;
	m_ptr = static_cast<const uint8_t*>(data);
}

void RHIBitmap::initWritableWrap(void* data, int32_t elementSize, int32_t width, int32_t height)
{
	initWrap(data, elementSize, width, height);
	m_writablePtr = static_cast<uint8_t*>(data);
}

void RHIBitmap::blit(const RHIBitmap* srcBitmap)
{
	uint8_t* dst = m_writablePtr;
	const uint8_t* src = srcBitmap->data();
	const int32_t height = std::min(m_height, srcBitmap->m_height);
	const int32_t dstPitch = m_width * m_elementSize;
	const int32_t srcPitch = srcBitmap->m_width * srcBitmap->m_elementSize;
	const int32_t copyPitch = std::min(dstPitch, srcPitch);

	for (int32_t y = 0; y < height; y++) {
		byte_t* d = static_cast<byte_t*>(dst) + (dstPitch * y);
		const byte_t* s = static_cast<const byte_t*>(src) + (srcPitch * y);
		memcpy(d, s, copyPitch);
	}
}

void RHIBitmap::copyRaw(const void* data, size_t size)
{
	const size_t s = m_elementSize * m_width * m_height;
	memcpy(m_writablePtr, data, std::min(s, size));
}

} // namespace detail
} // namespace ln

