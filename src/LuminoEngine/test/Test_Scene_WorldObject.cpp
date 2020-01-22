#include "Common.hpp"

#ifdef LN_UNIT_TEST_EXPERIMENTAL

//==============================================================================
//# Testing WorldObject
class Test_Scene_WorldObject : public LuminoSceneTest {};

//------------------------------------------------------------------------------
//## Transform
TEST_F(Test_Scene_WorldObject, Transform)
{
	{
        auto tex1 = Assets::loadTexture(LN_ASSETFILE("Sprite1.png"));

        //* [ ] Translation
        auto sprite1 = Sprite::create(tex1, 5, 5);
        sprite1->setPosition(3, 0, 0);

        //* [ ] Rotation
        auto sprite2 = Sprite::create(tex1, 5, 5);
        sprite2->setPosition(-3, 0, 0);
        sprite2->setEulerAngles(0.2, 0.3, 0.4);

        //* [ ] Scaling
        auto sprite3 = Sprite::create(tex1, 5, 5);
        sprite3->setPosition(0, 4, 0);
        sprite3->setScale(2, 0.5, 1.0);

        //* [ ] Center
        auto sprite4 = Sprite::create(tex1, 5, 5);
        sprite4->setPosition(0, 0, 0);
        sprite4->setCenterPoint(0, 3, 0);

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Result/Test_Scene_WorldObject-Transform-1.png"));
        LN_TEST_CLEAN_SCENE;
	}
}

//------------------------------------------------------------------------------
//## Destroy
TEST_F(Test_Scene_WorldObject, Destroy)
{
	{
		int count1 = Engine::mainWorld()->rootObjects()->size();

		auto sprite1 = Sprite::create(Assets::loadTexture(LN_ASSETFILE("Sprite1.png")), 5, 5);

		TestEnv::updateFrame();
		sprite1->destroy();
		int count2 = Engine::mainWorld()->rootObjects()->size();

		TestEnv::updateFrame();
		int count3 = Engine::mainWorld()->rootObjects()->size();

		ASSERT_EQ(count1 + 1, count2);
		ASSERT_EQ(count1, count3);
		LN_TEST_CLEAN_SCENE;
	}
}

#endif // LN_UNIT_TEST_EXPERIMENTAL
