#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {
class GraphicsResourceInternal;
}

/** Graphics 機能に関係するリソースのベースクラスです。*/
class LN_API GraphicsResource
	: public Object
{
protected:
	virtual void onDispose(bool explicitDisposing) override;
	virtual void onChangeDevice(detail::IGraphicsDevice* device) = 0;

LN_CONSTRUCT_ACCESS:
	GraphicsResource();
	virtual ~GraphicsResource();
	void init();

private:
	detail::GraphicsManager* m_manager;

	friend class detail::GraphicsResourceInternal;
};


namespace detail {

class GraphicsResourceInternal
{
public:
	static IndexBufferFormat selectIndexBufferFormat(int vertexCount) { return (vertexCount > 0xFFFF) ? IndexBufferFormat::UInt32 : IndexBufferFormat::UInt16; }

	static detail::GraphicsManager* manager(GraphicsResource* obj) { return obj->m_manager; }

	template<class TReturn, class TObject>
	static TReturn* resolveRHIObject(const TObject& obj, bool* outModified)
	{
		bool modified = false;
		TReturn* rhi = nullptr;
		if (obj) {
			rhi = obj->resolveRHIObject(&modified);
		}
		if (outModified) {
			*outModified = modified;
		}
		return rhi;
	}
};

} // namespace detail
} // namespace ln
