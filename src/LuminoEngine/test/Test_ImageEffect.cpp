#include "Common.hpp"

//==============================================================================
class Test_ImageEffect : public LuminoSceneTest
{
public:

};

TEST_F(Test_ImageEffect, Bloom)
{
	auto sprite1 = Sprite::create(Texture2D::whiteTexture(), 5, 5);
	sprite1->setShadingModel(ShadingModel::Unlit);

	auto bloomEffect = BloomImageEffect::create();
	Engine::renderView()->addImageEffect(bloomEffect);

	TestEnv::updateFrame();
	ASSERT_SCREEN(LN_ASSETFILE("ImageEffect/Expects/ImageEffect-Bloom-1.png"));
	Engine::renderView()->removeImageEffect(bloomEffect);
	LN_TEST_CLEAN_SCENE;
}

