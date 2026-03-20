#include "Common.hpp"

class Test_Scene_Text : public LuminoSceneTest {};

TEST_F(Test_Scene_Text, Basic)
{
	auto text1 = Text::create(_TT("Text test."));
	Engine::world()->add(text1);

    TestEnv::updateFrame();
    ASSERT_SCREEN(LN_ASSETFILE("Scene/Expects/Text-Basic-1.png"));
    LN_TEST_CLEAN_SCENE;
}

