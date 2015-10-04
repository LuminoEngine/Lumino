
#pragma once 
#include "../GraphicsDriverInterface.h"

namespace Lumino
{
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{
class DX9GraphicsDevice;

class DX9VertexBuffer
	: public IVertexBuffer
{
public:
	DX9VertexBuffer();
	virtual ~DX9VertexBuffer();

public:

	/// 作成
	void Create(DX9GraphicsDevice* device, const VertexElement* elements, int elementsCount, int vertexCount, const void* initialData, DeviceResourceUsage usage);

	/// 作成 (渡された IDirect3DVertexBuffer9 と D3DVERTEXELEMENT9 を参照する)
	//void create(DX9GraphicsDevice* device, IDirect3DVertexBuffer9* device_obj, D3DVERTEXELEMENT9* layout);

	/// IDirect3DVertexBuffer9 へのポインタ取得
	IDirect3DVertexBuffer9* GetDxVertexBuffer() { return m_vertexBuffer; }

	/// IDirect3DVertexDeclaration9 へのポインタ取得
	IDirect3DVertexDeclaration9* GetDxVertexDeclaration() { return m_vertexDecl; }

	/// 頂点数
	int GetVertexCount() const { return m_vertexCount; }

	/// 頂点ひとつ分のサイズ
	int GetVertexStride() const { return m_vertexStride; }


	DeviceResourceUsage GetUsage() const { return (m_vertices != NULL) ? DeviceResourceUsage_Dynamic : DeviceResourceUsage_Static; }

public:
	virtual size_t GetByteCount() const { return m_vertexStride * m_vertexCount; }
	virtual void SetSubData(uint32_t offsetBytes, const void* data, uint32_t dataBytes);
	virtual void* Lock();
	virtual void Unlock();
	virtual void OnLostDevice();
	virtual void OnResetDevice();

private:
	DX9GraphicsDevice*				m_graphicsDevice;
	IDirect3DVertexBuffer9*	        m_vertexBuffer;		///< 頂点バッファ
	byte_t*					        m_vertices;			///< 動的な頂点バッファとして作った場合はこっちに配列が作られる ( union でもいいかな・・・ )
	IDirect3DVertexDeclaration9*	m_vertexDecl;		///< 頂点宣言
	int								m_vertexCount;		///< 頂点の数
	int								m_vertexStride;	    ///< 頂点ひとつ分のサイズ
};

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
} // namespace Lumino
