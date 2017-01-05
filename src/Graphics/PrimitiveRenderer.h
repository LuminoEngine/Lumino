
#pragma once
#include <Lumino/Graphics/Common.h>
#include <Lumino/Graphics/ContextInterface.h>
#include <Lumino/Graphics/Text/Font.h>

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

	void SetState(PrimitiveRendererMode mode);
	void DrawLine(const Vector3& from, const Color& fromColor, const Vector3& to, const Color& toColor);
	void DrawSquare(const DrawSquareData& data);
	void Flush();

private:
	void AddVertex(const Vector3& pos, const Vector2& uv, const Color& color);
	int GetCurrentVertexCount() const { return m_vertexCacheUsed / m_vertexStride; }

	GraphicsManager*		m_manager;
	Driver::IRenderer*		m_renderer;
	Driver::IVertexBuffer*	m_vertexBuffer;
	Driver::IIndexBuffer*	m_indexBuffer;
	ByteBuffer				m_vertexCache;
	size_t					m_vertexCacheUsed;
	size_t					m_vertexStride;
	CacheBuffer<uint16_t>	m_indexCache;
	PrimitiveRendererMode	m_mode;
};

class PrimitiveRenderer
	: public RefObject
	, public detail::IRendererPloxy
{
public:
	PrimitiveRenderer();
	~PrimitiveRenderer();
	void Initialize(GraphicsManager* manager);

	void DrawLine(const Vector3& from, const Color& fromColor, const Vector3& to, const Color& toColor);

	void DrawSquare(
		const Vector3& position1, const Vector2& uv1, const Color& color1,
		const Vector3& position2, const Vector2& uv2, const Color& color2,
		const Vector3& position3, const Vector2& uv3, const Color& color3,
		const Vector3& position4, const Vector2& uv4, const Color& color4);

	void DrawRectangle(const RectF& rect);



	virtual bool IsStandaloneShader() const;
	virtual void Flush() override;
	virtual void OnActivated();
	virtual void OnDeactivated();

private:
	void SetPrimitiveRendererMode(PrimitiveRendererMode mode);
	void CheckUpdateState();

	GraphicsManager*		m_manager;
	PrimitiveRendererCore*	m_core;
	PrimitiveRendererMode	m_mode;
	bool					m_stateModified;
	bool					m_flushRequested;
};


class BlitRenderer
	: public RefObject
	, public detail::IRendererPloxy
{
public:
	BlitRenderer();
	virtual ~BlitRenderer();
	void Initialize(GraphicsManager* manager);
	Material* GetCommonMaterial() const;

	void Blit();

protected:
	virtual bool IsStandaloneShader() const;
	virtual void Flush();
	virtual void OnActivated();
	virtual void OnDeactivated();

private:
	void BlitImpl();

	GraphicsManager*					m_manager;
	RefPtr<Driver::IVertexBuffer>		m_vertexBuffer;
	RefPtr<Material>					m_commonMaterial;
};

} // namespace detail
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
