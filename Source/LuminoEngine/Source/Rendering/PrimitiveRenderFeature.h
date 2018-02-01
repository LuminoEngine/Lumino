
#pragma once
#include <Lumino/Graphics/Common.h>
#include <Lumino/Graphics/ContextInterface.h>
#include <Lumino/Graphics/Text/Font.h>
#include "../Graphics/RenderingCommand.h"

LN_NAMESPACE_BEGIN
struct Vertex;

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
	void initialize(GraphicsManager* manager);

	void setState(PrimitiveRendererMode mode);
	void drawLine(const Vector3& from, const Color& fromColor, const Vector3& to, const Color& toColor);
	void drawSquare(const DrawSquareData& data);
	void flush();

	void requestBuffers(int vertexCount, int indexCount, Vertex** vb, uint16_t** ib, uint16_t* outBeginVertexIndex);

private:
	void addVertex(const Vector3& pos, const Vector2& uv, const Color& color);

	GraphicsManager*		m_manager;
	Driver::IRenderer*		m_renderer;
	Driver::IVertexBuffer*	m_vertexBuffer;
	Driver::IIndexBuffer*	m_indexBuffer;
	CacheBuffer<Vertex>		m_vertexCache;
	CacheBuffer<uint16_t>	m_indexCache;
	PrimitiveRendererMode	m_mode;
};

class PrimitiveRenderFeature
	: public RefObject
	, public detail::IRenderFeature
{
public:
	PrimitiveRenderFeature();
	~PrimitiveRenderFeature();
	void initialize(GraphicsManager* manager);

	void drawLine(const Vector3& from, const Color& fromColor, const Vector3& to, const Color& toColor);

	void drawSquare(
		const Vector3& position1, const Vector2& uv1, const Color& color1,
		const Vector3& position2, const Vector2& uv2, const Color& color2,
		const Vector3& position3, const Vector2& uv3, const Color& color3,
		const Vector3& position4, const Vector2& uv4, const Color& color4);

	void drawRectangle(const Rect& rect);


	template<class TFactory>
	void drawMeshFromFactory(const TFactory& factory, PrimitiveRendererMode mode)
	{
		setPrimitiveRendererMode(mode);
		checkUpdateState();
		PrimitiveRendererCore::DrawSquareData data;

		LN_ENQUEUE_RENDER_COMMAND_2(
			drawMeshFromFactory, m_manager,
			PrimitiveRendererCore*, m_core,
			TFactory, factory,
			{
				Vertex* vb;
				uint16_t* ib;
				uint16_t beginVertexIndex;
				m_core->requestBuffers(factory.getVertexCount(), factory.getIndexCount(), &vb, &ib, &beginVertexIndex);
				factory.setBuffers(vb, ib, IndexBufferFormat::IndexBufferFormat_UInt16, beginVertexIndex);
				factory.generate();
			});

		m_flushRequested = true;
	}


	virtual bool isStandaloneShader() const;
	virtual void flush() override;
	virtual void onActivated();
	virtual void onDeactivated();

private:
	void setPrimitiveRendererMode(PrimitiveRendererMode mode);
	void checkUpdateState();

	GraphicsManager*		m_manager;
	PrimitiveRendererCore*	m_core;
	PrimitiveRendererMode	m_mode;
	bool					m_stateModified;
	bool					m_flushRequested;
};


class BlitRenderer
	: public RefObject
	, public detail::IRenderFeature
{
public:
	BlitRenderer();
	virtual ~BlitRenderer();
	void initialize(GraphicsManager* manager);
	CommonMaterial* getCommonMaterial() const;

	void blit();

protected:
	virtual bool isStandaloneShader() const;
	virtual void flush();
	virtual void onActivated();
	virtual void onDeactivated();

private:
	void blitImpl();

	GraphicsManager*					m_manager;
	Ref<Driver::IVertexBuffer>		m_vertexBuffer;
	Ref<CommonMaterial>					m_commonMaterial;
};

} // namespace detail
LN_NAMESPACE_END
