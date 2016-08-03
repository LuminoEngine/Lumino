
#pragma once 
#include "../GraphicsDriverInterface.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{
class DX9GraphicsDevice;

// DirectX9 用の IVertexBuffer の実装
class DX9VertexBuffer
	: public IVertexBuffer
{
public:
	DX9VertexBuffer();
	virtual ~DX9VertexBuffer();

	void Create(DX9GraphicsDevice* device, size_t bufferSize, const void* initialData, ResourceUsage usage);
	IDirect3DVertexBuffer9* GetDxVertexBuffer() { return m_vertexBuffer; }

	// IVertexBuffer interface
	virtual size_t GetByteCount() const { return m_bufferSize; }
	virtual void SetSubData(uint32_t offsetBytes, const void* data, uint32_t dataBytes);
	virtual void* Lock();
	virtual void Unlock();
	virtual void OnLostDevice();
	virtual void OnResetDevice();

private:
	DX9GraphicsDevice*			m_graphicsDevice;
	IDirect3DVertexBuffer9*		m_vertexBuffer;	
	size_t						m_bufferSize;
	ResourceUsage				m_usage;
};

// DirectX9 用の IVertexDeclaration の実装
class DX9VertexDeclaration
	: public IVertexDeclaration
{
public:
	DX9VertexDeclaration();
	virtual ~DX9VertexDeclaration();
	void Initialize(DX9GraphicsDevice* device, const VertexElement* elements, int elementsCount);

	IDirect3DVertexDeclaration9* GetDxVertexDeclaration() const { return m_vertexDecl; }
	int GetVertexStride(int streamIndex) const { return m_vertexStrides[streamIndex]; }


	// IDeviceObject interface
	virtual void OnLostDevice() override {};
	virtual void OnResetDevice() override {};

private:
	IDirect3DVertexDeclaration9*	m_vertexDecl;
	int								m_vertexStrides[MaxVertexStreams];
};

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
