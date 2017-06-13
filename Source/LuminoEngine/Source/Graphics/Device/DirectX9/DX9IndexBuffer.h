
#pragma once 
#include "../GraphicsDriverInterface.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{
class DX9GraphicsDevice;

class DX9IndexBuffer
	: public IIndexBuffer
{
public:
	DX9IndexBuffer();
	virtual ~DX9IndexBuffer();

public:

	/// 作成
	void create(DX9GraphicsDevice* device, int indexCount, const void* initialData, IndexBufferFormat format, ResourceUsage usage);

	/// IDirect3DIndexBuffer9 へのポインタ取得
	IDirect3DIndexBuffer9* getDxIndexBuffer() const { return m_indexBuffer; }

public:
	virtual size_t getByteCount() const { return m_indexCount * m_indexStride; }
	virtual IndexBufferFormat getFormat() const { return m_format; }
	virtual ResourceUsage getUsage() const { return m_usage; }
	virtual void setSubData(uint32_t offsetBytes, const void* data, uint32_t dataBytes);
	virtual void lock(void** lockedBuffer, size_t* lockedSize);
	virtual void unlock();
	virtual void onLostDevice();
	virtual void onResetDevice();

private:
	DX9GraphicsDevice*		m_graphicsDevice;
	IndexBufferFormat		m_format;
	IDirect3DIndexBuffer9*	m_indexBuffer;
	uint32_t				m_indexCount;
	uint32_t				m_indexStride;
	ResourceUsage			m_usage;
};

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
