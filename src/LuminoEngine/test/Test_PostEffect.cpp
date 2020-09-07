#include "Common.hpp"
#include <LuminoEngine/PostEffect/SSRPostEffect.hpp>

//==============================================================================
class Test_PostEffect : public LuminoSceneTest
{
public:

};

TEST_F(Test_PostEffect, Bloom)
{
	auto sprite1 = Sprite::create(Texture2D::whiteTexture(), 5, 5);
	sprite1->setShadingModel(ShadingModel::Unlit);

	auto bloomEffect = BloomPostEffect::create();
	Engine::renderView()->addPostEffect(bloomEffect);

	TestEnv::updateFrame();
	ASSERT_SCREEN(LN_ASSETFILE("PostEffect/Expects/PostEffect-Bloom-1.png"));
	Engine::renderView()->removePostEffect(bloomEffect);
	LN_TEST_CLEAN_SCENE;
}


TEST_F(Test_PostEffect, SSR)
{
	auto plane1 = PlaneMesh::create();
	auto plane1Material = Material::create();
	plane1Material->setColor(Color::Blue);
	plane1Material->setRoughness(0.0f);
	plane1Material->setMetallic(1.0f);
	plane1->setRotation(-Math::PI / 2, -Math::PI / 3, 0);
	plane1->setPosition(-2, 0, 0);
	plane1->planeMeshComponent()->setMaterial(plane1Material);

	auto box1 = BoxMesh::create();
	auto box1Material = Material::create();
	box1Material->setColor(Color::Green);
	box1Material->setRoughness(1.0f);
	box1Material->setMetallic(0.0f);
	box1->boxMeshComponent()->setMaterial(box1Material);

	box1->setScale(5);
	box1->setPosition(4, 0, 0);

	auto ssrEffect = makeObject<SSRPostEffect>();
	Engine::renderView()->addPostEffect(ssrEffect);

	Engine::light()->setEnabled(false);

	TestEnv::updateFrame();
	ASSERT_SCREEN(LN_ASSETFILE("PostEffect/Expects/PostEffect-SSR-1.png"));
	Engine::renderView()->removePostEffect(ssrEffect);
	LN_TEST_CLEAN_SCENE;
}
