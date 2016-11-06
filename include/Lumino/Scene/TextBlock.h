
#pragma once
#include <Lumino/Graphics/Texture.h>
#include "VisualNode.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN
namespace detail { class Paragraph; }
class TextBlock2D;
using TextBlock2DPtr = RefPtr<TextBlock2D>;

/**
	@brief
*/
class TextBlock2D
	: public VisualNode
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	
	/**
		@brief		
	*/
	static TextBlock2DPtr Create();

	/**
		@brief		
	*/
	static TextBlock2DPtr Create(const StringRef& text);

public:

	/** 表示する文字列を設定します。*/
	void SetText(const StringRef& text);

	/** 
		@brief		スプライトの原点を設定します。
		@details	値は (0,0) から (1,1) の間で指定します。
					デフォルトは (0,0) で、これはスプライトの左上が原点であることを意味します。
					(0.5,0.5) はスプライトの中央、(1,1) は右下が原点となります。
	*/
	void SetAnchor(const Vector2& ratio);
	void SetAnchor(float ratioX, float ratioY);		/**< @overload SetAnchor */

protected:
	TextBlock2D();
	virtual ~TextBlock2D();
	void Initialize(SceneGraph* owner);

	virtual void UpdateFrameHierarchy(SceneNode* parent, float deltaTime) override;
	virtual detail::Sphere GetBoundingSphere() override;
	virtual void OnRender(SceneGraphRenderingContext* dc) override;

protected:
	Vector2	m_anchor;
	RefPtr<detail::Paragraph>	m_paragraph;
	Size		m_renderSize;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
