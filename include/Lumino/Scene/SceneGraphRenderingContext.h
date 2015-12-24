
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
	//: public DrawingContext
{
LN_INTERNAL_ACCESS:
	RenderingPass*				Pass;
	Details::Renderer*			Renderer;	// TODO 内部クラス
	GeometryRenderer*			GeometryRenderer;
	Camera*						CurrentCamera;
	MMEShader*					Shader;				// 本当に必要なシェーダ (VisualNode::Render() 以下で使用可能)

public:

	DrawingContext* BeginDrawingContext();
	SpriteRenderer* BeginSpriteRendering();

	void Flush();

protected:
	friend class SceneGraphManager;
	SceneGraphRenderingContext(GraphicsManager* manager);
	virtual ~SceneGraphRenderingContext();

private:
	enum class RendererType
	{
		None,
		DrawingContext,
		SpriteRenderer,
	};

	RendererType	m_currentRenderer;
	DrawingContext	m_drawingContext;
	SpriteRenderer*	m_spriteRenderer;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
