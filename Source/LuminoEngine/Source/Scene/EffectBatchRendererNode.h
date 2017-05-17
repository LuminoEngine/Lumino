
#pragma once
#if 0
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Scene/VisualNode.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN
namespace detail { class EffectManager; }

class EffectBatchRendererNode
	: public VisualComponent
{
public:
	EffectBatchRendererNode();
	virtual ~EffectBatchRendererNode();
	void Initialize(SceneGraph* owner, detail::EffectManager* effectManager);

protected:

private:
	detail::EffectManager*	m_effectManager;	// TODO: Manager だとほんとに一括描画になるので Context とかもう少し細かい単位にできるとベスト。
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
#endif
