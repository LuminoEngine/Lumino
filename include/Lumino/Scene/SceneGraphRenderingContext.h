
#pragma once
#include "Common.h"
#include "../Graphics/DrawingContext.h"

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
	MMEShader*					Shader;				// 本当に必要なシェーダ (VisualNode::Render() 以下で使用可能)

public:
	DrawingContext* BeginDrawingContext();
	SpriteRenderer* BeginSpriteRendering();
	GraphicsContext* BeginGraphicsContext() { return m_internalContext; }
	void Flush();

	Details::Renderer* GetRenderer() { return m_internalContext->Renderer; }
	GeometryRenderer* GetGeometryRenderer() { return m_internalContext->GeometryRenderer; }	// TODO: あとで消すと思う

protected:
	friend class SceneGraphManager;
	SceneGraphRenderingContext(GraphicsContext* internalContext);
	virtual ~SceneGraphRenderingContext();

private:
	GraphicsContext*	m_internalContext;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
