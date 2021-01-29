#pragma once

namespace ln {
namespace detail {

class RHIObject
{
protected:
	RHIObject() = default;

private:
	RHIObject(const RHIObject&) = delete;
    void operator=(const RHIObject&) = delete;
};

class RHIBitmap
	: public RHIObject
{
public:
	RHIBitmap();
	bool init(int32_t elementSize, int32_t width, int32_t height);

	uint8_t* writableData() { return m_data.data(); }
	const uint8_t* data() const { return m_data.data(); }
	int32_t width() const { return m_width; }
	int32_t height() const { return m_height; }

	void blit(const RHIBitmap* srcBitmap);
	void copyRaw(const void* data, size_t size);

private:
	std::vector<uint8_t> m_data;
	int32_t m_elementSize;
	int32_t m_width;
	int32_t m_height;
};

template<class T>
using RHIPtr = std::shared_ptr<T>;

template<class T, class... TArgs>
inline RHIPtr<T> makeRHIRef(TArgs&&... args)
{
	return std::make_shared<T>(std::forward<TArgs>(args)...);
}

} // namespace detail
} // namespace ln

