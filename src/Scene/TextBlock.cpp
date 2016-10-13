
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
	m_paragraph = RefPtr<detail::Paragraph>::MakeRef();
	m_paragraph->Initialize(owner->GetManager()->GetDocumentsManager());
}

//------------------------------------------------------------------------------
void TextBlock2D::SetText(const StringRef& text)
{
}


LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
