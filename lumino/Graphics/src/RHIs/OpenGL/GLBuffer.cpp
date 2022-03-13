#include "GLBuffer.hpp"

namespace ln {
namespace detail {

//==============================================================================
// GLVertexBuffer

GLVertexBuffer::GLVertexBuffer()
	: m_glVertexBuffer(0)
	//, m_byteCount(0)
	//, m_data(NULL)
	, m_usage(GraphicsResourceUsage::Static)
	//, m_format(GraphicsResourceUsage::Static)
	, m_size(0)
{
}

GLVertexBuffer::~GLVertexBuffer()
{
}

void GLVertexBuffer::init(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData)
{
	//m_format = usage;
	m_usage = usage;
	m_size = bufferSize;
	//m_byteCount = bufferSize;
	//m_data = LN_NEW byte_t[m_byteCount];
	//if (initialData) {
	//	memcpy_s(m_data, m_byteCount, initialData, m_byteCount);
	//}
	//else {
	//	memset(m_data, 0, m_byteCount);
	//}

	//if (m_format == GraphicsResourceUsage::Dynamic) {
	//	m_usage = GL_DYNAMIC_DRAW;
	//}
	//else {
	//	m_usage = GL_STATIC_DRAW;
	//}

	GL_CHECK(glGenBuffers(1, &m_glVertexBuffer));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer));
	GL_CHECK(glBufferData(GL_ARRAY_BUFFER, bufferSize, initialData, (m_usage == GraphicsResourceUsage::Static) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void GLVertexBuffer::dispose()
{
	if (m_glVertexBuffer) {
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GL_CHECK(glDeleteBuffers(1, &m_glVertexBuffer));
		m_glVertexBuffer = 0;
	}

	RHIResource::dispose();
}

void GLVertexBuffer::setSubData(size_t offset, const void* data, size_t length)
{
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer));
	GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, offset, length, data));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void* GLVertexBuffer::map(uint32_t offset, uint32_t size)
{
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer));
	void* buffer;
	//GL_CHECK(buffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	//GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

	// https://developer.apple.com/jp/documentation/3DDrawing/Conceptual/OpenGLES_ProgrammingGuide/TechniquesforWorkingwithVertexData/TechniquesforWorkingwithVertexData.html


	GL_CHECK(buffer = glMapBufferRange(GL_ARRAY_BUFFER, offset, size, GL_MAP_WRITE_BIT));


	//GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
	//glBufferStorage(GL_ARRAY_BUFFER, DYN_VB_SIZE, NULL, flags);
	//m_dyn_vb_ptr = glMapBufferRange(GL_ARRAY_BUFFER, 0, DYN_VB_SIZE, flags);

	return buffer;
}

void GLVertexBuffer::unmap()
{
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer));
	GL_CHECK(glUnmapBuffer(GL_ARRAY_BUFFER));
	//GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

//------------------------------------------------------------------------------
//void* GLVertexBuffer::lock()
//{
//	return m_data;
//}
//
////------------------------------------------------------------------------------
//void GLVertexBuffer::unlock()
//{
//	//glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer);
//	//if (LN_ENSURE_GLERROR()) return;
//	//glBufferSubData(GL_ARRAY_BUFFER, 0, m_byteCount, m_data);
//	//if (LN_ENSURE_GLERROR()) return;
//	//glBindBuffer(GL_ARRAY_BUFFER, 0);
//	//if (LN_ENSURE_GLERROR()) return;
//}

////------------------------------------------------------------------------------
//void GLVertexBuffer::onLostDevice()
//{
//	glDeleteBuffers(1, &m_glVertexBuffer);
//	if (LN_ENSURE_GLERROR()) return;
//}
//
////------------------------------------------------------------------------------
//void GLVertexBuffer::onResetDevice()
//{
//	glGenBuffers(1, &m_glVertexBuffer);
//	if (LN_ENSURE_GLERROR()) return;
//	glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer);
//	if (LN_ENSURE_GLERROR()) return;
//	glBufferData(GL_ARRAY_BUFFER, m_byteCount, m_data, m_usage);
//	if (LN_ENSURE_GLERROR()) return;
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	if (LN_ENSURE_GLERROR()) return;
//}

//==============================================================================
// GLIndexBuffer

GLIndexBuffer::GLIndexBuffer()
	: m_indexBufferId(0)
	, m_format(IndexBufferFormat::UInt16)
	, m_usage(GraphicsResourceUsage::Static)
	, m_size(0)
{
}

GLIndexBuffer::~GLIndexBuffer()
{
}

void GLIndexBuffer::init(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData)
{
	m_format = format;
	m_usage = usage;
	int stride = (m_format == IndexBufferFormat::UInt16) ? 2 : 4;
	m_size = stride * indexCount;

	GL_CHECK(glGenBuffers(1, &m_indexBufferId));
	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId));
	GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, initialData, (m_usage == GraphicsResourceUsage::Static) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW));
	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void GLIndexBuffer::dispose()
{
	if (m_indexBufferId) {
		GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		GL_CHECK(glDeleteBuffers(1, &m_indexBufferId));
		m_indexBufferId = 0;
	}

	RHIResource::dispose();
}

void GLIndexBuffer::setSubData(size_t offset, const void* data, size_t length)
{
	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId));
	GL_CHECK(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, length, data));
	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void* GLIndexBuffer::map(uint32_t offset, uint32_t size)
{
	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId));
	void* buffer;
	//GL_CHECK(buffer = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
	GL_CHECK(buffer = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, offset, m_size, GL_MAP_WRITE_BIT));
	return buffer;
}

void GLIndexBuffer::unmap()
{
	GL_CHECK(glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER));
	//GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

} // namespace detail
} // namespace ln

