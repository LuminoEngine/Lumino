#include "Common.hpp"

//==============================================================================
class Test_Graphics_Texture : public ::testing::Test {};

#if 0
//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_Texture, drawText)
{
    auto font = Font::create();
    auto texture = Texture2D::create(160, 120);
    texture->drawText(_LT("Left"), Rect(0, 0, 160, 120), font, Color::White, TextAlignment::Left);
    texture->drawText(_LT("Center"), Rect(0, 0, 160, 120), font, Color::White, TextAlignment::Center);
    texture->drawText(_LT("Rigth"), Rect(0, 0, 160, 120), font, Color::White, TextAlignment::Right);
    //texture->drawText("Justify", Rect(0, 32, 120, 160), font, Color32::White, Color32::White, 0, TextAlignment::Justify);
    auto sprite = Sprite2D::create(texture);
    sprite->setBlendMode(BlendMode::Alpha);
    Engine::update();

    ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Texture.DrawText1.png"), 99));

    sprite->removeFromWorld();
}

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


//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_Texture, Issues)
{
	// <Issue> Clear の後に Drawtext すると、Clear されていない問題の修正。
	// <Issue> Drawtext 連続で描画すると、1フレーム前の内容がクリアされる問題の修正。
	{
		auto font = Font::create();
		auto texture = Texture2D::create(160, 120);
		texture->drawText(_LT("__________"), RectI(0, 0, 160, 120), font, Color32::White, Color32::White, 0, TextAlignment::Left);
		auto sprite = Sprite2D::create(texture);
		Engine::update();

		texture->clear(Color32(0, 0, 0, 0));
		texture->drawText(_LT("Clear1"), RectI(0, 0, 160, 120), font, Color32::White, Color32::White, 0, TextAlignment::Left);
		Engine::update();

		texture->drawText(_LT("Clear2"), RectI(0, 32, 160, 120), font, Color32::White, Color32::White, 0, TextAlignment::Left);
		Engine::update();

		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Texture.Clear1.png"), 95));

		sprite->removeFromWorld();
	}
}
#endif

