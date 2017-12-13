
#include "../../Internal.h"
#include "DX9GraphicsDevice.h"
#include "DX9IndexBuffer.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{

//==============================================================================
// DX9IndexBuffer
//==============================================================================

//------------------------------------------------------------------------------
DX9IndexBuffer::DX9IndexBuffer()
	: m_graphicsDevice(NULL)
	, m_format(IndexBufferFormat_UInt16)
	, m_indexBuffer(NULL)
	, m_indexCount(0)
	, m_indexStride(0)
	, m_usage(ResourceUsage::Static)
{
}

//------------------------------------------------------------------------------
DX9IndexBuffer::~DX9IndexBuffer()
{
	LN_COM_SAFE_RELEASE(m_indexBuffer);
	LN_SAFE_RELEASE(m_graphicsDevice);
}

//------------------------------------------------------------------------------
void DX9IndexBuffer::create(DX9GraphicsDevice* device, int indexCount, const void* initialData, IndexBufferFormat format, ResourceUsage usage)
{
	LN_REFOBJ_SET(m_graphicsDevice, device);
	m_format = format;
	m_indexCount = indexCount;
	m_indexStride = (m_format == IndexBufferFormat_UInt16) ? 2 : 4;

	onResetDevice();

	// data コピー
	if (initialData)
	{
		void* buf;
		size_t lockedSize;
		lock(&buf, &lockedSize);
		memcpy(buf, initialData, m_indexStride * m_indexCount);
		unlock();
	}
}

//------------------------------------------------------------------------------
void DX9IndexBuffer::setSubData(uint32_t offsetBytes, const void* data, uint32_t dataBytes)
{
	byte_t* buf;
	size_t lockedSize;
	lock((void**)&buf, &lockedSize);
	memcpy(buf + offsetBytes, data, std::min(lockedSize - offsetBytes, dataBytes));
	unlock();
}

//------------------------------------------------------------------------------
void DX9IndexBuffer::lock(void** lockedBuffer, size_t* lockedSize)
{
	DWORD flags = 0;
	if (m_usage == ResourceUsage::Dynamic)
	{
		flags = D3DLOCK_DISCARD;
	}

	LN_COMCALL(m_indexBuffer->Lock(0, 0, lockedBuffer, flags));
	*lockedSize = m_indexStride * m_indexCount;
}

//------------------------------------------------------------------------------
void DX9IndexBuffer::unlock()
{
	LN_COMCALL(m_indexBuffer->Unlock());
}

//------------------------------------------------------------------------------
void DX9IndexBuffer::onLostDevice()
{
	if (m_usage == ResourceUsage::Dynamic)
	{
		LN_COM_SAFE_RELEASE(m_indexBuffer);
	}
}

//------------------------------------------------------------------------------
void DX9IndexBuffer::onResetDevice()
{
	if (m_indexBuffer == nullptr)
	{
		D3DPOOL pool = D3DPOOL_MANAGED;
		DWORD dxUsage = D3DUSAGE_WRITEONLY;
		//if (m_usage == ResourceUsage::Dynamic)
		{
			pool = D3DPOOL_DEFAULT;
			dxUsage |= D3DUSAGE_DYNAMIC;
		}

		LN_COMCALL(
			m_graphicsDevice->getIDirect3DDevice9()->CreateIndexBuffer(
				m_indexStride * m_indexCount,
				dxUsage,
				(m_format == IndexBufferFormat_UInt16) ? D3DFMT_INDEX16 : D3DFMT_INDEX32,
				pool,
				&m_indexBuffer,
				NULL));
	}
}

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
