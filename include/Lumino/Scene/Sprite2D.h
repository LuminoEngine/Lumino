
#pragma once
#include "SpriteBase.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

/**
	@brief	2D 空間に配置されるスプライトです。
*/
class Sprite2D
	: public SpriteBase
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	
	/**
		@brief		テクスチャを持たないスプライトを作成します。テクスチャを割り当てるには SetTexture() を使用します。
	*/
	static Sprite2DPtr Create();
	
	/**
		@brief		指定したファイルパスのテクスチャを読み込み、スプライトを作成します。
	*/
	static Sprite2DPtr Create(const StringRef& filePath);

	static Sprite2DPtr Create(Texture* texture);

protected:
	Sprite2D();
	virtual ~Sprite2D();
	void Initialize(SceneGraphManager* manager);
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
