
#include "../Internal.h"
#include <Lumino/Graphics/GraphicsContext.h>
#include "SceneGraphManager.h"
#include "RenderingPass.h"
#include <Lumino/Scene/SceneGraphRenderingContext.h>
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
TextBlock2DPtr TextBlock2D::Create(const StringRef& text)
{
	auto ptr = TextBlock2DPtr::MakeRef();
	ptr->Initialize(SceneGraphManager::Instance->GetDefault2DSceneGraph());
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

	owner->GetManager()->GetDefault2DSceneGraph()->GetRootNode()->AddChild(this);
	SetAutoRemove(true);

	m_paragraph = RefPtr<detail::Paragraph>::MakeRef();
	m_paragraph->Initialize(owner->GetManager()->GetDocumentsManager());
}

//------------------------------------------------------------------------------
void TextBlock2D::SetText(const StringRef& text)
{
	m_paragraph->ClearInlines();
	auto run = RefPtr<detail::Run>::MakeRef();
	run->Initialize(GetOwnerSceneGraph()->GetManager()->GetDocumentsManager());
	run->SetText(text);
	m_paragraph->AddInline(run);
}

//------------------------------------------------------------------------------
void TextBlock2D::UpdateFrameHierarchy(SceneNode* parent, float deltaTime)
{
	VisualNode::UpdateFrameHierarchy(parent, deltaTime);
	m_paragraph->MeasureLayout(SizeF::Max);
	m_paragraph->ArrangeLayout(RectF(0, 0, SizeF::Max));
}

//------------------------------------------------------------------------------
detail::Sphere TextBlock2D::GetBoundingSphere()
{
	return VisualNode::GetBoundingSphere();
}

//------------------------------------------------------------------------------
void TextBlock2D::OnRender(SceneGraphRenderingContext* dc)
{
	m_paragraph->Render(dc);
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
