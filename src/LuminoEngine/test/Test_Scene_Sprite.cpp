#include "Common.hpp"

//==============================================================================
//# スプライトのテスト
class Test_Scene_Sprite : public LuminoSceneTest {};


#if 0   // TODO: Rendering の デフォルトシェーダ確定後

//------------------------------------------------------------------------------
//## 画像を表示する
TEST_F(Test_Scene_Sprite, Basic)
{
	//* [ ] can be drawn
	//* [ ] setSourceRect
    //* [ ] Sprite はデフォルトで BlendModel=Aplha
	{

        auto light1 = AmbientLight::create();
        auto texture1 = Assets::loadTexture(LN_ASSETFILE("Sprite1.png"));

        auto sprite1 = Sprite::create(3, 3, texture1);
        sprite1->setPosition(-3, 0, 0);

        auto sprite2 = Sprite::create(3, 3, texture1);
        sprite2->setPosition(0, 0, 0);
        sprite2->setSourceRect(Rect(0, 0, 16, 16));

        auto sprite3 = Sprite::create(3, 3, texture1);
        sprite3->setPosition(3, 0, 0);
        sprite3->setSourceRect(16, 16, 16, 16);


        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Result/Test_Scene_Sprite-Basic-1.png"));
        LN_TEST_CLEAN_SCENE;
	}
}

#endif
