#include "Common.hpp"

//==============================================================================
//# Testing UIElement
class Test_UI_UIElement : public LuminoSceneTest {};

//------------------------------------------------------------------------------
//## Basic
TEST_F(Test_UI_UIElement, RenderEffects)
{
    //* [ ] UISprite はデフォルトで BlendMode=Alpha
	{
        auto tex1 = Texture2D::load(LN_ASSETFILE("Sprite1.png"));

        //* [ ] Visible
        auto sprite1 = makeObject_deprecated<UISprite>();
		sprite1->setAlignments(UIAlignment::TopLeft);
        sprite1->setTexture(tex1);
        sprite1->setPosition(0, 0, 0);
        sprite1->setVisibility(UIVisibility::Hidden);
		Engine::mainUIView()->addElement(sprite1);

        //* [ ] BlendMode
        auto sprite2 = makeObject_deprecated<UISprite>();
		sprite2->setAlignments(UIAlignment::TopLeft);
        sprite2->setTexture(tex1);
        sprite2->setBlendMode(BlendMode::Add);
        sprite2->setPosition(32, 0, 0);
		Engine::mainUIView()->addElement(sprite2);

        //* [ ] Opacity
        auto sprite3 = makeObject_deprecated<UISprite>();
		sprite3->setAlignments(UIAlignment::TopLeft);
        sprite3->setTexture(tex1);
        sprite3->setPosition(64, 0, 0);
        sprite3->setOpacity(0.5);
		Engine::mainUIView()->addElement(sprite3);

        //* [ ] ColorScale
        auto sprite4 = makeObject_deprecated<UISprite>();
		sprite4->setAlignments(UIAlignment::TopLeft);
        sprite4->setTexture(tex1);
        sprite4->setPosition(96, 0, 0);
        sprite4->setColorScale(Color(1, 0, 0, 1));
		Engine::mainUIView()->addElement(sprite4);

        //* [ ] BlendColor
        auto sprite5 = makeObject_deprecated<UISprite>();
		sprite5->setAlignments(UIAlignment::TopLeft);
        sprite5->setTexture(tex1);
        sprite5->setPosition(0, 32, 0);
        sprite5->setBlendColor(Color(1, 0, 0, 1));
		Engine::mainUIView()->addElement(sprite5);

        //* [ ] Tone
        auto sprite6 = makeObject_deprecated<UISprite>();
		sprite6->setAlignments(UIAlignment::TopLeft);
        sprite6->setTexture(tex1);
        sprite6->setPosition(32, 32, 0);
        sprite6->setTone(ColorTone(0.5, 0.3, 0.1, 1.0));
		Engine::mainUIView()->addElement(sprite6);

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Result/Test_UI_UIElement-RenderEffects-1.png"));
        LN_TEST_CLEAN_SCENE;
	}
}

//------------------------------------------------------------------------------
//## Background
TEST_F(Test_UI_UIElement, Background)
{
	//auto e1 = makeObject_deprecated<UIElement>();

	//e1->setBackgroundColor(Color::Red);

	//TestEnv::updateFrame();
	//ASSERT_SCREEN_S(LN_ASSETFILE("Result/Test_UI_UIElement-Background-1.png"));
	//LN_TEST_CLEAN_SCENE;
}

