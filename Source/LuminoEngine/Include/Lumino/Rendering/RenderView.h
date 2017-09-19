
#pragma once
#include "Rendering.h"

LN_NAMESPACE_BEGIN
class PostEffect;

/**
	@brief	
*/
class WorldRenderView
	: public RenderView
{
public:
	void setLayerCullingMask(uint32_t mask) { m_layerCullingMask = mask; }
	uint32_t getLayerCullingMask() const { return m_layerCullingMask; }
	
LN_CONSTRUCT_ACCESS:
	WorldRenderView();
	virtual ~WorldRenderView();

private:
	uint32_t	m_layerCullingMask;
};

/**
	@brief
	@note	シーン描画の起点。特に UIViewport に属するレイヤーとして扱われる。
*/
class SceneRenderView
	: public WorldRenderView
{
LN_CONSTRUCT_ACCESS:
	SceneRenderView();
	virtual ~SceneRenderView();

private:
	List<Ref<PostEffect>>		m_postEffects;
};

LN_NAMESPACE_END
