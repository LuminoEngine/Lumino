
#include "../../Internal.h"
#include "DX9Module.h"
#include "DX9GraphicsDevice.h"
#include "DX9VertexBuffer.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{

//==============================================================================
// DX9VertexBuffer
//==============================================================================

//------------------------------------------------------------------------------
DX9VertexBuffer::DX9VertexBuffer()
	: m_graphicsDevice(nullptr)
	, m_vertexBuffer(nullptr)
	, m_bufferSize(0)
	, m_usage(ResourceUsage::Static)
{
}

//------------------------------------------------------------------------------
DX9VertexBuffer::~DX9VertexBuffer()
{
	LN_COM_SAFE_RELEASE(m_vertexBuffer);
	LN_SAFE_RELEASE(m_graphicsDevice);
}

//------------------------------------------------------------------------------
void DX9VertexBuffer::create(DX9GraphicsDevice* device, size_t bufferSize, const void* initialData, ResourceUsage usage)
{
	LN_REFOBJ_SET(m_graphicsDevice, device);
	m_bufferSize = bufferSize;
	m_usage = usage;

	onResetDevice();

	// 頂点として設定するデータがある場合
	if (initialData != NULL)
	{
		void* v = lock();
		memcpy_s(v, m_bufferSize, initialData, m_bufferSize);
		unlock();
	}
}

//------------------------------------------------------------------------------
void DX9VertexBuffer::setSubData(uint32_t offsetBytes, const void* data, uint32_t dataBytes)
{
	byte_t* buf = (byte_t*)lock();
	memcpy(buf + offsetBytes, data, dataBytes);	// TOOD: IndexBuffer と同じサイズチェック
	unlock();
}

//------------------------------------------------------------------------------
void* DX9VertexBuffer::lock()
{
	DWORD flags = 0;
	if (m_usage == ResourceUsage::Dynamic)
	{
		flags = D3DLOCK_DISCARD;
	}

	void* v = NULL;
	LN_COMCALL(m_vertexBuffer->Lock(0, 0, &v, flags));
	return v;
}

//------------------------------------------------------------------------------
void DX9VertexBuffer::unlock()
{
	LN_COMCALL(m_vertexBuffer->Unlock());
}

//------------------------------------------------------------------------------
void DX9VertexBuffer::onLostDevice()
{
	if (m_usage == ResourceUsage::Dynamic)
	{
		LN_COM_SAFE_RELEASE(m_vertexBuffer);
	}
}

//------------------------------------------------------------------------------
void DX9VertexBuffer::onResetDevice()
{
	if (m_vertexBuffer == nullptr)
	{
		IDirect3DDevice9* dxDevice = m_graphicsDevice->GetIDirect3DDevice9();

		D3DPOOL pool = D3DPOOL_MANAGED;
		DWORD dxUsage = D3DUSAGE_WRITEONLY;
		if (m_usage == ResourceUsage::Dynamic)
		{
			pool = D3DPOOL_DEFAULT;
			dxUsage |= D3DUSAGE_DYNAMIC;
		}

		// 頂点バッファ作成
		LN_COMCALL(dxDevice->CreateVertexBuffer(
			m_bufferSize,
			dxUsage, 0, pool,
			&m_vertexBuffer,
			NULL));
	}
}


//==============================================================================
// DX9VertexDeclaration
//==============================================================================

//------------------------------------------------------------------------------
DX9VertexDeclaration::DX9VertexDeclaration()
	: m_vertexDecl(nullptr)
{
}

//------------------------------------------------------------------------------
DX9VertexDeclaration::~DX9VertexDeclaration()
{
	LN_COM_SAFE_RELEASE(m_vertexDecl);
}

//------------------------------------------------------------------------------
void DX9VertexDeclaration::initialize(DX9GraphicsDevice* device, const VertexElement* elements, int elementsCount)
{
	if (LN_CHECK_ARG(device != nullptr)) return;
	if (LN_CHECK_ARG(elements != nullptr)) return;
	if (LN_CHECK_ARG(elementsCount >= 0)) return;
	memset(m_vertexStrides, 0, sizeof(m_vertexStrides));

	IDirect3DDevice9* dxDevice = device->GetIDirect3DDevice9();

	// D3DVERTEXELEMENT9 を作成して、elements から DirectX用の頂点宣言を作る
	D3DVERTEXELEMENT9* dxelem = LN_NEW D3DVERTEXELEMENT9[elementsCount + 1];
	uint8_t to;
	for (int i = 0; i < elementsCount; ++i)
	{
		int si = elements[i].StreamIndex;
		dxelem[i].Stream = si;
		dxelem[i].Offset = m_vertexStrides[si];
		dxelem[i].Method = D3DDECLMETHOD_DEFAULT;
		dxelem[i].UsageIndex = elements[i].UsageIndex;

		DX9Module::TranslateElementLNToDX(&elements[i], &dxelem[i].Type, &to, &dxelem[i].Usage);
		m_vertexStrides[elements[i].StreamIndex] += to;
	}

	// 最後の要素を示す値で埋める
	dxelem[elementsCount].Stream = 0xff;
	dxelem[elementsCount].Offset = 0;
	dxelem[elementsCount].Type = D3DDECLTYPE_UNUSED;
	dxelem[elementsCount].Method = 0;
	dxelem[elementsCount].Usage = 0;
	dxelem[elementsCount].UsageIndex = 0;

	// 頂点宣言作成
	LN_COMCALL(dxDevice->CreateVertexDeclaration(dxelem, &m_vertexDecl));
	LN_SAFE_DELETE(dxelem);		// 一時バッファはもういらない
}

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
