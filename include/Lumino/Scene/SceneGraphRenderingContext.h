
#pragma once
#include "Common.h"
#include "../Graphics/RenderingContext.h"

LN_NAMESPACE_BEGIN
class SpriteRenderer;
LN_NAMESPACE_SCENE_BEGIN
class SceneGraphManager;

/**
	@brief	
*/
class SceneGraphRenderingContext
	: public RenderingContext
{
LN_INTERNAL_ACCESS:
	RenderingPass*				Pass;
	Camera*						CurrentCamera;
	LightNodeList*				renderingLightList;
	MMEShader*					Shader;				// 本当に必要なシェーダ (VisualNode::Render() 以下で使用可能)


private:
	friend class SceneGraphManager;
	SceneGraphRenderingContext();
	virtual ~SceneGraphRenderingContext();
	void Initialize(SceneGraphManager* manager);

private:
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
