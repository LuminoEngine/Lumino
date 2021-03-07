#pragma once

namespace ln {
namespace detail {
class IGraphicsDevice;
class RHIProfiler;
class RHIBitmap;

	
class RHIDeviceObject
    : public RefObject
{
public:
	virtual void dispose();	// Prepare for multiple calls
	IGraphicsDevice* device() const { return m_device; }
	int32_t objectId() const { return m_objectId; }

	IGraphicsDevice* m_device;
	int32_t m_objectId;
protected:
    RHIDeviceObject();
    virtual ~RHIDeviceObject();
    virtual void finalize();

private:
    bool m_disposed;
	bool m_profiling;

	friend class RHIProfiler;
	friend class IGraphicsDevice;
};

class RHIRefObject
{
protected:
	RHIRefObject() = default;

private:
	RHIRefObject(const RHIRefObject&) = delete;
	void operator=(const RHIRefObject&) = delete;
};

template<class T>
using RHIRef = std::shared_ptr<T>;

template<class T, class... TArgs>
inline RHIRef<T> makeRHIRef(TArgs&&... args)
{
	return std::make_shared<T>(std::forward<TArgs>(args)...);
}

struct RHISizeI
{
	int32_t width;
	int32_t height;
};

} // namespace detail
} // namespace ln

