
#pragma once
#include <Lumino/Graphics/Texture.h>
#include "VisualNode.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN
namespace detail { class Paragraph; }
class TextBlock2DComponent;
using TextBlock2DComponentPtr = RefPtr<TextBlock2DComponent>;

/**
	@brief
*/
class TextBlock2DComponent
	: public VisualComponent
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	
	/**
		@brief		
	*/
	static TextBlock2DComponentPtr create();

	/**
		@brief		
	*/
	static TextBlock2DComponentPtr create(const StringRef& text);

public:

	/** 表示する文字列を設定します。*/
	void setText(const StringRef& text);

	/** 
		@brief		テキストの表示の原点を設定します。
		@details	値は (0,0) から (1,1) の間で指定します。
					デフォルトは (0,0) で、これはテキストの左上が原点であることを意味します。
					(0.5,0.5) はテキストの中央、(1,1) は右下が原点となります。
	*/
	void setAnchorPoint(const Vector2& ratio);
	void setAnchorPoint(float ratioX, float ratioY);		/**< @overload setAnchorPoint */

protected:
	TextBlock2DComponent();
	virtual ~TextBlock2DComponent();
	void initialize();

	virtual void updateFrameHierarchy(SceneNode* parent, float deltaTime) override;
	virtual detail::Sphere getBoundingSphere() override;
	virtual void onRender2(RenderingContext* renderer) override;

protected:
	Vector2	m_anchor;
	RefPtr<detail::Paragraph>	m_paragraph;
	Size		m_renderSize;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
