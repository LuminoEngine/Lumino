
#pragma once
#include "Common.h"
#include "../Graphics/Painter.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

/**
	@brief	
*/
class SceneGraphRenderingContext
	: public RenderingContext
{
LN_INTERNAL_ACCESS:
	RenderingPass*				Pass;
	Details::Renderer*			Renderer;	// TODO 内部クラス
	GeometryRenderer*			GeometryRenderer;
	Camera*						CurrentCamera;
	MMEShader*					Shader;				// 本当に必要なシェーダ (VisualNode::Render() 以下で使用可能)

protected:
	friend class SceneGraphManager;
	SceneGraphRenderingContext(GraphicsManager* manager);
	virtual ~SceneGraphRenderingContext();
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
