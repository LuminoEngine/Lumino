#include <TestConfig.h>

class Test_Rendering_Light : public LuminoGraphicsTest {};

TEST_F(Test_Rendering_Light, Basic)
{
	// <Test> 
	{
		ScopedCameraPosition cam(0, 5, -5);

		Engine::getCamera3D()->addComponent(newObject<CameraMouseMoveBehavior>());

		auto tex1 = Assets::loadTexture(LN_LOCALFILE("../Graphics/TestData/img2.png"));
		auto mesh1 = StaticMeshComponent::createPlane(Vector2(10, 10), 1, 1);
		auto material1 = mesh1->getStaticMeshModel()->getMaterial(0);
		material1->setMaterialTexture(tex1);
		auto meshObj1 = newObject<WorldObject>();
		meshObj1->addComponent(mesh1);
		Engine::getWorld3D()->add(meshObj1);

		auto light1 = PointLight::create(Color::White, 2);
		Engine::getWorld3D()->add(light1);

		while(Engine::update());
		//Engine::update();
		//ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Rendering_Light.Basic1.png"), 99, true));
	}
}
