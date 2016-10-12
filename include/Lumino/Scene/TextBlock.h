
#pragma once
#include <Lumino/Graphics/Texture.h>
#include "VisualNode.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

/**
	@brief
*/
class TextBlock2D
	: public VisualNode
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	//
	///**
	//	@brief		テクスチャを持たないスプライトを作成します。テクスチャを割り当てるには SetTexture() を使用します。
	//*/
	//static SpriteBase* Create();

	///**
	//	@brief		
	//*/
	//static SpriteBase* Create3D();

public:

	/** 表示する文字列を設定します。 */
	void SetText(const StringRef& text);

protected:
	TextBlock2D();
	virtual ~TextBlock2D();
	void Initialize(SceneGraph* owner);

	virtual detail::Sphere GetBoundingSphere() override;
	virtual void OnRender(SceneGraphRenderingContext* dc) override;

protected:
	String		m_text;
	SizeF		m_renderSize;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
