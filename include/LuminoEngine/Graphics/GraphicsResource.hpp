#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {
class GraphicsManager;
class IGraphicsDeviceContext;
}

/**
	Graphics 機能に関係するリソースのベースクラスです。

*/
class LN_API GraphicsResource
	: public Object
{
public:

	virtual void dispose() override;

protected:
	virtual void onChangeDevice(detail::IGraphicsDeviceContext* device) = 0;

LN_CONSTRUCT_ACCESS:
	GraphicsResource();
	virtual ~GraphicsResource();
	void init();

LN_INTERNAL_ACCESS:
	detail::GraphicsManager* manager() const { return m_manager; }
	detail::IGraphicsDeviceContext* deviceContext() const;
private:
	detail::GraphicsManager* m_manager;
};


namespace detail {

class GraphicsResourceHelper
{
public:
	template<class TReturn, class TObject>
	static TReturn* resolveRHIObject(TObject& obj) { return (obj) ? obj->resolveRHIObject() : nullptr; }

	static IndexBufferFormat selectIndexBufferFormat(int vertexCount) { return (vertexCount > 0xFFFF) ? IndexBufferFormat::UInt32 : IndexBufferFormat::UInt16; }
};

} // namespace detail
} // namespace ln
