
#include "Internal.hpp"
#include "MixHash.hpp"
#include "RHIBitmap.hpp"

namespace ln {
namespace detail {

//==============================================================================
// RHIBitmap

RHIBitmap::RHIBitmap()
	: m_data()
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
	return true;
}

void RHIBitmap::blit(const RHIBitmap* srcBitmap)
{
	uint8_t* dst = m_data.data();
	const uint8_t* src = srcBitmap->m_data.data();
	const int32_t height = std::min(m_height, srcBitmap->m_height);
	const int32_t dstPitch = m_width * m_elementSize;
	const int32_t srcPitch = srcBitmap->m_width * srcBitmap->m_elementSize;

	for (int32_t y = 0; y < height; y++) {
		byte_t* d = static_cast<byte_t*>(dst) + (dstPitch * y);
		const byte_t* s = static_cast<const byte_t*>(src) + (srcPitch * y);
		memcpy(d, s, dstPitch);
	}
}

void RHIBitmap::copyRaw(const void* data, size_t size)
{
	memcpy(m_data.data(), data, std::min(m_data.size(), size));
}

} // namespace detail
} // namespace ln

