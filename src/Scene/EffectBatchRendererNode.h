
#pragma once
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Scene/VisualNode.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN
namespace detail { class EffectManager; }

class EffectBatchRendererNode
	: public VisualNode	// TODO: 単に SceneNode のほうがいい気がするが・・
{
public:
	EffectBatchRendererNode();
	virtual ~EffectBatchRendererNode();
	void Initialize(SceneGraphManager* manager, detail::EffectManager* effectManager);

protected:
	virtual void OnRender(SceneGraphRenderingContext* dc) override;
	virtual void DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex) override {}

private:
	detail::EffectManager*	m_effectManager;	// TODO: Manager だとほんとに一括描画になるので Context とかもう少し細かい単位にできるとベスト。
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
