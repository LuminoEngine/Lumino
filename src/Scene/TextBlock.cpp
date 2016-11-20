
#include "../Internal.h"
#include <Lumino/Graphics/GraphicsContext.h>
#include "SceneGraphManager.h"
#include "RenderingPass.h"
#include <Lumino/Scene/SceneGraph.h>
#include <Lumino/Scene/TextBlock.h>
#include "../Documents/DocumentElements.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//==============================================================================
// TextBlock2D
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(TextBlock2D, VisualNode);

//------------------------------------------------------------------------------
TextBlock2DPtr TextBlock2D::Create()
{
	auto ptr = TextBlock2DPtr::MakeRef();
	ptr->Initialize(SceneGraphManager::Instance->GetDefaultSceneGraph2D());
	return ptr;
}

//------------------------------------------------------------------------------
TextBlock2DPtr TextBlock2D::Create(const StringRef& text)
{
	auto ptr = TextBlock2DPtr::MakeRef();
	ptr->Initialize(SceneGraphManager::Instance->GetDefaultSceneGraph2D());
	ptr->SetText(text);
	return ptr;
}

//------------------------------------------------------------------------------
TextBlock2D::TextBlock2D()
	: VisualNode()
	, m_paragraph(nullptr)
{
}

//------------------------------------------------------------------------------
TextBlock2D::~TextBlock2D()
{
}

//------------------------------------------------------------------------------
void TextBlock2D::Initialize(SceneGraph* owner)
{
	VisualNode::Initialize(owner, 1);

	owner->GetManager()->GetDefaultSceneGraph2D()->GetRootNode()->AddChild(this);
	SetAutoRemove(true);

	m_paragraph = RefPtr<detail::Paragraph>::MakeRef();
	m_paragraph->Initialize(owner->GetManager()->GetDocumentsManager());
}

//------------------------------------------------------------------------------
void TextBlock2D::SetText(const StringRef& text)
{
	m_paragraph->ClearChildElements();
	auto run = RefPtr<detail::Run>::MakeRef();
	run->Initialize(GetOwnerSceneGraph()->GetManager()->GetDocumentsManager());
	run->SetText(text);
	m_paragraph->AddChildElement(run);
}

//------------------------------------------------------------------------------
void TextBlock2D::SetAnchorPoint(const Vector2& ratio)
{
	m_anchor = ratio;
}

//------------------------------------------------------------------------------
void TextBlock2D::SetAnchorPoint(float ratioX, float ratioY)
{
	m_anchor.Set(ratioX, ratioY);
}

//------------------------------------------------------------------------------
void TextBlock2D::UpdateFrameHierarchy(SceneNode* parent, float deltaTime)
{
	VisualNode::UpdateFrameHierarchy(parent, deltaTime);
	m_paragraph->MeasureLayout(Size::MaxValue);
	m_paragraph->ArrangeLayout(RectF(0, 0, Size::MaxValue));
}

//------------------------------------------------------------------------------
detail::Sphere TextBlock2D::GetBoundingSphere()
{
	return VisualNode::GetBoundingSphere();
}

//------------------------------------------------------------------------------
//void TextBlock2D::OnRender(SceneGraphRenderingContext* dc)
//{
//	const Size& size = m_paragraph->GetRenderSize();
//	m_paragraph->Render(Matrix::MakeTranslation(-size.width * m_anchor.x, -size.height * m_anchor.y, 0) * m_combinedGlobalMatrix, dc);
//}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
