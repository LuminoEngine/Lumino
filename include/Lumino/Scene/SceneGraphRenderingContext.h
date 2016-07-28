
#pragma once
#include "Common.h"
#include "../Graphics/RenderingContext.h"

LN_NAMESPACE_BEGIN
class SpriteRenderer;
LN_NAMESPACE_SCENE_BEGIN

/**
	@brief	
*/
class SceneGraphRenderingContext
	//: public GraphicsContext
{
LN_INTERNAL_ACCESS:
	RenderingPass*				Pass;
	Camera*						CurrentCamera;
	LightNodeList*				renderingLightList;
	MMEShader*					Shader;				// 本当に必要なシェーダ (VisualNode::Render() 以下で使用可能)

public:
	RenderingContext* BeginGraphicsContext() { return m_context; }
	void Flush();

	//Details::Renderer* GetRenderer() { return m_context->; }

protected:
	friend class SceneGraphManager;
	SceneGraphRenderingContext(RenderingContext* context);
	virtual ~SceneGraphRenderingContext();

private:
	RenderingContext*	m_context;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
