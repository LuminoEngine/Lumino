
#include "../../../Internal.h"
#include "GLIndexBuffer.h"

namespace Lumino
{
namespace Graphics
{
namespace Device
{

//=============================================================================
// GLIndexBuffer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLIndexBuffer::GLIndexBuffer()
	: m_indexBufferObject(0)
	, m_byteCount(0)
	, m_format(IndexBufferFormat_UInt16)
	, m_usage(DeviceResourceUsage_Static)
	, m_inited(false)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLIndexBuffer::~GLIndexBuffer()
{
	OnLostDevice();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLIndexBuffer::Create(int indexCount, const void* initialData, IndexBufferFormat format, DeviceResourceUsage usage)
{
	m_format = format;
	int stride = (format == IndexBufferFormat_UInt16) ? 2 : 4;
	m_byteCount = stride * indexCount;
	OnResetDevice();
	if (initialData != NULL) {
		SetSubData(0, initialData, m_byteCount);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLIndexBuffer::SetSubData(uint32_t offsetBytes, const void* data, uint32_t dataBytes)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject); LN_CHECK_GLERROR();
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offsetBytes, dataBytes, data); LN_CHECK_GLERROR();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); LN_CHECK_GLERROR();
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLIndexBuffer::Lock(void** lockedBuffer, size_t* lockedSize)
{
	LN_THROW(0, NotImplementedException);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLIndexBuffer::Unlock()
{
	LN_THROW(0, NotImplementedException);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLIndexBuffer::OnLostDevice()
{
	glDeleteBuffers(1, &m_indexBufferObject); LN_CHECK_GLERROR();
	m_indexBufferObject = 0;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLIndexBuffer::OnResetDevice()
{
	GLenum gl_usage = (m_usage == DeviceResourceUsage_Static) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
	glGenBuffers(1, &m_indexBufferObject); LN_CHECK_GLERROR();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject); LN_CHECK_GLERROR();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_byteCount, NULL, gl_usage);  LN_CHECK_GLERROR();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); LN_CHECK_GLERROR();
}

} // namespace Device
} // namespace Graphics
} // namespace Lumino
