#include "Common.hpp"

//==============================================================================
class Test_ImageEffect : public LuminoSceneTest
{
public:

};

TEST_F(Test_ImageEffect, Bloom)
{
	auto sprite1 = Sprite::create(Texture2D::whiteTexture(), 5, 5);
	sprite1->setShadingModel(ShadingModel::UnLighting);

	auto bloomEffect = BloomImageEffect::create();
	Engine::mainViewport()->addImageEffect(bloomEffect);

	TestEnv::updateFrame();
	ASSERT_SCREEN(LN_ASSETFILE("ImageEffect/Expects/ImageEffect-Bloom-1.png"));
	Engine::mainViewport()->removeImageEffect(bloomEffect);
	LN_TEST_CLEAN_SCENE;
}

