
#pragma once
#include "Common.h"
#include "../Graphics/GraphicsContext.h"

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
	GraphicsContext* BeginGraphicsContext() { return m_graphicsContext; }
	void Flush();

	Details::Renderer* GetRenderer() { return m_graphicsContext->Renderer; }

protected:
	friend class SceneGraphManager;
	SceneGraphRenderingContext(GraphicsContext* graphicsContext);
	virtual ~SceneGraphRenderingContext();

private:
	GraphicsContext*	m_graphicsContext;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
