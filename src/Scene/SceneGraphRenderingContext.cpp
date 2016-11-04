
#include "../Internal.h"
#include <Lumino/Scene/Camera.h>
#include <Lumino/Scene/SceneGraphRenderingContext.h>
#include "../Graphics/SpriteRenderer.h"
#include "../Graphics/Text/TextRenderer.h"
#include "SceneGraphManager.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//==============================================================================
// SceneGraphRenderingContext
//==============================================================================

//------------------------------------------------------------------------------
SceneGraphRenderingContext::SceneGraphRenderingContext()
	: m_currentCamera(nullptr)
	, m_spriteRenderer(nullptr)
	, m_textRenderer(nullptr)
{
}

//------------------------------------------------------------------------------
SceneGraphRenderingContext::~SceneGraphRenderingContext()
{
}

//------------------------------------------------------------------------------
void SceneGraphRenderingContext::Initialize(SceneGraphManager* manager)
{
	RenderingContext::Initialize(manager->GetGraphicsManager());

	m_spriteRenderer = RefPtr<detail::SpriteRenderer>::MakeRef(manager->GetGraphicsManager(), 2048);	// TODO

	m_textRenderer = RefPtr<detail::TextRenderer>::MakeRef();
	m_textRenderer->Initialize(manager->GetGraphicsManager());
}

//------------------------------------------------------------------------------
void SceneGraphRenderingContext::SetCurrentCamera(Camera* camera)
{
	if (m_currentCamera != nullptr) Flush();
	m_currentCamera = camera;
}

//------------------------------------------------------------------------------
void SceneGraphRenderingContext::DrawSprite2D(
	const Matrix& transform,
	const SizeF& size,
	const Vector2& anchorRatio,
	Texture* texture,
	const RectF& srcRect,
	const Color& color)
{
	NorityStartDrawing(m_spriteRenderer);
	m_spriteRenderer->SetTransform(transform);
	m_spriteRenderer->DrawRequest2D(Vector3::Zero, Vector2(size.width, size.height), anchorRatio, texture, srcRect, color);
}

//------------------------------------------------------------------------------
void SceneGraphRenderingContext::DrawSprite3D(
	const Matrix& transform,
	const SizeF& size,
	const Vector2& anchorRatio,
	Texture* texture,
	const RectF& srcRect,
	const Color& color,
	SpriteBaseDirection baseDirection)
{
	NorityStartDrawing(m_spriteRenderer);
	m_spriteRenderer->SetTransform(transform);
	m_spriteRenderer->DrawRequest3D(Vector3::Zero, Vector2(size.width, size.height), anchorRatio, texture, srcRect, color, baseDirection);
}

//------------------------------------------------------------------------------
void SceneGraphRenderingContext::OnDrawGlyphRun(const Matrix& transform, GlyphRun* glyphRun, const Point& point)
{
	NorityStartDrawing(m_textRenderer);
	m_textRenderer->DrawGlyphRun(transform, point, glyphRun);
}

//------------------------------------------------------------------------------
void SceneGraphRenderingContext::OnStateFlush(detail::IRendererPloxy* activeRenderer)
{
	RenderingContext::OnStateFlush(activeRenderer);

	// SpriteRenderer
	if (activeRenderer == m_spriteRenderer)
	{
		const SizeI& size = GetRenderTarget(0)->GetSize();
		m_spriteRenderer->SetViewInfo(SizeF((float)size.width, (float)size.height), m_currentCamera->GetViewMatrix(), m_currentCamera->GetProjectionMatrix());
		m_spriteRenderer->SetState(GetRenderState());
	}
	// TextRenderer
	if (activeRenderer == m_textRenderer)
	{
		const SizeI& size = GetRenderTarget(0)->GetSize();
		m_textRenderer->SetViewInfo(m_currentCamera->GetViewProjectionMatrix(), size);
		m_textRenderer->SetState(nullptr, nullptr);
	}
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
