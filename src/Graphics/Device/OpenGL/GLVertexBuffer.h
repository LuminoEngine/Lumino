
#pragma once 
#include "../GraphicsDriverInterface.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{

struct LNGLVertexElement
{
	// 以下は GLShaderPass::GetUsageAttributeIndex() に渡して attribute の location を取得する
	VertexElementUsage	Usage;			///< 要素の使用法
	int					UsageIndex;     ///< 使用法番号

	// 以下は glVertexAttribPointer() に渡す引数
	GLint				Size;			///< データの要素数 (1, 2, 3, 4 のいずれか。Vector3 なら 3 を指定する)
	GLenum				Type;			///< (終端は0)
	GLboolean			Normalized;		///< データを正規化するか (0.0 ～ 1.0 にするか。色を 0～255 で指定していて、GLSL では 0.0～1.0 で使いたいときは true)
	GLsizei				Stride;			///< 頂点1つ分のバイト数 (この要素が1つのバイト数ではないので注意)
	size_t				ByteOffset;		///< 先頭からのバイト数
};

// OpenGL 用の IVertexBuffer の実装
class GLVertexBuffer
	: public IVertexBuffer
{
public:
	GLVertexBuffer();
	virtual ~GLVertexBuffer();

public:
	/// オブジェクト作成
	void Create(const VertexElement* vertexElements, int elementsCount, int vertexCount, const void* initialData, DeviceResourceUsage usage);

	/// 頂点バッファオブジェクトの取得
	GLuint GetGLVertexBuffer() const { return m_glVertexBuffer; }

	/// 頂点宣言の取得
	const Array<LNGLVertexElement>& GetVertexElements() const { return m_vertexElements; }



public:
	virtual size_t GetByteCount() const { return m_byteCount; }
	//virtual DeviceResourceUsage GetUsage() const { return m_format; }
	virtual void SetSubData(uint32_t offsetBytes, const void* data, uint32_t dataBytes);
	virtual void* Lock();
	virtual void Unlock();
	virtual void OnLostDevice();
	virtual void OnResetDevice();

private:
	Array<LNGLVertexElement>	m_vertexElements;
	GLuint					m_glVertexBuffer;
	size_t					m_byteCount;
	byte_t*					m_data;
	GLenum					m_usage;
	DeviceResourceUsage		m_format;
};

// OpenGL 用の IVertexDeclaration の実装
class GLVertexDeclaration
	: public IVertexDeclaration
{
public:
	GLVertexDeclaration();
	virtual ~GLVertexDeclaration();
	void Initialize(const VertexElement* elements, int elementsCount);

	const Array<LNGLVertexElement>& GetVertexElements() const { return m_vertexElements; }

	// 頂点宣言から GL 用の頂点宣言を生成する
	static void CreateGLVertexElements(const VertexElement* vertexElements, int elementsCount, Array<LNGLVertexElement>* outList);

	// 頂点宣言から頂点1つ分のデータサイズ (バイト数) を求める
	static int GetVertexSize(const VertexElement* vertexElements, int elementsCount, int streamIndex);

	// 頂点宣言の型のサイズ (バイト数) を求める
	static int GetVertexElementTypeSize(VertexElementType type);

	// 頂点宣言の型から LNGLVertexElement 用のデータを作る
	static void ConvertDeclTypeLNToGL(VertexElementType type, GLenum* gl_type, GLint* size, GLboolean* normalized);

private:
	Array<LNGLVertexElement>	m_vertexElements;
};

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
