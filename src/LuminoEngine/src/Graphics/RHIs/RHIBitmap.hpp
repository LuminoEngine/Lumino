#pragma once
#include "GraphicsDeviceContext.hpp"

namespace ln {
namespace detail {

class RHIBitmap
	: public RHIRefObject
{
public:
	RHIBitmap();
	bool init(int32_t elementSize, int32_t width, int32_t height);
	void initWrap(const void* data, int32_t elementSize, int32_t width, int32_t height);
	void initWritableWrap(void* data, int32_t elementSize, int32_t width, int32_t height);

	uint8_t* writableData() { return m_writablePtr; }
	const uint8_t* data() const { return m_ptr; }
	int32_t width() const { return m_width; }
	int32_t height() const { return m_height; }

	void blit(const RHIBitmap* srcBitmap);
	void copyRaw(const void* data, size_t size);

private:
	std::vector<uint8_t> m_data;
	const uint8_t* m_ptr;
	uint8_t* m_writablePtr;
	int32_t m_elementSize;
	int32_t m_width;
	int32_t m_height;
};

} // namespace detail
} // namespace ln

