
#include "Internal.h"
#include <Lumino/Graphics/RenderingContext.h>
#include <Lumino/Graphics/DrawingContext.h>
#include <Lumino/Graphics/Shader.h>
#include "RendererImpl.h"
#include "Text/TextRenderer.h"
#include "Text/FontManager.h"
#include "GeometryRenderer.h"
#include "FrameRectRenderer.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// DrawingContext
//==============================================================================

//------------------------------------------------------------------------------
DrawingContext::DrawingContext()
	: m_geometryRenderer(nullptr)
	, m_textRenderer(nullptr)
	, m_frameRectRenderer(nullptr)
{
}

//------------------------------------------------------------------------------
DrawingContext::~DrawingContext()
{
	LN_SAFE_RELEASE(m_frameRectRenderer);
	LN_SAFE_RELEASE(m_textRenderer);
	LN_SAFE_RELEASE(m_geometryRenderer);
}

//------------------------------------------------------------------------------
void DrawingContext::Initialize(detail::GraphicsManager* manager)
{
	detail::ContextInterface::Initialize(manager);

	m_geometryRenderer = LN_NEW detail::GeometryRenderer();
	m_geometryRenderer->Initialize(manager);

	m_textRenderer = LN_NEW detail::TextRenderer();
	m_textRenderer->Initialize(manager);

	m_frameRectRenderer = LN_NEW detail::FrameRectRenderer();
	m_frameRectRenderer->Initialize(manager);

	//m_state.renderState.alphaBlendEnabled = true;
	//m_state.renderState.blendOp = BlendOp::Add;
	//m_state.renderState.sourceBlend = BlendFactor::SourceAlpha;
	//m_state.renderState.destinationBlend = BlendFactor::InverseSourceAlpha;
}

//------------------------------------------------------------------------------
void DrawingContext::InitializeFrame(RenderTarget* renderTarget)
{
	//ResetStates();
	m_state.SetRenderTarget(0, renderTarget);
	m_backendState.SetRenderTarget(0, renderTarget);
}

//------------------------------------------------------------------------------
void DrawingContext::Set2DRenderingMode(float minZ, float maxZ)
{
	NorityStateChanging();
	const SizeI& size = m_state.GetRenderTarget(0)->GetSize();
	m_state.viewTransform = Matrix::Identity;
	m_state.projectionTransform = Matrix::MakePerspective2DLH((float)size.width, (float)size.height, minZ, maxZ);
}

//------------------------------------------------------------------------------
void DrawingContext::SetViewProjectionTransform(const Matrix& view, const Matrix& proj)
{
	NorityStateChanging();
	m_state.viewTransform = view;
	m_state.projectionTransform = proj;
}

//------------------------------------------------------------------------------
void DrawingContext::SetRenderTarget(int index, Texture* texture)
{
	if (texture != m_state.GetRenderTarget(index))
	{
		NorityStateChanging();
		m_state.SetRenderTarget(index, texture);
	}
}

//------------------------------------------------------------------------------
void DrawingContext::SetDepthBuffer(Texture* depthBuffer)
{
	if (m_state.depthBuffer != depthBuffer)
	{
		NorityStateChanging();
		m_state.depthBuffer = depthBuffer;
	}
}

//------------------------------------------------------------------------------
void DrawingContext::SetBlendMode(BlendMode mode)
{
	printf("NOTIMPLEMENTED\n");
	//LN_NOTIMPLEMENTED();
	//RenderState newState = m_state.renderState;
	//MakeBlendMode(mode, &newState);
	//if (m_state.renderState != newState)
	//{
	//	NorityStateChanging();
	//	m_state.renderState = newState;
	//}
}

//------------------------------------------------------------------------------
void DrawingContext::SetTransform(const Matrix& matrix)
{
	NorityStateChanging();
	m_state.worldTransform = matrix;
}

//------------------------------------------------------------------------------
void DrawingContext::SetOpacity(float opacity)
{
	if (m_state.opacity != opacity)
	{
		NorityStateChanging();
		m_state.opacity = opacity;
	}
}

//------------------------------------------------------------------------------
void DrawingContext::SetBrush(Brush* brush)
{
	if (m_state.fillBrush != brush)
	{
		NorityStateChanging();
		m_state.fillBrush = brush;
	}
}

//------------------------------------------------------------------------------
void DrawingContext::SetFont(Font* font)
{
	if (m_state.font != font)
	{
		NorityStateChanging();
		m_state.font = font;
	}
}

//------------------------------------------------------------------------------
void DrawingContext::PushState()
{
	m_stateStack.push(m_state);
}

//------------------------------------------------------------------------------
void DrawingContext::PopState()
{
	if (!m_state.Equals(m_stateStack.top()))
	{
		NorityStateChanging();
		m_state = m_stateStack.top();
	}
	m_stateStack.pop();
}






//------------------------------------------------------------------------------
void DrawingContext::Clear(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
	NorityStartDrawing(GetBaseRenderer());
	GetBaseRenderer()->Clear(flags, color, z, stencil);
}

//------------------------------------------------------------------------------
void DrawingContext::MoveTo(const Vector3& point, const Color& color)
{
	NorityStartDrawing(m_geometryRenderer);
	m_geometryRenderer->MoveTo(point, color);
}

//------------------------------------------------------------------------------
void DrawingContext::LineTo(const Vector3& point, const Color& color)
{
	NorityStartDrawing(m_geometryRenderer);
	m_geometryRenderer->LineTo(point, color);
}

//------------------------------------------------------------------------------
void DrawingContext::BezierCurveTo(const Vector3& cp1, const Vector3& cp2, const Vector3& endPt, const Color& color)
{
	NorityStartDrawing(m_geometryRenderer);
	m_geometryRenderer->BezierCurveTo(cp1, cp2, endPt, color);
}

//------------------------------------------------------------------------------
void DrawingContext::ClosePath()
{
	NorityStartDrawing(m_geometryRenderer);
	m_geometryRenderer->ClosePath();
}

//------------------------------------------------------------------------------
//void DrawingContext::DrawPoint(const Vector3& point, const Color& color)
//{
//	NorityStartDrawing(m_geometryRenderer);
//	m_geometryRenderer->DrawPoint(point, color);
//}

//------------------------------------------------------------------------------
void DrawingContext::DrawTriangle(const Vector3& p1, const Color& p1Color, const Vector3& p2, const Color& p2Color, const Vector3& p3, const Color& p3Color)
{
	NorityStartDrawing(m_geometryRenderer);
	m_geometryRenderer->DrawTriangle(p1, p1Color, p2, p2Color, p3, p3Color);
}

//------------------------------------------------------------------------------
void DrawingContext::DrawRectangle(const RectF& rect)
{
	NorityStartDrawing(m_geometryRenderer);

	if (m_state.fillBrush != nullptr &&
		m_state.fillBrush->GetType() == BrushType_Texture &&
		(static_cast<TextureBrush*>(m_state.fillBrush.Get())->GetImageDrawMode() == BrushImageDrawMode::BoxFrame || static_cast<TextureBrush*>(m_state.fillBrush.Get())->GetImageDrawMode() == BrushImageDrawMode::BorderFrame))
	{
		m_frameRectRenderer->Draw(rect);
	}
	else
	{
		m_geometryRenderer->DrawRectangle(rect);
	}
}

//------------------------------------------------------------------------------
void DrawingContext::DrawEllipse(const Vector3& center, const Vector2& radius)
{
	NorityStartDrawing(m_geometryRenderer);
	m_geometryRenderer->DrawEllipse(center, radius);
}

//------------------------------------------------------------------------------
void DrawingContext::DrawTexture(const RectF& rect, Texture* texture, const Rect& srcRect, const Color& color)
{
	NorityStartDrawing(m_geometryRenderer);
	m_geometryRenderer->DrawTexture(rect, texture, srcRect, color);
}

//------------------------------------------------------------------------------
#pragma push_macro("DrawText")
#undef DrawText
void DrawingContext::DrawText(const StringRef& text, const PointF& position)
{
	NorityStartDrawing(m_textRenderer);
	Flush();	// TODO: TextRenderer ‚ª Flush –¢‘Î‰ž‚È‚Ì‚ÅB
	m_textRenderer->DrawString(Matrix::Identity, text.GetBegin(), text.GetLength(), position);
}
void DrawingContext::DrawText(const StringRef& text, const RectF& rect, StringFormatFlags flags)
{
	NorityStartDrawing(m_textRenderer);
	Flush();	// TODO: TextRenderer ‚ª Flush –¢‘Î‰ž‚È‚Ì‚ÅB
	m_textRenderer->DrawString(Matrix::Identity, text.GetBegin(), text.GetLength(), rect, flags);
}
void DrawingContext::LN_AFX_FUNCNAME(DrawText)(const StringRef& text, const PointF& position)
{
	DrawText(text, position);
}
void DrawingContext::LN_AFX_FUNCNAME(DrawText)(const StringRef& text, const RectF& rect, StringFormatFlags flags)
{
	DrawText(text, rect, flags);
}
#pragma pop_macro("DrawText")




//------------------------------------------------------------------------------
void DrawingContext::Flush()
{
	detail::ContextInterface::FlushImplemented();
}

//------------------------------------------------------------------------------
void DrawingContext::InheritStatus(RenderingContext* parent)
{
	LN_ASSERT(parent != nullptr);
	if (!m_state.detail::BasicContextState::Equals(parent->GetCurrentContxtState()))
	{
		m_state.Copy(parent->GetCurrentContxtState());
		NorityStateChanging();
	}
}

//------------------------------------------------------------------------------
bool DrawingContext::OnCheckStateChanged()
{
	return !m_state.Equals(m_backendState);
}

//------------------------------------------------------------------------------
void DrawingContext::OnStateFlush(detail::IRendererPloxy* activeRenderer)
{
	ContextInterface::OnStateFlush(activeRenderer);
	const ContextState& state = m_state;

	SetBasicContextState(state);


	const SizeI& size = state.GetRenderTarget(0)->GetSize();
	Matrix viewProj = state.viewTransform * state.projectionTransform;

	// GeometryRenderer
	if (activeRenderer == m_geometryRenderer)
	{
		m_geometryRenderer->SetTransform(state.worldTransform);
		m_geometryRenderer->SetOpacity(state.opacity);
		m_geometryRenderer->SetBrush(state.fillBrush);
		m_geometryRenderer->SetViewProjection(state.viewTransform, state.projectionTransform, size);
	}

	// TextRenderer
	if (activeRenderer == m_textRenderer)
	{
		m_textRenderer->SetTransform(state.worldTransform);
		m_textRenderer->SetViewInfo(viewProj, size);
		m_textRenderer->SetState((state.font != nullptr) ? state.font : GetManager()->GetFontManager()->GetDefaultFont(), state.fillBrush);
	}


	if (state.fillBrush != nullptr && state.fillBrush->GetType() == BrushType_Texture)
	{
		m_frameRectRenderer->SetViewInfo(viewProj);
		m_frameRectRenderer->SetState(static_cast<TextureBrush*>(state.fillBrush.Get()));
	}

	m_backendState = m_state;
}

//------------------------------------------------------------------------------
void DrawingContext::OnPrimitiveFlush()
{
	ContextInterface::OnPrimitiveFlush();
}

//------------------------------------------------------------------------------
void DrawingContext::OnShaderVariableModified(ShaderVariable* var)
{
	if (m_state.GetShaderPass() != nullptr && m_state.GetShaderPass()->GetOwnerShader() == var->GetOwnerShader())
	{
		NorityStateChanging();
	}
}

//==============================================================================
// DrawingContext::ContextState
//==============================================================================

//------------------------------------------------------------------------------
bool DrawingContext::ContextState::Equals(const ContextState& s) const
{
	if (!BasicContextState::Equals(s)) return false;
	if (worldTransform != s.worldTransform) return false;
	if (viewTransform != s.viewTransform) return false;
	if (projectionTransform != s.projectionTransform) return false;
	if (opacity != s.opacity) return false;
	if (fillBrush != s.fillBrush) return false;
	if (font != s.font) return false;
	return true;
}


LN_NAMESPACE_END
