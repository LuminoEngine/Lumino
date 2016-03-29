
#pragma once
#include "SpriteBase.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

/**
	@brief	3D 空間に配置されるスプライトです。
*/
class Sprite3D
	: public SpriteBase
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	
	/**
		@brief		テクスチャを持たないスプライトを作成します。テクスチャを割り当てるには SetTexture() を使用します。
	*/
	static Sprite3DPtr Create();

	static Sprite3DPtr Create(float width, float height);

protected:
	Sprite3D();
	virtual ~Sprite3D();
	void Initialize(SceneGraphManager* manager);
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
