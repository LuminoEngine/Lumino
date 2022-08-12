#include "Common.hpp"

#ifdef LN_UNIT_TEST_EXPERIMENTAL

//==============================================================================
//# ClusteredShading
class Test_Rendering_ClusteredShading : public LuminoSceneTest {};

//------------------------------------------------------------------------------
//## Basic
TEST_F(Test_Rendering_ClusteredShading, Basic)
{
	{
#ifdef LN_COORD_RH
		Engine::mainCamera()->setPosition(0, 5, 5);
#else
	Engine::mainCamera()->setPosition(0, 5, -5);
#endif
		Engine::mainCamera()->lookAt(Vector3(0, 0, 0));

		auto obj1 = makeObject_deprecated<WorldObject>();
		auto cmp1 = makeObject_deprecated<PlaneMeshComponent>();
		cmp1->setSize(10, 10);
		auto mat1 = Material::create();
		obj1->addComponent(cmp1);
		cmp1->setMaterial(mat1);

		// 中央
		auto light1 = PointLight::create();
		light1->setRange(2);
		light1->setPosition(0, 0.1, 0);
        light1->setColor(Color::Green);

		// 左奥
		auto light2 = PointLight::create();
		light2->setRange(2);
		light2->setPosition(-3, 0.1, 3);
        light2->setColor(Color::Red);

		// 右奥
		auto light3 = PointLight::create();
		light3->setRange(2);
		light3->setPosition(3, 0.1, 3);
        light3->setColor(Color::Blue);

		// 左手前
		auto light4 = SpotLight::create();
		light4->setRange(2);
		light4->setPosition(-3, 0.1, -3);

		// 右手前
		auto light5 = SpotLight::create();
		light5->setRange(2);
		light5->setPosition(3, 0.1, -3);

		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("Rendering/Result/Test_Rendering_ClusteredShading-Basic-1.png"));
		LN_TEST_CLEAN_SCENE;
	}
}

#endif // LN_UNIT_TEST_EXPERIMENTAL


//==============================================================================
class Test_Rendering_Shading : public LuminoSceneTest {};

#if 0
//------------------------------------------------------------------------------
TEST_F(Test_Rendering_Shading, Fog)
{
	auto plane = PlaneMesh::create();
	plane->planeMeshComponent()->setSize(1000, 1000);

	auto mat1 = Material::create();
	mat1->setColor(Color::White);
	plane->planeMeshComponent()->setMaterial(mat1);


	Engine::camera()->setPosition(0, 20, -40);
	Engine::camera()->lookAt(0, 0, 0);

	Scene::setFogColor(Color::Red);
	Scene::setFogDensity(1);
	Scene::setFogStartDistance(10);

	// 今のところデフォルトデライトは作成せず、それ用にテストを作ってあるので assert 入れておく
	assert(!Engine::world()->mainDirectionalLight());

	auto light = DirectionalLight::create();
	light->lookAt(0, -1, 0);
	Engine::world()->setMainDirectionalLight(light);

	TestEnv::updateFrame();
	ASSERT_SCREEN(LN_ASSETFILE("Rendering/Expects/Shading-Basic-1.png"));
	LN_TEST_CLEAN_SCENE;
	Engine::world()->setMainDirectionalLight(nullptr);
}
#endif

#ifdef LN_COORD_RH
#else
//------------------------------------------------------------------------------
TEST_F(Test_Rendering_Shading, EnvironmentLight)
{
	auto mat1 = Material::create();
	mat1->setColor(Color::White);
	mat1->setRoughness(1.0);
	mat1->setMetallic(0.0);
	SphereMesh::Builder()
		.material(mat1)
		.buildInto();

#ifdef LN_COORD_RH
	Engine::mainCamera()->setPosition(0, 0, 1);
#else
	Engine::mainCamera()->setPosition(0, 0, -1);
#endif

	auto light = Engine::world()->mainLight();
	light->setIntensity(0.0f);
	light->setAmbientColor(Color::Blue);
	light->setSkyColor(Color::Green);
	light->setGroundColor(Color::Red);

	TestEnv::updateFrame();
	ASSERT_SCREEN(LN_ASSETFILE("Rendering/Expects/Shading-EnvironmentLight-1.png"));
	LN_TEST_CLEAN_SCENE;
	light->setSkyColor(Color::Black);
	light->setGroundColor(Color::Black);
}
#endif

//------------------------------------------------------------------------------
TEST_F(Test_Rendering_Shading, MaterialEmissive)
{
	auto mat1 = Material::create();
	mat1->setColor(Color::Red);
	mat1->setRoughness(1.0);
	mat1->setMetallic(0.0);
	mat1->setEmissive(Color::Blue);
	SphereMesh::Builder()
		.material(mat1)
		.buildInto();

#ifdef LN_COORD_RH
	Engine::mainCamera()->setPosition(0, 0, 1);
#else
	Engine::mainCamera()->setPosition(0, 0, -1);
#endif

	auto light = Engine::world()->mainLight();
	light->setIntensity(0.0f);
	light->setAmbientColor(Color::Black);

	// NOTE: Material Color は Red だが、DirectionalLight と Ambient が Black なので、光源が無い。Red は打ち消される。
	// そのため Emissive の Blue だけが出てくる。
	TestEnv::updateFrame();
	ASSERT_SCREEN(LN_ASSETFILE("Rendering/Expects/Shading-MaterialEmissive-1.png"));

	// NOTE: Ambient を White にすると、物体表面の放射照度が 1 になる。
	// これと AlbedoColor が乗算されるため、Red が出てくるようになる。
	light->setAmbientColor(Color::White);
	TestEnv::updateFrame();
	ASSERT_SCREEN(LN_ASSETFILE("Rendering/Expects/Shading-MaterialEmissive-2.png"));

	LN_TEST_CLEAN_SCENE;
}
