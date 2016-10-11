
#pragma once
#include "Common.h"
#include "../Graphics/RenderingContext.h"

LN_NAMESPACE_BEGIN
namespace detail { class SpriteRenderer; }
namespace detail { class TextRenderer; }
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
		const Vector2& anchorRatio,
		Texture* texture,
		const RectF& srcRect,
		const Color& color);

	void DrawSprite3D(
		const Matrix& transform,
		const SizeF& size,
		const Vector2& anchorRatio,
		Texture* texture,
		const RectF& srcRect,
		const Color& color,
		AxisDirection front = AxisDirection_RZ);

	void DrawText();
	
LN_INTERNAL_ACCESS:
	RenderingPass*				Pass;
	LightNodeList*				renderingLightList;
	MMEShader*					Shader;				// 本当に必要なシェーダ (VisualNode::Render() 以下で使用可能)

protected:
	virtual void OnStateFlush(detail::IRendererPloxy* activeRenderer) override;

LN_INTERNAL_ACCESS:
	SceneGraphRenderingContext();
	virtual ~SceneGraphRenderingContext();
	void Initialize(SceneGraphManager* manager);
	void SetCurrentCamera(Camera* camera);
	Camera* GetCurrentCamera() const { return m_currentCamera; }

private:
	Camera*							m_currentCamera;
	RefPtr<detail::SpriteRenderer>	m_spriteRenderer;
	RefPtr<detail::TextRenderer>	m_textRenderer;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
