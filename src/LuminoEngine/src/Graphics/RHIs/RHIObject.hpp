#pragma once

namespace ln {
namespace detail {
class IGraphicsDevice;
class RHIProfiler;
	
class IGraphicsDeviceObject
    : public RefObject
{
public:
	virtual void dispose();	// Prepare for multiple calls
	IGraphicsDevice* device() const { return m_device; }
	int32_t objectId() const { return m_objectId; }

	IGraphicsDevice* m_device;
	int32_t m_objectId;
protected:
    IGraphicsDeviceObject();
    virtual ~IGraphicsDeviceObject();
    virtual void finalize();

private:
    bool m_disposed;
	bool m_profiling;

	friend class RHIProfiler;
	friend class IGraphicsDevice;
};

class RHIObject
{
protected:
	RHIObject() = default;

private:
	RHIObject(const RHIObject&) = delete;
	void operator=(const RHIObject&) = delete;
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

