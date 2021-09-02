#include "Common.hpp"
#include "../src/Graphics/RenderTargetTextureCache.hpp"

//==============================================================================
class Test_Graphics_Texture : public ::testing::Test {};

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_Texture, clear)
{
    auto texture = Texture2D::create(100, 100);

    //* [ ] クリア
    {
        texture->clear(Color::Red);

        auto sprite = UISprite::create(texture);
		sprite->setAlignments(UIHAlignment::Left, UIVAlignment::Top);
        sprite->setBlendMode(BlendMode::Alpha);
		Engine::mainUIView()->addElement(sprite);

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Graphics/Result/Test_Graphics_Texture-clear-1.png"));
        LN_TEST_CLEAN_SCENE;
    }

    //* [ ] 別色クリア
    {
#ifdef LN_NEW_GRAPHICS_MIGRATION
        // TODO: GraphicsContext への map 実装が必要
#else
        texture->clear(Color::Green);

        auto sprite = UISprite::create(texture);
        sprite->setBlendMode(BlendMode::Alpha);
		Engine::mainUIView()->addElement(sprite);

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Graphics/Result/Test_Graphics_Texture-clear-2.png"));
        LN_TEST_CLEAN_SCENE;
#endif
    }
}

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_Texture, drawText)
{
	auto font = Font::create();

	//* [ ] 余白がある場合の配置テスト
	{
		auto texture = Texture2D::create(160, 120);
		texture->drawText(u"Left", Rect(10, 10, 140, 100), font, Color::White, TextAlignment::Forward);
		texture->drawText(u"Center", Rect(10, 10, 140, 100), font, Color::White, TextAlignment::Center);
		texture->drawText(u"Right", Rect(10, 10, 140, 100), font, Color::White, TextAlignment::Backward);
		texture->drawText(u"Justify", Rect(10, 50, 140, 100), font, Color::White, TextAlignment::Justify);
		auto sprite = UISprite::create(texture);
		sprite->setAlignments(UIHAlignment::Left, UIVAlignment::Top);
		sprite->setBlendMode(BlendMode::Alpha);
		Engine::mainUIView()->addElement(sprite);

		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("Graphics/Result/Test_Graphics_Texture-drawText-2.png"));
		LN_TEST_CLEAN_SCENE;
	}

	//* [ ] 余白がない場合の配置テスト
	{
		auto texture = Texture2D::create(160, 120);
		texture->drawText(u"Left", Rect(0, 0, 160, 120), font, Color::White, TextAlignment::Forward);
		texture->drawText(u"Center", Rect(0, 0, 160, 120), font, Color::White, TextAlignment::Center);
		texture->drawText(u"Right", Rect(0, 0, 160, 120), font, Color::White, TextAlignment::Backward);
		texture->drawText(u"Justify", Rect(0, 30, 160, 160), font, Color::White, TextAlignment::Justify);
		auto sprite = UISprite::create(texture);
		sprite->setAlignments(UIHAlignment::Left, UIVAlignment::Top);
		sprite->setBlendMode(BlendMode::Alpha);
		Engine::mainUIView()->addElement(sprite);

		TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Graphics/Result/Test_Graphics_Texture-drawText-1.png"));
		LN_TEST_CLEAN_SCENE;
	}
}

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_Texture, setPixel)
{
	auto tex1 = Texture2D::create(2, 2);
	tex1->setPixel(0, 0, Color::Red);
	tex1->setPixel(1, 0, Color::Blue);
	tex1->setPixel(0, 1, Color::Green);
	tex1->setPixel(1, 1, Color::White);

	auto sprite = UISprite::create(tex1);
	sprite->setAlignments(UIHAlignment::Left, UIVAlignment::Top);
    sprite->setScale(20);
	Engine::mainUIView()->addElement(sprite);

    TestEnv::updateFrame();
    ASSERT_SCREEN(LN_ASSETFILE("Graphics/Result/Test_Graphics_Texture-setPixel-1.png"));
    LN_TEST_CLEAN_SCENE;
}

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_Texture, blit)
{
	auto tex1 = Texture2D::load(LN_ASSETFILE("Graphics/Numbers1.png"));
	auto tex2 = Texture2D::create(100, 100);
	tex2->blit(20 * 0, 20 * 0, tex1, 8 * 0, 14 * 0, 8, 14);
	tex2->blit(20 * 1, 20 * 1, tex1, 8 * 1, 14 * 1, 8, 14);
	tex2->blit(20 * 2, 20 * 2, tex1, 8 * 2, 14 * 2, 8, 14);
	tex2->blit(20 * 3, 20 * 3, tex1, 8 * 3, 14 * 3, 8, 14);

    auto sprite = UISprite::create(tex2);
	sprite->setAlignments(UIHAlignment::Left, UIVAlignment::Top);
	sprite->setBlendMode(BlendMode::Alpha);
    TestEnv::updateFrame();
	Engine::mainUIView()->addElement(sprite);

    ASSERT_SCREEN(LN_ASSETFILE("Graphics/Result/Test_Graphics_Texture-blit-1.png"));
    LN_TEST_CLEAN_SCENE;
}

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_Texture, Issues)
{
    //* [ ] <Issue> Clear の後に Drawtext すると、Clear されていない問題の修正。
    //* [ ] <Issue> Drawtext 連続で描画すると、1フレーム前の内容がクリアされる問題の修正。
    {
#ifdef LN_NEW_GRAPHICS_MIGRATION
        // TODO: GraphicsContext への map 実装が必要
#else
        auto font = Font::create();
        auto texture = Texture2D::create(160, 120);
        texture->drawText(_LT("Clear0"), Rect(60, 0, 160, 120), font, Color::White, TextAlignment::Left);
        auto sprite = UISprite::create(texture);
		sprite->setAlignments(UIHAlignment::Left, UIVAlignment::Top);
        sprite->setBlendMode(BlendMode::Alpha);
		Engine::mainUIView()->addElement(sprite);

        TestEnv::updateFrame();
        // ^ "Clear0" disappears

        // line 1 : "Clear1"
        texture->clear(Color(0, 0, 0, 0));
        texture->drawText(_LT("Clear1"), Rect(0, 0, 160, 120), font, Color::White, TextAlignment::Left);
        TestEnv::updateFrame();

        // line 2 : "Clear2"
        texture->drawText(_LT("Clear2"), Rect(0, 32, 160, 120), font, Color::White, TextAlignment::Left);
        TestEnv::updateFrame();

        ASSERT_SCREEN(LN_ASSETFILE("Graphics/Result/Test_Graphics_Texture-Issues-1.png"));
        LN_TEST_CLEAN_SCENE;
#endif
    }
}


//==============================================================================
class Test_Graphics_FrameBufferCache : public ::testing::Test {};

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_FrameBufferCache, Basic)
{
	auto m1 = makeRef<detail::RenderTargetTextureCacheManager>();
	auto m2 = makeRef<detail::DepthBufferCacheManager>();
	auto cache1 = makeRef<detail::FrameBufferCache>(m1, m2);
	auto t1 = cache1->requestRenderTargetTexture2(SizeI(32, 32), TextureFormat::RGBA8, false);
	auto t2 = cache1->requestRenderTargetTexture2(SizeI(32, 32), TextureFormat::RGBA8, false);
	auto t3 = cache1->requestRenderTargetTexture2(SizeI(16, 16), TextureFormat::RGBA8, false);
	auto t4 = cache1->requestRenderTargetTexture2(SizeI(16, 16), TextureFormat::RGBA8, false);
	auto d1 = cache1->requestDepthBuffer2(SizeI(32, 32));
	auto d2 = cache1->requestDepthBuffer2(SizeI(32, 32));
	auto d3 = cache1->requestDepthBuffer2(SizeI(16, 16));
	auto d4 = cache1->requestDepthBuffer2(SizeI(16, 16));

	// 同じものを間違って取り出したりしていないこと
	ASSERT_NE(t1, t2);
	ASSERT_NE(t3, t4);
	ASSERT_NE(d1, d2);
	ASSERT_NE(d3, d4);

	auto t1w = WeakRefPtr<RenderTargetTexture>(t1);
	auto t2w = WeakRefPtr<RenderTargetTexture>(t2);
	auto t3w = WeakRefPtr<RenderTargetTexture>(t3);
	auto t4w = WeakRefPtr<RenderTargetTexture>(t4);
	auto d1w = WeakRefPtr<DepthBuffer>(d1);
	auto d2w = WeakRefPtr<DepthBuffer>(d2);
	auto d3w = WeakRefPtr<DepthBuffer>(d3);
	auto d4w = WeakRefPtr<DepthBuffer>(d4);

	// 一度 GC. 特に消えたりしない
	{
		cache1->gcObjects();
		ASSERT_EQ(4, cache1->aliveRenderTargetCount());
		ASSERT_EQ(4, cache1->aliveDepthBufferCount());
	}

	// 参照を外して GC. FrameBufferCache からは除外されるが、インスタンスはまだ生きている
	{
		t4 = nullptr;
		d4 = nullptr;
		cache1->gcObjects();
		ASSERT_EQ(3, cache1->aliveRenderTargetCount());
		ASSERT_EQ(3, cache1->aliveDepthBufferCount());
		ASSERT_EQ(true, t4w.isAlive());
		ASSERT_EQ(true, d4w.isAlive());
	}

	// 同じパラメータで取得しなおす. 前回のインスタンスが返される
	{
		t4 = cache1->requestRenderTargetTexture2(SizeI(16, 16), TextureFormat::RGBA8, false);
		d4 = cache1->requestDepthBuffer2(SizeI(16, 16));
		ASSERT_EQ(4, cache1->aliveRenderTargetCount());
		ASSERT_EQ(4, cache1->aliveDepthBufferCount());
		ASSERT_EQ(t4, t4w.resolve());
		ASSERT_EQ(d4, d4w.resolve());
	}

	// 参照を外してしばらく GC すると、インスタンスが消える
	{
		t4 = nullptr;
		d4 = nullptr;
		for (int i = 0; i <= detail::TemporaryGraphicsResourceObjectCacheBase::HoldFrameCount; i++) {
			cache1->gcObjects();
		}
		ASSERT_EQ(3, cache1->aliveRenderTargetCount());
		ASSERT_EQ(3, cache1->aliveDepthBufferCount());
		ASSERT_EQ(false, t4w.isAlive());
		ASSERT_EQ(false, d4w.isAlive());
	}

	// 明示的に release. 直後に request すると、同じインスタンスが得られる (できるだけ無駄なく再利用)
	{
		cache1->release(t3);
		cache1->release(d3);
		auto t3_ = cache1->requestRenderTargetTexture2(SizeI(16, 16), TextureFormat::RGBA8, false);
		auto d3_ = cache1->requestDepthBuffer2(SizeI(16, 16));
		ASSERT_EQ(t3, t3_);
		ASSERT_EQ(d3, d3_);
	}

	// すべて GC でクリア
	{
		t1 = nullptr;
		t2 = nullptr;
		t3 = nullptr;
		d1 = nullptr;
		d2 = nullptr;
		d3 = nullptr;
		for (int i = 0; i <= detail::TemporaryGraphicsResourceObjectCacheBase::HoldFrameCount; i++) {
			cache1->gcObjects();
		}
		ASSERT_EQ(false, t1w.isAlive());
		ASSERT_EQ(false, t2w.isAlive());
		ASSERT_EQ(false, t3w.isAlive());
		ASSERT_EQ(false, d1w.isAlive());
		ASSERT_EQ(false, d2w.isAlive());
		ASSERT_EQ(false, d3w.isAlive());
	}
}
