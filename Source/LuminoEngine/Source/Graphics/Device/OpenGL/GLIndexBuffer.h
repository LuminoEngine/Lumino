
#pragma once 
#include "../GraphicsDriverInterface.h"
#include "GLCommon.h"

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
	void create(int indexCount, const void* initialData, IndexBufferFormat format, ResourceUsage usage);

	/// 頂点バッファオブジェクトの取得
	GLuint getIndexBufferObject() const { return m_indexBufferObject; }

public:
	virtual size_t getByteCount() const { return m_byteCount; }
	virtual IndexBufferFormat getFormat() const { return m_format; }
	virtual ResourceUsage getUsage() const { return m_usage; }
	virtual void setSubData(uint32_t offsetBytes, const void* data, uint32_t dataBytes);
	virtual void lock(void** lockedBuffer, size_t* lockedSize);
	virtual void unlock();
	virtual void onLostDevice();
	virtual void onResetDevice();

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
