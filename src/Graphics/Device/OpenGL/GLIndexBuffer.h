
#pragma once 
#include "../GraphicsDriverInterface.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{

/// OpenGL 用の IIndexBuffer の実装
class GLIndexBuffer
	: public IIndexBuffer
{
public:
	GLIndexBuffer();
	virtual ~GLIndexBuffer();

public:
	/// オブジェクト作成
	void Create(int indexCount, const void* initialData, IndexBufferFormat format, ResourceUsage usage);

	/// 頂点バッファオブジェクトの取得
	GLuint GetIndexBufferObject() const { return m_indexBufferObject; }

public:
	virtual IndexBufferFormat GetFormat() const { return m_format; }
	virtual ResourceUsage GetUsage() const { return m_usage; }
	virtual void SetSubData(uint32_t offsetBytes, const void* data, uint32_t dataBytes);
	virtual void Lock(void** lockedBuffer, size_t* lockedSize);
	virtual void Unlock();
	virtual void OnLostDevice();
	virtual void OnResetDevice();

private:
	GLuint              m_indexBufferObject;
	size_t				m_byteCount;
	IndexBufferFormat	m_format;
	ResourceUsage		m_usage;
	bool				m_inited;
};

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
