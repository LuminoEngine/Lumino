
#pragma once
#include <Lumino/Graphics/Texture.h>
#include "VisualNode.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

/**
	@brief
*/
class Text2D
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


protected:
	Text2D();
	virtual ~Text2D();
	void Initialize(SceneGraph* owner, SpriteCoord spriteCoord);

	virtual detail::Sphere GetBoundingSphere() override;
	virtual void OnRender(SceneGraphRenderingContext* dc) override;

protected:
	SizeF		m_renderSize;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
