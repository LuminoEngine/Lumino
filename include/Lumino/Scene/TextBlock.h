
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
	static TextBlock2DPtr Create(const StringRef& text);

public:

	/** 表示する文字列を設定します。*/
	void SetText(const StringRef& text);

protected:
	TextBlock2D();
	virtual ~TextBlock2D();
	void Initialize(SceneGraph* owner);

	virtual detail::Sphere GetBoundingSphere() override;
	virtual void OnRender(SceneGraphRenderingContext* dc) override;

protected:
	RefPtr<detail::Paragraph>	m_paragraph;
	SizeF		m_renderSize;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
