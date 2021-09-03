#include "Common.hpp"
#include <LuminoEngine/PostEffect/SSRPostEffect.hpp>

//==============================================================================
class Test_PostEffect : public LuminoSceneTest
{
public:

};

TEST_F(Test_PostEffect, Bloom)
{
	auto sprite1 = Sprite::Builder()// Sprite::create(Texture2D::whiteTexture(), 5, 5);
		.texture(Texture2D::whiteTexture())
		.size(5, 5)
		.buildInto();
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


	auto plane1Material = Material::create();
	plane1Material->setColor(Color::Blue);
	plane1Material->setRoughness(0.0f);
	plane1Material->setMetallic(1.0f);
	PlaneMesh::Builder()
		.position(-2, 0, 0)
#ifdef LN_COORD_RH
		.rotation(Math::PI / 2, Math::PI / 3, 0)
#else
		.rotation(-Math::PI / 2, -Math::PI / 3, 0)
#endif
		.material(plane1Material)
		.buildInto();

	//auto box1Material = Material::create();
	//box1Material->setColor(Color::Green);
	//box1Material->setRoughness(1.0f);
	//box1Material->setMetallic(0.0f);
	auto box1Material = Material::With()
		.color(Color::Green)
		.roughness(1.0f)
		.metallic(0.0f)
		.build();

	auto box = BoxMesh::Builder()
		.position(4, 0, 0)
		.scale(5)
		.material(box1Material)
		.buildInto();


	auto ssrEffect = makeObject<SSRPostEffect>();
	Engine::renderView()->addPostEffect(ssrEffect);

	Engine::mainLight()->setEnabled(false);

	TestEnv::updateFrame();
	ASSERT_SCREEN(LN_ASSETFILE("PostEffect/Expects/PostEffect-SSR-1.png"));
	Engine::renderView()->removePostEffect(ssrEffect);
	LN_TEST_CLEAN_SCENE;
}
