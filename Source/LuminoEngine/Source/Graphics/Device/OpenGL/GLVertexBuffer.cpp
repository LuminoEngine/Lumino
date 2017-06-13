
#include "../../Internal.h"
#include <Lumino/Graphics/GraphicsException.h>
#include "GLVertexBuffer.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{

//==============================================================================
// GLVertexBuffer
//==============================================================================

//------------------------------------------------------------------------------
GLVertexBuffer::GLVertexBuffer()
	: m_glVertexBuffer(0)
	, m_byteCount(0)
	, m_data(NULL)
	, m_usage(0)
	, m_format(ResourceUsage::Static)
{
}

//------------------------------------------------------------------------------
GLVertexBuffer::~GLVertexBuffer()
{
	glDeleteBuffers(1, &m_glVertexBuffer);
	LN_SAFE_DELETE_ARRAY(m_data);
}

//------------------------------------------------------------------------------
void GLVertexBuffer::create(size_t bufferSize, const void* initialData, ResourceUsage usage)
{
	m_format = usage;
	m_byteCount = bufferSize;
	m_data = LN_NEW byte_t[m_byteCount];
	if (initialData) {
		memcpy_s(m_data, m_byteCount, initialData, m_byteCount);
	}
	else {
		memset(m_data, 0, m_byteCount);
	}

	if (m_format == ResourceUsage::Dynamic) {
		m_usage = GL_DYNAMIC_DRAW;
	}
	else {
		m_usage = GL_STATIC_DRAW;
	}

	onResetDevice();
}

//------------------------------------------------------------------------------
void GLVertexBuffer::setSubData(uint32_t offsetBytes, const void* data, uint32_t dataBytes)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer); LN_CHECK_GLERROR();
	glBufferSubData(GL_ARRAY_BUFFER, offsetBytes, dataBytes, data); LN_CHECK_GLERROR();
	glBindBuffer(GL_ARRAY_BUFFER, 0); LN_CHECK_GLERROR();
}

//------------------------------------------------------------------------------
void* GLVertexBuffer::lock()
{
	/*	glMapBuffer は使わない。
	 *	これは OpenGL ES では READ モードでロックできないため。
	 *	つまり 一度 OpenGL にデータを送ってしまった後、その値を得ることができなくなってしまう。
	 *	通常の lock の用途なら Write だけでほぼ問題ないが、
	 *	デバイスロストしたときの復帰ではバックアップから GL の頂点バッファを作り直さなければならず、
	 *	結局こちら側でずっと握っていた方が色々と都合が良かったりする。
	 */
	return m_data;
}

//------------------------------------------------------------------------------
void GLVertexBuffer::unlock()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer); LN_CHECK_GLERROR();
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_byteCount, m_data); LN_CHECK_GLERROR();
	glBindBuffer(GL_ARRAY_BUFFER, 0); LN_CHECK_GLERROR();
}

//------------------------------------------------------------------------------
void GLVertexBuffer::onLostDevice()
{
	glDeleteBuffers(1, &m_glVertexBuffer); LN_CHECK_GLERROR();
}

//------------------------------------------------------------------------------
void GLVertexBuffer::onResetDevice()
{
	glGenBuffers(1, &m_glVertexBuffer); LN_CHECK_GLERROR();
	glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer); LN_CHECK_GLERROR();
	glBufferData(GL_ARRAY_BUFFER, m_byteCount, m_data, m_usage);  LN_CHECK_GLERROR();
	glBindBuffer(GL_ARRAY_BUFFER, 0); LN_CHECK_GLERROR();
}

//==============================================================================
// GLVertexDeclaration
//==============================================================================

//------------------------------------------------------------------------------
GLVertexDeclaration::GLVertexDeclaration()
	: m_vertexElements()
{
}

//------------------------------------------------------------------------------
GLVertexDeclaration::~GLVertexDeclaration()
{
}

//------------------------------------------------------------------------------
void GLVertexDeclaration::initialize(const VertexElement* elements, int elementsCount)
{
	if (LN_CHECK_ARG(elements != nullptr)) return;
	if (LN_CHECK_ARG(elementsCount >= 0)) return;

	// 頂点宣言作成
	createGLVertexElements(elements, elementsCount, &m_vertexElements);
}

//------------------------------------------------------------------------------
void GLVertexDeclaration::createGLVertexElements(const VertexElement* vertexElements, int elementsCount, List<LNGLVertexElement>* outList)
{
	outList->reserve(elementsCount);

	int vertexSize = getVertexSize(vertexElements, elementsCount, 0);
	int totalSize = 0;
	for (int i = 0; i < elementsCount; ++i)
	{
		LNGLVertexElement elm;
		elm.Usage = vertexElements[i].Usage;
		elm.UsageIndex = vertexElements[i].UsageIndex;

		convertDeclTypeLNToGL(
			vertexElements[i].Type,
			&elm.Type,
			&elm.Size,
			&elm.Normalized);

		elm.Stride = vertexSize;
		elm.ByteOffset = totalSize;
		outList->add(elm);

		totalSize += getVertexElementTypeSize(vertexElements[i].Type);
	}
}

//------------------------------------------------------------------------------
int GLVertexDeclaration::getVertexSize(const VertexElement* vertexElements, int elementsCount, int streamIndex)
{
	int size = 0;
	for (int i = 0; i < elementsCount; ++i)
	{
		if (vertexElements[i].StreamIndex == streamIndex) {
			size += getVertexElementTypeSize(vertexElements[i].Type);
		}
	}
	return size;
}

//------------------------------------------------------------------------------
int GLVertexDeclaration::getVertexElementTypeSize(VertexElementType type)
{
	switch (type)
	{
		case VertexElementType_Float1:	return sizeof(float);
		case VertexElementType_Float2:	return sizeof(float) * 2;
		case VertexElementType_Float3:	return sizeof(float) * 3;
		case VertexElementType_Float4:	return sizeof(float) * 4;
		case VertexElementType_Ubyte4:	return sizeof(unsigned char) * 4;
		case VertexElementType_Color4:	return sizeof(unsigned char) * 4;
		case VertexElementType_Short2:	return sizeof(short) * 2;
		case VertexElementType_Short4:	return sizeof(short) * 4;
	}
	LN_THROW(0, ArgumentException);
	return 0;
}

//------------------------------------------------------------------------------
void GLVertexDeclaration::convertDeclTypeLNToGL(VertexElementType type, GLenum* gl_type, GLint* size, GLboolean* normalized)
{
	static const struct _FormatType
	{
		GLenum		Type;
		GLint		Size;
		GLboolean	normalize;
	} formatTable[] =
	{
		{ 0,				0,	GL_FALSE },	// VertexElementType_Unknown
		{ GL_FLOAT,			1,	GL_FALSE },	// VertexElementType_Float1
		{ GL_FLOAT,			2,	GL_FALSE },	// VertexElementType_Float2
		{ GL_FLOAT,			3,	GL_FALSE },	// VertexElementType_Float3
		{ GL_FLOAT,			4,	GL_FALSE },	// VertexElementType_Float4
		{ GL_UNSIGNED_BYTE, 4,	GL_FALSE },	// VertexElementType_Ubyte4
		{ GL_UNSIGNED_BYTE, 4,	GL_TRUE },	// VertexElementType_Color4
		{ GL_SHORT,			2,	GL_FALSE },	// VertexElementType_Short2
		{ GL_SHORT,			4,	GL_FALSE },	// VertexElementType_Short4
	};
	assert(LN_ARRAY_SIZE_OF(formatTable) == VertexElementType_Max);
	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribPointer.xml
	// GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_FLOAT, GL_DOUBLE

	*gl_type = formatTable[type].Type;
	*size = formatTable[type].Size;
	*normalized = formatTable[type].normalize;
}

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
