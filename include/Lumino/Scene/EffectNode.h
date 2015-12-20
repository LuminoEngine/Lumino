
#pragma once
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Scene/VisualNode.h>
#include "../Effect/VisualEffectInstance.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN
namespace detail { class EffectManager; }

/**
	@brief
*/
class EffectNode
	: public VisualNode
{
public:

	//static VisualEffectInstancePtr PlayInstance(const);

protected:
	EffectNode();
	virtual ~EffectNode();
	void Initialize(SceneGraphManager* manager, detail::EffectManager* effectManager);
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
