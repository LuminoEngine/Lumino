
#include "../Internal.h"
#include <Lumino/Graphics/SpriteRenderer.h>
#include <Lumino/Scene/SceneGraphRenderingContext.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//=============================================================================
// SceneGraphRenderingContext
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SceneGraphRenderingContext::SceneGraphRenderingContext(GraphicsManager* manager)
	//: DrawingContext()
	: m_currentRenderer(RendererType::None)
	, m_spriteRenderer(nullptr)
{
	m_drawingContext.Initialize(manager);
	m_spriteRenderer = LN_NEW SpriteRenderer(manager, 2048);	// TODO:
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SceneGraphRenderingContext::~SceneGraphRenderingContext()
{
	LN_SAFE_RELEASE(m_spriteRenderer);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DrawingContext* SceneGraphRenderingContext::BeginDrawingContext()
{
	if (m_currentRenderer != RendererType::DrawingContext)
	{
		Flush();
		m_currentRenderer = RendererType::DrawingContext;
	}
	return &m_drawingContext;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SpriteRenderer* SceneGraphRenderingContext::BeginSpriteRendering()
{
	if (m_currentRenderer != RendererType::DrawingContext)
	{
		Flush();
		m_currentRenderer = RendererType::SpriteRenderer;
	}
	return m_spriteRenderer;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SceneGraphRenderingContext::Flush()
{
	m_drawingContext.Flush();
	m_spriteRenderer->Flush();
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
