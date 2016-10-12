
#include "../Internal.h"
#include <Lumino/Graphics/GraphicsContext.h>
#include "SceneGraphManager.h"
#include "RenderingPass.h"
#include <Lumino/Scene/SceneGraphRenderingContext.h>
#include <Lumino/Scene/SceneGraph.h>
#include <Lumino/Scene/TextBlock.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//==============================================================================
// TextBlock2D
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(TextBlock2D, VisualNode);

//------------------------------------------------------------------------------
TextBlock2D::TextBlock2D()
	: VisualNode()
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
}


LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
