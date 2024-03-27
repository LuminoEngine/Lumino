#include "Common.hpp"

#if 0
//==============================================================================
//# MeshPrimitive
class Test_Scene_MeshPrimitive : public LuminoSceneTest
{
	Ref<AmbientLight> light1;
	Ref<DirectionalLight> light2;

	virtual void SetUp() override
	{
		light1 = AmbientLight::create();

		// DirectionalLight は 手前右上からあてる
		light2 = DirectionalLight::create();
		light2->setPosition(10, 10, -10);
		light2->lookAt(Vector3(0, 0, 0));

		// Camera は 手前左上から見る
		Engine::mainCamera()->setPosition(5, 5, 5);
		Engine::mainCamera()->lookAt(Vector3(0, 0, 0));
	}

	virtual void TearDown() override
	{
	}
};

//------------------------------------------------------------------------------
//## Box
TEST_F(Test_Scene_MeshPrimitive, Box)
{
	//* [ ] Box
	{
		auto obj1 = makeObject_deprecated<WorldObject>();
		auto cmp1 = makeObject_deprecated<SphereMeshComponent>();
		auto mat1 = Material::create();
		//mat1->setMetallic(1);
		//mat1->setRoughness(1);
		//mat2->setMainTexture(tex2);
		obj1->addComponent(cmp1);
		//obj1->setPosition(i, -y, 0);
		cmp1->setMaterial(mat1);

        TestEnv::updateFrame();
        ASSERT_SCREEN_S(LN_ASSETFILE("Scene/Result/Test_Scene_MeshPrimitive-Box-1.png"));
        LN_TEST_CLEAN_SCENE;
	}
}

#endif

