
#include "../Internal.h"
#include <Lumino/Graphics/GraphicsContext.h>
#include <Lumino/Rendering/Rendering.h>
#include "SceneGraphManager.h"
#include <Lumino/Scene/SceneGraph.h>
#include <Lumino/Scene/TextBlock.h>
#include "../Documents/DocumentElements.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//==============================================================================
// TextBlock2DComponent
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(TextBlock2DComponent, VisualComponent);

//------------------------------------------------------------------------------
TextBlock2DComponentPtr TextBlock2DComponent::create()
{
	auto ptr = TextBlock2DComponentPtr::makeRef();
	ptr->initialize();
	return ptr;
}

//------------------------------------------------------------------------------
TextBlock2DComponentPtr TextBlock2DComponent::create(const StringRef& text)
{
	auto ptr = TextBlock2DComponentPtr::makeRef();
	ptr->initialize();
	ptr->setText(text);
	return ptr;
}

//------------------------------------------------------------------------------
TextBlock2DComponent::TextBlock2DComponent()
	: VisualComponent()
	, m_paragraph(nullptr)
{
}

//------------------------------------------------------------------------------
TextBlock2DComponent::~TextBlock2DComponent()
{
}

//------------------------------------------------------------------------------
void TextBlock2DComponent::initialize()
{
	VisualComponent::initialize();

	//owner->GetRootNode()->AddChild(this);
	setAutoRemove(true);

	m_paragraph = RefPtr<detail::Paragraph>::makeRef();
	m_paragraph->initialize();
}

//------------------------------------------------------------------------------
void TextBlock2DComponent::setText(const StringRef& text)
{
	m_paragraph->ClearInlines();
	auto run = RefPtr<detail::Run>::makeRef();
	run->initialize();
	run->setText(text);
	m_paragraph->AddInline(run);
}

//------------------------------------------------------------------------------
void TextBlock2DComponent::SetAnchorPoint(const Vector2& ratio)
{
	m_anchor = ratio;
}

//------------------------------------------------------------------------------
void TextBlock2DComponent::SetAnchorPoint(float ratioX, float ratioY)
{
	m_anchor.set(ratioX, ratioY);
}

//------------------------------------------------------------------------------
void TextBlock2DComponent::UpdateFrameHierarchy(SceneNode* parent, float deltaTime)
{
	VisualComponent::UpdateFrameHierarchy(parent, deltaTime);
	m_paragraph->UpdateLayout(Size::MaxValue);
	//m_paragraph->MeasureLayout(Size::MaxValue);
	//m_paragraph->ArrangeLayout(RectF(0, 0, Size::MaxValue));
}

//------------------------------------------------------------------------------
detail::Sphere TextBlock2DComponent::GetBoundingSphere()
{
	return VisualComponent::GetBoundingSphere();
}

//------------------------------------------------------------------------------
void TextBlock2DComponent::OnRender2(DrawList* renderer)
{
	struct LocalRenderer : detail::IDocumentsRenderer
	{
		DrawList* renderer;

		virtual void OnDrawGlyphRun(const Matrix& transform, Brush* forground, GlyphRun* glyphRun, const PointF& point) override
		{
			renderer->SetTransform(transform);
			renderer->SetBrush(forground);
			renderer->DrawGlyphRun(point, glyphRun);
		}
	} r;
	r.renderer = renderer;

	const Size& size = m_paragraph->GetRenderSize();
	m_paragraph->Render(Matrix::MakeTranslation(-size.width * m_anchor.x, -size.height * m_anchor.y, 0) * GetOwnerObject()->transform.GetWorldMatrix(), &r);
}

//------------------------------------------------------------------------------
//void TextBlock2DComponent::OnRender(SceneGraphRenderingContext* dc)
//{
//	const Size& size = m_paragraph->GetRenderSize();
//	m_paragraph->Render(Matrix::MakeTranslation(-size.width * m_anchor.x, -size.height * m_anchor.y, 0) * m_combinedGlobalMatrix, dc);
//}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
