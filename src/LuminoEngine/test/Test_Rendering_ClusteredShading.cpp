﻿#include "Common.hpp"
#include <LuminoEngine/Visual/MeshPrimitiveComponent.hpp>

//==============================================================================
//# ClusteredShading
class Test_Rendering_ClusteredShading : public LuminoSceneTest {};

//------------------------------------------------------------------------------
//## Basic
TEST_F(Test_Rendering_ClusteredShading, Basic)
{
	{
		Engine::mainCamera()->setPosition(0, 5, -5);
		Engine::mainCamera()->lookAt(Vector3(0, 0, 0));

		auto obj1 = newObject<WorldObject>();
		auto cmp1 = newObject<PlaneComponent>();
		cmp1->setSize(10, 10);
		auto mat1 = Material::create();
		obj1->addComponent(cmp1);
		cmp1->setMaterial(mat1);

		// 中央
		auto light1 = PointLight::create();
		light1->setRange(5);
		light1->setPosition(0, 0.1, 0);
        light1->setColor(Color::Green);

		// 左奥
		auto light2 = PointLight::create();
		light2->setRange(5);
		light2->setPosition(-3, 0.1, 3);
        light2->setColor(Color::Red);

		// 右奥
		auto light3 = PointLight::create();
		light3->setRange(5);
		light3->setPosition(3, 0.1, 3);
        light3->setColor(Color::Blue);

		// 左手前
		auto light4 = SpotLight::create();
		light4->setRange(5);
		light4->setPosition(-3, 0.1, -3);

		// 右手前
		auto light5 = SpotLight::create();
		light5->setRange(5);
		light5->setPosition(3, 0.1, -3);

		TestEnv::updateFrame();
		ASSERT_SCREEN_S(LN_ASSETFILE("Rendering/Result/Test_Rendering_ClusteredShading-Basic-1.png"));
		LN_TEST_CLEAN_SCENE;
	}
}
