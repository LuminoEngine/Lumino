#include "Common.hpp"

//==============================================================================
class Test_Graphics_Texture : public ::testing::Test {};

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_Texture, clear)
{
    auto font = Font::create();
    auto texture = Texture2D::create(100, 100);

    //* [ ] クリア
    {
        texture->clear(Color::Red);

        auto sprite = UISprite::create(texture);
        sprite->setBlendMode(BlendMode::Alpha);

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Result/Graphics/Test_Graphics_Texture-clear-1.png"));
        LN_TEST_CLEAN_SCENE;
    }

    //* [ ] 別色クリア
    {
        texture->clear(Color::Green);

        auto sprite = UISprite::create(texture);
        sprite->setBlendMode(BlendMode::Alpha);

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Result/Graphics/Test_Graphics_Texture-clear-2.png"));
        LN_TEST_CLEAN_SCENE;
    }
}

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_Texture, drawText)
{
	auto font = Font::create();

	//* [ ] 余白がある場合の配置テスト
	{
		auto texture = Texture2D::create(160, 120);
		texture->drawText(u"Left", Rect(10, 10, 140, 100), font, Color::White, TextAlignment::Left);
		texture->drawText(u"Center", Rect(10, 10, 140, 100), font, Color::White, TextAlignment::Center);
		texture->drawText(u"Right", Rect(10, 10, 140, 100), font, Color::White, TextAlignment::Right);
		texture->drawText(u"Justify", Rect(10, 50, 140, 100), font, Color::White, TextAlignment::Justify);
		auto sprite = UISprite::create(texture);
		sprite->setBlendMode(BlendMode::Alpha);

		TestEnv::updateFrame();
		ASSERT_SCREEN_S(LN_ASSETFILE("Result/Graphics/Test_Graphics_Texture-drawText-2.png"));
		LN_TEST_CLEAN_SCENE;
	}

	//* [ ] 余白がない場合の配置テスト
	{
		auto texture = Texture2D::create(160, 120);
		texture->drawText(u"Left", Rect(0, 0, 160, 120), font, Color::White, TextAlignment::Left);
		texture->drawText(u"Center", Rect(0, 0, 160, 120), font, Color::White, TextAlignment::Center);
		texture->drawText(u"Right", Rect(0, 0, 160, 120), font, Color::White, TextAlignment::Right);
		texture->drawText(u"Justify", Rect(0, 30, 160, 160), font, Color::White, TextAlignment::Justify);
		auto sprite = UISprite::create(texture);
		sprite->setBlendMode(BlendMode::Alpha);

		TestEnv::updateFrame();
		ASSERT_SCREEN_S(LN_ASSETFILE("Result/Graphics/Test_Graphics_Texture-drawText-1.png"));
		LN_TEST_CLEAN_SCENE;
	}
}

#if 0

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_Texture, setPixel)
{
	auto tex1 = Texture2D::create(2, 2, TextureFormat::R8G8B8A8);
	tex1->setPixel(0, 0, Color::Red);
	tex1->setPixel(0, 1, Color::Green);
	tex1->setPixel(1, 0, Color::Blue);
	tex1->setPixel(1, 1, Color::White);

	auto sprite = Sprite2D::create(tex1);
	sprite->setBlendMode(BlendMode::Alpha);
	Engine::update();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Texture.setPixel1.png"), 100));

	sprite->removeFromWorld();
}

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_Texture, blit)
{
	auto tex1 = Texture2D::create(LN_LOCALFILE("TestData/Numbers1.png"));
	auto tex2 = Texture2D::create(100, 100);
	tex2->blit(20 * 0, 20 * 0, tex1, RectI(8 * 0, 14 * 0, 8, 14));
	tex2->blit(20 * 1, 20 * 1, tex1, RectI(8 * 1, 14 * 1, 8, 14));
	tex2->blit(20 * 2, 20 * 2, tex1, RectI(8 * 2, 14 * 2, 8, 14));
	tex2->blit(20 * 3, 20 * 3, tex1, RectI(8 * 3, 14 * 3, 8, 14));

	auto sprite = Sprite2D::create(tex2);
	sprite->setBlendMode(BlendMode::Alpha);
	Engine::update();

	// TODO: 完全一致するはずだが Rが1違う
	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Texture.Blit1.png"), 95));

	// TODO: destroy のほうがいいかな
	sprite->removeFromWorld();
}
#endif

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_Texture, Issues)
{
    //* [ ] <Issue> Clear の後に Drawtext すると、Clear されていない問題の修正。
    //* [ ] <Issue> Drawtext 連続で描画すると、1フレーム前の内容がクリアされる問題の修正。
    {
        auto font = Font::create();
        auto texture = Texture2D::create(160, 120);
        texture->drawText(_LT("Clear0"), Rect(60, 0, 160, 120), font, Color::White, TextAlignment::Left);
        auto sprite = UISprite::create(texture);
        sprite->setBlendMode(BlendMode::Alpha);
        TestEnv::updateFrame();
        // ^ "Clear0" disappears

        // line 1 : "Clear1"
        texture->clear(Color(0, 0, 0, 0));
        texture->drawText(_LT("Clear1"), Rect(0, 0, 160, 120), font, Color::White, TextAlignment::Left);
        TestEnv::updateFrame();

        // line 2 : "Clear2"
        texture->drawText(_LT("Clear2"), Rect(0, 32, 160, 120), font, Color::White, TextAlignment::Left);
        TestEnv::updateFrame();

        ASSERT_SCREEN_S(LN_ASSETFILE("Result/Graphics/Test_Graphics_Texture-Issues-1.png"));
        LN_TEST_CLEAN_SCENE;
    }
}
