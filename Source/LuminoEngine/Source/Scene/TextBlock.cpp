
#include "../Internal.h"
#include <Lumino/Graphics/GraphicsContext.h>
#include <Lumino/Graphics/Rendering.h>
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
TextBlock2DComponentPtr TextBlock2DComponent::Create()
{
	auto ptr = TextBlock2DComponentPtr::MakeRef();
	ptr->Initialize(detail::EngineDomain::GetDefaultSceneGraph2D());
	return ptr;
}

//------------------------------------------------------------------------------
TextBlock2DComponentPtr TextBlock2DComponent::Create(const StringRef& text)
{
	auto ptr = TextBlock2DComponentPtr::MakeRef();
	ptr->Initialize(detail::EngineDomain::GetDefaultSceneGraph2D());
	ptr->SetText(text);
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
void TextBlock2DComponent::Initialize(SceneGraph* owner)
{
	VisualComponent::Initialize(owner, 1);

	owner->GetRootNode()->AddChild(this);
	SetAutoRemove(true);

	m_paragraph = RefPtr<detail::Paragraph>::MakeRef();
	m_paragraph->Initialize();
}

//------------------------------------------------------------------------------
void TextBlock2DComponent::SetText(const StringRef& text)
{
	m_paragraph->ClearInlines();
	auto run = RefPtr<detail::Run>::MakeRef();
	run->Initialize();
	run->SetText(text);
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
	m_anchor.Set(ratioX, ratioY);
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
