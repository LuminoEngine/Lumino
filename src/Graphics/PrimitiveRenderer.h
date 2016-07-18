
#pragma once
#include <Lumino/Graphics/Common.h>
#include <Lumino/Graphics/Text/Font.h>
#include "PainterEngine.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

namespace detail
{

enum class PrimitiveRendererMode
{
	LineList,
	TriangleList,
};

class PrimitiveRendererCore
	: public RefObject
{
public:
	struct DrawSquareData
	{
		Vector3 pos[4];
		Vector2 uv[4];
		Color color[4];
	};

	PrimitiveRendererCore();
	~PrimitiveRendererCore();
	void Initialize(GraphicsManager* manager);

	void SetState(const Matrix& world, const Matrix& viewProj, const Size& viewPixelSize, bool useInternalShader, PrimitiveRendererMode mode, Driver::IShader* userShader, Driver::ITexture* texture);
	void DrawLine(const Vector3& from, const Color& fromColor, const Vector3& to, const Color& toColor);
	void DrawSquare(const DrawSquareData& data);
	void Blt(Driver::ITexture* source, Driver::ITexture* dest, const Matrix& transform, Driver::IShader* shader = nullptr);
	void Flush();

private:
	void AddVertex(const Vector3& pos, const Vector2& uv, const Color& color);
	int GetCurrentVertexCount() const { return m_vertexCacheUsed / m_vertexStride; }

	// TODO 頂点宣言とかは外部からもらうようにしたい
	struct Vertex
	{
	public:
		Vector3	position;
		Vector4	color;
		Vector2	uv;

		// 頂点レイアウト
		static VertexElement* Elements()
		{
			static VertexElement elements[] =
			{
				{ 0, VertexElementType_Float3, VertexElementUsage_Position, 0 },
				{ 0, VertexElementType_Float4, VertexElementUsage_Color, 0 },
				{ 0, VertexElementType_Float2, VertexElementUsage_TexCoord, 0 },
			};
			return elements;
		}
		static const int ElementCount = 3;
	};

	GraphicsManager*		m_manager;
	Driver::IRenderer*		m_renderer;
	RefPtr<Driver::IVertexDeclaration>	m_vertexDeclaration;
	Driver::IVertexBuffer*	m_vertexBuffer;
	Driver::IIndexBuffer*	m_indexBuffer;
	RefPtr<Driver::IVertexDeclaration>	m_vertexDeclarationForBlt;
	Driver::IVertexBuffer*	m_vertexBufferForBlt;
	ByteBuffer				m_vertexCache;
	size_t					m_vertexCacheUsed;
	size_t					m_vertexStride;
	CacheBuffer<uint16_t>	m_indexCache;
	Driver::ITexture*		m_foreTexture;
	PrimitiveRendererMode	m_mode;
	Driver::IShader*		m_userShader;
	bool					m_useInternalShader;

	Matrix					m_worldMatrix;
	Matrix					m_viewProjMatrix;
	Vector4					m_pixelStep;

	struct
	{
		Driver::IShader*			shader;
		Driver::IShaderTechnique*	technique;
		Driver::IShaderPass*		pass;
		Driver::IShaderVariable*	varWorldMatrix;
		Driver::IShaderVariable*	varViewProjMatrix;
		Driver::IShaderVariable*	varPixelStep;
		Driver::IShaderVariable*	varTexture;		// TODO: 使ってない？

	} m_shader;

	//struct
	//{
	//	Driver::IShader*			shader;
	//	Driver::IShaderTechnique*	technique;
	//	Driver::IShaderPass*		pass;
	//	Driver::IShaderVariable*	varPixelStep;
	//	Driver::IShaderVariable*	varTexture;

	//} m_shaderForBlt;
};

class PrimitiveRenderer
	: public RefObject
	, public detail::IRendererPloxy
{
public:
	PrimitiveRenderer();
	~PrimitiveRenderer();
	void Initialize(GraphicsManager* manager);

	void SetTransform(const Matrix& matrix);
	void SetViewProjMatrix(const Matrix& matrix);
	void SetViewPixelSize(const Size& size);
	void SetUseInternalShader(bool useInternalShader);	// TODO: いらないかも
	void SetUserShader(Shader* shader);
	void SetTexture(Texture* texture);

	void DrawLine(const Vector3& from, const Color& fromColor, const Vector3& to, const Color& toColor);

	void DrawSquare(
		const Vector3& position1, const Vector2& uv1, const Color& color1,
		const Vector3& position2, const Vector2& uv2, const Color& color2,
		const Vector3& position3, const Vector2& uv3, const Color& color3,
		const Vector3& position4, const Vector2& uv4, const Color& color4);

	void DrawRectangle(const RectF& rect);

	void Blt(Texture* source, RenderTarget* dest, const Matrix& transform, Shader* shader = nullptr);

	virtual void Flush() override;
	virtual void OnActivated() { m_stateModified = true; }
	virtual void OnDeactivated() { Flush(); }

private:
	void SetPrimitiveRendererMode(PrimitiveRendererMode mode);
	void CheckUpdateState();

	GraphicsManager*		m_manager;
	PrimitiveRendererCore*	m_core;
	Matrix					m_transform;
	Matrix					m_viewProj;
	Size					m_viewPixelSize;
	Texture*				m_texture;
	Shader*					m_userShader;
	PrimitiveRendererMode	m_mode;
	bool					m_useInternalShader;
	bool					m_stateModified;
	bool					m_flushRequested;
};

} // namespace detail
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
