
#include "../../../Internal.h"
#include "GLVertexBuffer.h"

namespace Lumino
{
namespace Graphics
{
namespace Device
{

//=============================================================================
// GLVertexBuffer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLVertexBuffer::GLVertexBuffer()
	: m_glVertexBuffer(0)
	, m_byteCount(0)
	, m_data(NULL)
	, m_usage(0)
	, m_format(DeviceResourceUsage_Static)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLVertexBuffer::~GLVertexBuffer()
{
	glDeleteBuffers(1, &m_glVertexBuffer);
	LN_SAFE_DELETE_ARRAY(m_data);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLVertexBuffer::Create(const VertexElement* vertexElements, int elementsCount, int vertexCount, const void* initialData, DeviceResourceUsage usage)
{
	// 頂点宣言作成
	CreateGLVertexElements(vertexElements, elementsCount, &m_vertexElements);

	m_format = usage;
	m_byteCount = vertexCount * m_vertexElements[0].Stride;	// OpenGL の API に渡す都合上、Stride はすべて頂点1つ分のサイズが入っている
	m_data = LN_NEW byte_t[m_byteCount];
	if (initialData) {
		memcpy_s(m_data, m_byteCount, initialData, m_byteCount);
	}
	else {
		memset(m_data, 0, m_byteCount);
	}

	if (m_format == DeviceResourceUsage_Dynamic) {
		m_usage = GL_DYNAMIC_DRAW;
	}
	else {
		m_usage = GL_STATIC_DRAW;
	}

	OnResetDevice();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLVertexBuffer::CreateGLVertexElements(const VertexElement* vertexElements, int elementsCount, Array<LNGLVertexElement>* outList)
{
	outList->Reserve(elementsCount);

	int vertexSize = GetVertexSize(vertexElements, elementsCount, 0);
	int totalSize = 0;
	for (int i = 0; i < elementsCount; ++i)
	{
		LNGLVertexElement elm;
		elm.Usage = vertexElements[i].Usage;
		elm.UsageIndex = vertexElements[i].UsageIndex;

		ConvertDeclTypeLNToGL(
			vertexElements[i].Type,
			&elm.Type,
			&elm.Size,
			&elm.Normalized);

		elm.Stride = vertexSize;
		elm.ByteOffset = totalSize;
		outList->Add(elm);

		totalSize += GetVertexElementTypeSize(vertexElements[i].Type);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int GLVertexBuffer::GetVertexSize(const VertexElement* vertexElements, int elementsCount, int streamIndex)
{
	int size = 0;
	for (int i = 0; i < elementsCount; ++i)
	{
		if (vertexElements[i].StreamIndex == streamIndex) {
			size += GetVertexElementTypeSize(vertexElements[i].Type);
		}
	}
	return size;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int GLVertexBuffer::GetVertexElementTypeSize(VertexElementType type)
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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLVertexBuffer::ConvertDeclTypeLNToGL(VertexElementType type, GLenum* gl_type, GLint* size, GLboolean* normalized)
{
	static const struct _FormatType
	{
		GLenum		Type;
		GLint		Size;
		GLboolean	Normalize;
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

	*gl_type	= formatTable[type].Type;
	*size		= formatTable[type].Size;
	*normalized	= formatTable[type].Normalize;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLVertexBuffer::SetSubData(uint32_t offsetBytes, const void* data, uint32_t dataBytes)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer); LN_CHECK_GLERROR();
	glBufferSubData(GL_ARRAY_BUFFER, offsetBytes, dataBytes, data); LN_CHECK_GLERROR();
	glBindBuffer(GL_ARRAY_BUFFER, 0); LN_CHECK_GLERROR();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void* GLVertexBuffer::Lock()
{
	/*	glMapBuffer は使わない。
	 *	これは OpenGL ES では READ モードでロックできないため。
	 *	つまり 一度 OpenGL にデータを送ってしまった後、その値を得ることができなくなってしまう。
	 *	通常の Lock の用途なら Write だけでほぼ問題ないが、
	 *	デバイスロストしたときの復帰ではバックアップから GL の頂点バッファを作り直さなければならず、
	 *	結局こちら側でずっと握っていた方が色々と都合が良かったりする。
	 */
	return m_data;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLVertexBuffer::Unlock()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer); LN_CHECK_GLERROR();
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_byteCount, m_data); LN_CHECK_GLERROR();
	glBindBuffer(GL_ARRAY_BUFFER, 0); LN_CHECK_GLERROR();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLVertexBuffer::OnLostDevice()
{
	glDeleteBuffers(1, &m_glVertexBuffer); LN_CHECK_GLERROR();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLVertexBuffer::OnResetDevice()
{
	glGenBuffers(1, &m_glVertexBuffer); LN_CHECK_GLERROR();
	glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer); LN_CHECK_GLERROR();
	glBufferData(GL_ARRAY_BUFFER, m_byteCount, m_data, m_usage);  LN_CHECK_GLERROR();
	glBindBuffer(GL_ARRAY_BUFFER, 0); LN_CHECK_GLERROR();
}

} // namespace Device
} // namespace Graphics
} // namespace Lumino
