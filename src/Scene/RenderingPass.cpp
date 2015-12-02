
#pragma once
#include "../Internal.h"
#include "SceneGraphManager.h"
#include <Lumino/Scene/SceneNode.h>
#include <Lumino/Scene/VisualNode.h>
#include "RenderingPass.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//=============================================================================
// RenderingPass
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RenderingPass::RenderingPass(SceneGraphManager* manager/*, int passID, MMEShader* ownerShader*/)
	: m_manager(NULL)
	, m_internalEntryID(-1)
{
	LN_REFOBJ_SET(m_manager, manager);
	m_manager->AddRenderingPass(this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RenderingPass::~RenderingPass()
{
	if (m_manager != NULL) {
		m_manager->RemoveRenderingPass(this);
		LN_SAFE_RELEASE(m_manager);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingPass::RenderNode(SceneGraphRenderingContext* dc, SceneNode* node)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingPass::RenderSubset(SceneGraphRenderingContext* dc, VisualNode* node, int subset)
{
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
