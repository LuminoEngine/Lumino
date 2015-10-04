
#pragma once 
#include "../GraphicsDriverInterface.h"

namespace Lumino
{
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
	void Create(DX9GraphicsDevice* device, int indexCount, const void* initialData, IndexBufferFormat format, DeviceResourceUsage usage);

	/// IDirect3DIndexBuffer9 へのポインタ取得
	IDirect3DIndexBuffer9* GetDxIndexBuffer() const { return m_indexBuffer; }

	/// オンメモリデータの取得
	void* GetDynamicData() { return m_indices; }

public:
	virtual IndexBufferFormat GetFormat() const { return m_format; }
	virtual DeviceResourceUsage GetUsage() const { return (m_indices != NULL) ? DeviceResourceUsage_Dynamic : DeviceResourceUsage_Static; }
	virtual void SetSubData(uint32_t offsetBytes, const void* data, uint32_t dataBytes);
	virtual void Lock(void** lockedBuffer, size_t* lockedSize);
	virtual void Unlock();
	virtual void OnLostDevice() {}
	virtual void OnResetDevice() {}

private:
	DX9GraphicsDevice*		m_graphicsDevice;
	IndexBufferFormat		m_format;
	IDirect3DIndexBuffer9*	m_indexBuffer;	    ///< IDirect3DIndexBuffer9 へのポインタ
	void*	                m_indices;			///< 動的なインデックスを作る場合はここに作る
	uint32_t				m_indexCount;
	uint32_t				m_indexStride;
};

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
} // namespace Lumino
