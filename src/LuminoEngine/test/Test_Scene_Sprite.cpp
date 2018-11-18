#include "Common.hpp"

//==============================================================================
//# スプライトのテスト
class Test_Scene_Sprite : public LuminoSceneTest {};

//------------------------------------------------------------------------------
//## 画像を表示する
TEST_F(Test_Scene_Sprite, Basic)
{
	//* [ ] can be drawn
	//* [ ] setOpacity
	{

        auto light1 = AmbientLight::create();
        auto sprite1 = Sprite::create(5, 5, Assets::loadTexture(LN_ASSETFILE("Sprite1.png")));
        sprite1->setPosition(2, 0, 0);

        TestEnv::updateFrame();
        ASSERT_SCREEN_S(LN_ASSETFILE("Result/Test_Scene_Sprite-Basic-1.png"));
	}
}
