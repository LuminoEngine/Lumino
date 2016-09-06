
#pragma once
#include "Common.h"
#include "../Graphics/RenderingContext.h"

LN_NAMESPACE_BEGIN
namespace detail { class SpriteRenderer; }
LN_NAMESPACE_SCENE_BEGIN
class SceneGraphManager;

/**
	@brief	
*/
class SceneGraphRenderingContext
	: public RenderingContext
{
public:

	void DrawSprite2D(
		const Matrix& transform,
		const SizeF& size,
		Texture* texture,
		const RectF& srcRect,
		const Color& color);

LN_INTERNAL_ACCESS:
	RenderingPass*				Pass;
	Camera*						CurrentCamera;
	LightNodeList*				renderingLightList;
	MMEShader*					Shader;				// 本当に必要なシェーダ (VisualNode::Render() 以下で使用可能)

protected:
	virtual void OnStateFlush() override;

LN_INTERNAL_ACCESS:
	SceneGraphRenderingContext();
	virtual ~SceneGraphRenderingContext();
	void Initialize(SceneGraphManager* manager);
	void SetCurrentCamera(Camera* camera);

private:
	RefPtr<detail::SpriteRenderer>	m_spriteRenderer;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
