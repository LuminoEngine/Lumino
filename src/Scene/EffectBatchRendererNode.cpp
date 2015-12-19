
#pragma once
#include "Internal.h"
#include <Lumino/Scene/Camera.h>
#include <Lumino/Scene/SceneGraphRenderingContext.h>
#include "../Effect/EffectManager.h"
#include "../Effect/EffectDriver.h"
#include "EffectBatchRendererNode.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//=============================================================================
// EffectBatchRendererNode
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EffectBatchRendererNode::EffectBatchRendererNode()
	: m_effectManager(nullptr)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EffectBatchRendererNode::~EffectBatchRendererNode()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EffectBatchRendererNode::Initialize(SceneGraphManager* manager, detail::EffectManager* effectManager)
{
	VisualNode::CreateCore(manager, 1);
	m_effectManager = effectManager;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EffectBatchRendererNode::OnRender(SceneGraphRenderingContext* dc)
{
	m_effectManager->GetEffectEngine()->SetViewProjectin(dc->CurrentCamera->GetViewMatrix(), dc->CurrentCamera->GetProjectionMatrix());
	m_effectManager->Render();
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
