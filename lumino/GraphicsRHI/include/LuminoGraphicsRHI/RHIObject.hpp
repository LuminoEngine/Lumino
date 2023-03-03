#pragma once

namespace ln {
namespace detail {
class IGraphicsDevice;
class RHIProfiler;
class RHIBitmap;

	
class RHIDeviceObject : public URefObject
{
public:
    void destroy(); 
	IGraphicsDevice* device() const { return m_device; }

	IGraphicsDevice* m_device;
	int32_t m_objectId;

protected:
    RHIDeviceObject();
    virtual ~RHIDeviceObject();
    virtual void finalize();
    virtual void onDestroy() {}

private:
    bool m_destroyed;
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

struct RHIExtent2D {
    uint32_t width;
    uint32_t height;
};

struct RHIExtent3D {
    uint32_t width;
    uint32_t height;
    uint32_t depth;

    bool operator==(const RHIExtent3D& obj) const { return (width == obj.width && height == obj.height && depth == obj.depth); }
    bool operator!=(const RHIExtent3D& obj) const { return !operator==(obj); }
};

} // namespace detail
} // namespace ln

