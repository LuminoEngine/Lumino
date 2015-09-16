
#include "../../../Internal.h"
#include "DX9Module.h"
#include "DX9GraphicsDevice.h"
#include "DX9VertexBuffer.h"

namespace Lumino
{
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Device
{

//=============================================================================
// DX9VertexBuffer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DX9VertexBuffer::DX9VertexBuffer()
	: m_graphicsDevice(NULL)
	, m_vertexBuffer(NULL)
	, m_vertices(NULL)
	, m_vertexDecl(NULL)
	, m_vertexCount(0)
	, m_vertexStride(0)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DX9VertexBuffer::~DX9VertexBuffer()
{
	LN_SAFE_DELETE_ARRAY(m_vertices);
	LN_SAFE_RELEASE(m_vertexBuffer);
	LN_SAFE_RELEASE(m_vertexDecl);
	LN_SAFE_RELEASE(m_graphicsDevice);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9VertexBuffer::Create(DX9GraphicsDevice* device, const VertexElement* elements, int elementsCount, int vertexCount, const void* initialData, DeviceResourceUsage usage)
{
	LN_REFOBJ_SET(m_graphicsDevice, device);
	m_vertexCount = vertexCount;

	IDirect3DDevice9* dxDevice = m_graphicsDevice->GetIDirect3DDevice9();

	// 頂点宣言の数を調べる
	//int elem_size = 0;
	//while (1)
	//{
	//	if (elements[elem_size].Type == VertexElementType_Unknown) {
	//		break;
	//	}
	//	++elem_size;
	//}
	//LN_THROW_Argument((elem_size != 0), "elements size is 0.");

	// D3DVERTEXELEMENT9 を作成して、elements から DirectX用の頂点宣言を作る
	D3DVERTEXELEMENT9* dxelem = LN_NEW D3DVERTEXELEMENT9[elementsCount + 1];
	uint8_t offset = 0;
	uint8_t to;
	for (int i = 0; i < elementsCount; ++i)
	{
		dxelem[i].Stream = 0;
		dxelem[i].Offset = offset;
		dxelem[i].Method = D3DDECLMETHOD_DEFAULT;
		dxelem[i].UsageIndex = elements[i].UsageIndex;

		DX9Module::TranslateElementLNToDX(&elements[i], &dxelem[i].Type, &to, &dxelem[i].Usage);
		offset += to;
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

	// 頂点ひとつ分のサイズ
	m_vertexStride = offset;

	if (usage == DeviceResourceUsage_Dynamic)
	{
		m_vertices = LN_NEW byte_t[m_vertexStride * m_vertexCount];
	}
	else
	{
		// 頂点バッファ作成
		LN_COMCALL(dxDevice->CreateVertexBuffer(
			m_vertexStride * m_vertexCount,
			0, 0,
			D3DPOOL_MANAGED,
			&m_vertexBuffer,
			NULL));
	}

	// 頂点として設定するデータがある場合
	if (initialData != NULL)
	{
		if (m_vertexBuffer != NULL) {
			void* v = NULL;
			LN_COMCALL(m_vertexBuffer->Lock(0, 0, &v, D3DLOCK_DISCARD));
			memcpy_s(v, m_vertexStride * m_vertexCount, initialData, m_vertexStride * m_vertexCount);
			LN_COMCALL(m_vertexBuffer->Unlock());
		}
		else {
			memcpy_s(m_vertices, m_vertexStride * m_vertexCount, initialData, m_vertexStride * m_vertexCount);
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9VertexBuffer::SetSubData(uint32_t offsetBytes, const void* data, uint32_t dataBytes)
{
	byte_t* buf = (byte_t*)Lock();
	memcpy(buf + offsetBytes, data, dataBytes);	// TOOD: IndexBuffer と同じサイズチェック
	Unlock();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void* DX9VertexBuffer::Lock()
{
	if (m_vertexBuffer != NULL)
	{
		void* v = NULL;
		LN_COMCALL(m_vertexBuffer->Lock(0, 0, &v, 0));
		return v;
	}
	else
	{
		return m_vertices;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9VertexBuffer::Unlock()
{
	if (m_vertexBuffer != NULL) {
		LN_COMCALL(m_vertexBuffer->Unlock());
	}
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9VertexBuffer::OnLostDevice()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9VertexBuffer::OnResetDevice()
{
}

} // namespace Device
LN_NAMESPACE_GRAPHICS_END
} // namespace Lumino
