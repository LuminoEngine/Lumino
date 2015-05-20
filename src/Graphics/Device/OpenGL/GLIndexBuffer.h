
#pragma once 

#include "../DeviceInterface.h"
#include "../../../../include/Lumino/Graphics/GraphicsDevice.h"

namespace Lumino
{
namespace Graphics
{
namespace Device
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
	void Create(int indexCount, const void* initialData, IndexBufferFormat format, DeviceResourceUsage usage);

	/// 頂点バッファオブジェクトの取得
	GLuint GetIndexBufferObject() const { return m_indexBufferObject; }

public:
	virtual IndexBufferFormat GetFormat() const { return m_format; }
	virtual DeviceResourceUsage GetUsage() const { return m_usage; }
	virtual void SetSubData(uint32_t offsetBytes, const void* data, uint32_t dataBytes);
	virtual void OnLostDevice();
	virtual void OnResetDevice();

private:
	GLuint              m_indexBufferObject;
	size_t				m_byteCount;
	IndexBufferFormat	m_format;
	DeviceResourceUsage	m_usage;
	bool				m_inited;
};

} // namespace Device
} // namespace Graphics
} // namespace Lumino
