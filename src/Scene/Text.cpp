
#include "../Internal.h"
#include <Lumino/Graphics/GraphicsContext.h>
#include "SceneGraphManager.h"
#include "RenderingPass.h"
#include <Lumino/Scene/SceneGraphRenderingContext.h>
#include <Lumino/Scene/SceneGraph.h>
#include <Lumino/Scene/Text.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//==============================================================================
// Text2D
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Text2D, VisualNode);

//------------------------------------------------------------------------------
Text2D::Text2D()
	: VisualNode()
{
}

//------------------------------------------------------------------------------
Text2D::~Text2D()
{
}

//------------------------------------------------------------------------------
void Text2D::Initialize(SceneGraph* owner, SpriteCoord spriteCoord)
{
	VisualNode::Initialize(owner, 1);
}


LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
