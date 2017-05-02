
#if 0
#include "Internal.h"
#include <Lumino/Scene/Camera.h>
#include "../Effect/EffectManager.h"
#include "../Effect/EffectDriver.h"
#include "EffectBatchRendererNode.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//==============================================================================
// EffectBatchRendererNode
//==============================================================================

//------------------------------------------------------------------------------
EffectBatchRendererNode::EffectBatchRendererNode()
	: m_effectManager(nullptr)
{
}

//------------------------------------------------------------------------------
EffectBatchRendererNode::~EffectBatchRendererNode()
{
}

//------------------------------------------------------------------------------
void EffectBatchRendererNode::Initialize(SceneGraph* owner, detail::EffectManager* effectManager)
{
	VisualComponent::Initialize(owner, 1);
	m_effectManager = effectManager;
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
#endif
