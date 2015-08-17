
#pragma once
#include "../Internal.h"
#include "SceneGraphManager.h"
#include "SceneNode.h"
#include "VisualNode.h"
#include "RenderingPass.h"

namespace Lumino
{
namespace Scene
{

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
void RenderingPass::RenderNode(RenderingParams& params, SceneNode* node)
{
}



} // namespace Scene
} // namespace Lumino
