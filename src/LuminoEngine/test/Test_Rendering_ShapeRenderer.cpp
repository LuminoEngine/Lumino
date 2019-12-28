#include "Common.hpp"
#include <LuminoEngine/UI/UIRenderingContext.hpp>

//==============================================================================
//# ShapeRenderer
class Test_Rendering_ShapeRenderer : public LuminoSceneTest
{
public:
	class TestElement : public UIElement
	{
	public:
		std::function<void(UIRenderingContext* context)> render;

		virtual void onRender(UIRenderingContext* context) override { render(context); }
	};

};

//------------------------------------------------------------------------------
//## Basic
TEST_F(Test_Rendering_ShapeRenderer, Basic)
{

    auto element1 = makeObject<TestElement>();
    element1->setHorizontalAlignment(HAlignment::Stretch);
    element1->setVerticalAlignment(VAlignment::Stretch);
	Engine::mainUIView()->addChild(element1);

	element1->render = [](UIRenderingContext* context)
    {
		BoxElementShapeBaseStyle baseStyle;
        BoxElementShapeBorderStyle borderStyle;
        borderStyle.borderLeftColor = Color::Red;
        borderStyle.borderTopColor = Color::Green;
        borderStyle.borderRightColor = Color::Blue;
        borderStyle.borderBottomColor = Color::Yellow;
		BoxElementShapeShadowStyle shadowStyle;
		shadowStyle.shadowInset = false;
/*
		baseStyle.baseRect = Rect(30, 30, 60, 40);
		baseStyle.cornerRadius = CornerRadius(0);
		shadowStyle.shadowWidth = 20;
		shadowStyle.shadowBlur = 10;
		shadowStyle.shadowColor = Color::Red;
		context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);*/

        baseStyle.baseRect = Rect(20, 20, 80, 40);
        baseStyle.cornerRadius = CornerRadius(10);
        borderStyle.borderThickness = Thickness(0, 10, 0, 10);
        borderStyle.borderInset = false;
        context->drawBoxElement(baseStyle, nullptr, &borderStyle, nullptr);
	};

    TestEnv::updateFrame();
    ASSERT_SCREEN_S(LN_ASSETFILE("Rendering/Result/Test_Rendering_ShapeRenderer-Basic-1.png"));
    LN_TEST_CLEAN_SCENE;
}

TEST_F(Test_Rendering_ShapeRenderer, BackgroundOnly)
{
	auto element1 = makeObject<TestElement>();
	element1->setHorizontalAlignment(HAlignment::Stretch);
	element1->setVerticalAlignment(VAlignment::Stretch);
	Engine::mainUIView()->addChild(element1);

	element1->render = [](UIRenderingContext* context)
	{
		BoxElementShapeBaseStyle baseStyle;
		BoxElementShapeBackgroundStyle backgroundStyle;

		baseStyle.baseRect = Rect(0, 0, 80, 60);
		baseStyle.cornerRadius = CornerRadius(0);
		backgroundStyle.color = Color::Red;
		context->drawBoxElement(baseStyle, &backgroundStyle, nullptr, nullptr);

		baseStyle.baseRect = Rect(80, 0, 80, 60);
		baseStyle.cornerRadius = CornerRadius(20);
		backgroundStyle.color = Color::Green;
		context->drawBoxElement(baseStyle, &backgroundStyle, nullptr, nullptr);

		baseStyle.baseRect = Rect(0, 60, 80, 60);
		baseStyle.cornerRadius = CornerRadius(20, 0, 20, 0);
		backgroundStyle.color = Color::Blue;
		context->drawBoxElement(baseStyle, &backgroundStyle, nullptr, nullptr);

		baseStyle.baseRect = Rect(80, 60, 80, 60);
		baseStyle.cornerRadius = CornerRadius(0, 20, 0, 20);
		backgroundStyle.color = Color::Yellow;
		context->drawBoxElement(baseStyle, &backgroundStyle, nullptr, nullptr);
	};

	TestEnv::updateFrame();
	ASSERT_SCREEN_S(LN_ASSETFILE("Rendering/Result/Test_Rendering_ShapeRenderer-BackgroundOnly-1.png"));
	LN_TEST_CLEAN_SCENE;
}

TEST_F(Test_Rendering_ShapeRenderer, BorderOnly)
{
	auto element1 = makeObject<TestElement>();
	element1->setHorizontalAlignment(HAlignment::Stretch);
	element1->setVerticalAlignment(VAlignment::Stretch);
	Engine::mainUIView()->addChild(element1);

	// outset, disable-corner
	{
		element1->render = [](UIRenderingContext* context)
		{
			BoxElementShapeBaseStyle baseStyle;
			BoxElementShapeBorderStyle borderStyle;
			borderStyle.borderLeftColor = Color::Red;
			borderStyle.borderTopColor = Color::Green;
			borderStyle.borderRightColor = Color::Blue;
			borderStyle.borderBottomColor = Color::Yellow;

			baseStyle.baseRect = Rect(0, 0, 60, 40);
			baseStyle.cornerRadius = CornerRadius(0);
			borderStyle.borderThickness = Thickness(10);
			borderStyle.borderInset = false;
			context->drawBoxElement(baseStyle, nullptr, &borderStyle, nullptr);

			// Drawing triangles using degeneration
			baseStyle.baseRect = Rect(80, 0, 0, 0);
			baseStyle.cornerRadius = CornerRadius(0);
			borderStyle.borderThickness = Thickness(40, 0, 40, 40);
			borderStyle.borderInset = false;
			context->drawBoxElement(baseStyle, nullptr, &borderStyle, nullptr);

			baseStyle.baseRect = Rect(0, 60, 60, 60);
			baseStyle.cornerRadius = CornerRadius(0);
			borderStyle.borderThickness = Thickness(10, 0, 10, 0);
			borderStyle.borderInset = false;
			context->drawBoxElement(baseStyle, nullptr, &borderStyle, nullptr);

			baseStyle.baseRect = Rect(80, 60, 80, 40);
			baseStyle.cornerRadius = CornerRadius(0);
			borderStyle.borderThickness = Thickness(0, 10, 0, 10);
			borderStyle.borderInset = false;
			context->drawBoxElement(baseStyle, nullptr, &borderStyle, nullptr);
		};

		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("Rendering/Result/Test_Rendering_ShapeRenderer-BorderOnly-1.png"));
	}

	// outset, enable-corner
	{
		element1->render = [](UIRenderingContext* context)
		{
			BoxElementShapeBaseStyle baseStyle;
			BoxElementShapeBorderStyle borderStyle;
			borderStyle.borderLeftColor = Color::Red;
			borderStyle.borderTopColor = Color::Green;
			borderStyle.borderRightColor = Color::Blue;
			borderStyle.borderBottomColor = Color::Yellow;

			baseStyle.baseRect = Rect(0, 0, 60, 40);
			baseStyle.cornerRadius = CornerRadius(20);
			borderStyle.borderThickness = Thickness(10);
			borderStyle.borderInset = false;
			context->drawBoxElement(baseStyle, nullptr, &borderStyle, nullptr);

			baseStyle.baseRect = Rect(80, 0, 60, 40);
			baseStyle.cornerRadius = CornerRadius(15, 10, 5, 0);
			borderStyle.borderThickness = Thickness(10);
			borderStyle.borderInset = false;
			context->drawBoxElement(baseStyle, nullptr, &borderStyle, nullptr);

			baseStyle.baseRect = Rect(0, 60, 60, 30);
			baseStyle.cornerRadius = CornerRadius(20);
			borderStyle.borderThickness = Thickness(10, 0, 10, 30);
			borderStyle.borderInset = false;
			context->drawBoxElement(baseStyle, nullptr, &borderStyle, nullptr);

			baseStyle.baseRect = Rect(80, 60, 80, 40);
			baseStyle.cornerRadius = CornerRadius(10);
			borderStyle.borderThickness = Thickness(0, 10, 0, 10);
			borderStyle.borderInset = false;
			context->drawBoxElement(baseStyle, nullptr, &borderStyle, nullptr);
		};

		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("Rendering/Result/Test_Rendering_ShapeRenderer-BorderOnly-2.png"));
	}

	// inset, disable-corner
	{
		element1->render = [](UIRenderingContext* context)
		{
			BoxElementShapeBaseStyle baseStyle;
			BoxElementShapeBorderStyle borderStyle;
			borderStyle.borderLeftColor = Color::Red;
			borderStyle.borderTopColor = Color::Green;
			borderStyle.borderRightColor = Color::Blue;
			borderStyle.borderBottomColor = Color::Yellow;

			baseStyle.baseRect = Rect(0, 0, 80, 60);
			baseStyle.cornerRadius = CornerRadius(0);
			borderStyle.borderThickness = Thickness(10);
			borderStyle.borderInset = true;
			context->drawBoxElement(baseStyle, nullptr, &borderStyle, nullptr);

			// Drawing triangles using degeneration
			baseStyle.baseRect = Rect(80, 0, 80, 40);
			baseStyle.cornerRadius = CornerRadius(0);
			borderStyle.borderThickness = Thickness(40, 0, 40, 40);
			borderStyle.borderInset = true;
			context->drawBoxElement(baseStyle, nullptr, &borderStyle, nullptr);

			baseStyle.baseRect = Rect(0, 60, 80, 60);
			baseStyle.cornerRadius = CornerRadius(0);
			borderStyle.borderThickness = Thickness(10, 0, 10, 0);
			borderStyle.borderInset = true;
			context->drawBoxElement(baseStyle, nullptr, &borderStyle, nullptr);

			baseStyle.baseRect = Rect(80, 60, 80, 60);
			baseStyle.cornerRadius = CornerRadius(0);
			borderStyle.borderThickness = Thickness(0, 10, 0, 10);
			borderStyle.borderInset = true;
			context->drawBoxElement(baseStyle, nullptr, &borderStyle, nullptr);
		};

		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("Rendering/Result/Test_Rendering_ShapeRenderer-BorderOnly-3.png"));
	}

	// outset, enable-corner
	{
		element1->render = [](UIRenderingContext* context)
		{
			BoxElementShapeBaseStyle baseStyle;
			BoxElementShapeBorderStyle borderStyle;
			borderStyle.borderLeftColor = Color::Red;
			borderStyle.borderTopColor = Color::Green;
			borderStyle.borderRightColor = Color::Blue;
			borderStyle.borderBottomColor = Color::Yellow;

			baseStyle.baseRect = Rect(0, 0, 80, 60);
			baseStyle.cornerRadius = CornerRadius(20);
			borderStyle.borderThickness = Thickness(10);
			borderStyle.borderInset = true;
			context->drawBoxElement(baseStyle, nullptr, &borderStyle, nullptr);

			baseStyle.baseRect = Rect(80, 0, 80, 60);
			baseStyle.cornerRadius = CornerRadius(15, 10, 5, 0);
			borderStyle.borderThickness = Thickness(10);
			borderStyle.borderInset = true;
			context->drawBoxElement(baseStyle, nullptr, &borderStyle, nullptr);

			baseStyle.baseRect = Rect(0, 60, 80, 60);
			baseStyle.cornerRadius = CornerRadius(20);
			borderStyle.borderThickness = Thickness(10, 0, 10, 30);
			borderStyle.borderInset = true;
			context->drawBoxElement(baseStyle, nullptr, &borderStyle, nullptr);

			baseStyle.baseRect = Rect(80, 60, 80, 60);
			baseStyle.cornerRadius = CornerRadius(10);
			borderStyle.borderThickness = Thickness(0, 10, 0, 10);
			borderStyle.borderInset = true;
			context->drawBoxElement(baseStyle, nullptr, &borderStyle, nullptr);
		};

		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("Rendering/Result/Test_Rendering_ShapeRenderer-BorderOnly-4.png"));
	}

	//  0 size
	{
		element1->render = [](UIRenderingContext* context)
		{
			BoxElementShapeBaseStyle baseStyle;
			BoxElementShapeBorderStyle borderStyle;
			borderStyle.borderLeftColor = Color::Red;
			borderStyle.borderTopColor = Color::Green;
			borderStyle.borderRightColor = Color::Blue;
			borderStyle.borderBottomColor = Color::Yellow;

			// Note: CSS はデフォルトで inset. Box のサイズは、Border の合計サイズよりも小さくすることはできない。(Style で width: 0px とかはできるが、表示時に調整される)
			// この時 Corner が Border のサイズを超えた場合の動作は未定義とする。(CSS でもなんかおかしくなる)

			baseStyle.baseRect = Rect(0, 0, 0, 0);
			baseStyle.cornerRadius = CornerRadius(0);
			borderStyle.borderThickness = Thickness(30);
			borderStyle.borderInset = false;
			context->drawBoxElement(baseStyle, nullptr, &borderStyle, nullptr);

			// CornerRadius と Thickness が一致している場合、円を作ることができる
			baseStyle.baseRect = Rect(80, 0, 0, 0);
			baseStyle.cornerRadius = CornerRadius(30);
			borderStyle.borderThickness = Thickness(30);
			borderStyle.borderInset = false;
			context->drawBoxElement(baseStyle, nullptr, &borderStyle, nullptr);

			baseStyle.baseRect = Rect(0, 60, 0, 0);
			baseStyle.cornerRadius = CornerRadius(0);
			borderStyle.borderThickness = Thickness(30);
			borderStyle.borderInset = true;
			context->drawBoxElement(baseStyle, nullptr, &borderStyle, nullptr);

			// CornerRadius と Thickness が一致している場合、円を作ることができる
			baseStyle.baseRect = Rect(80, 60, 0, 0);
			baseStyle.cornerRadius = CornerRadius(30);
			borderStyle.borderThickness = Thickness(30);
			borderStyle.borderInset = true;
			context->drawBoxElement(baseStyle, nullptr, &borderStyle, nullptr);
		};

		TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Rendering/Result/Test_Rendering_ShapeRenderer-BorderOnly-5.png"));
	}

	LN_TEST_CLEAN_SCENE;
}

TEST_F(Test_Rendering_ShapeRenderer, ShadowOnly)
{
    auto element1 = makeObject<TestElement>();
    element1->setHorizontalAlignment(HAlignment::Stretch);
    element1->setVerticalAlignment(VAlignment::Stretch);
    Engine::mainUIView()->addChild(element1);

    //// outset
    //{
    //    element1->render = [](UIRenderingContext* context)
    //    {
    //        
    //        BoxElementShapeBaseStyle baseStyle;
    //        BoxElementShapeShadowStyle shadowStyle;
    //        shadowStyle.shadowInset = false;

    //        baseStyle.baseRect = Rect(20, 20, 40, 20);
    //        baseStyle.cornerRadius = CornerRadius(0);
    //        shadowStyle.shadowWidth = 10;
    //        shadowStyle.shadowBlur = 0;
    //        shadowStyle.shadowColor = Color::Red;
    //        context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

    //        baseStyle.baseRect = Rect(100, 20, 40, 20);
    //        baseStyle.cornerRadius = CornerRadius(0);
    //        shadowStyle.shadowWidth = 10;
    //        shadowStyle.shadowBlur = 5;
    //        shadowStyle.shadowColor = Color::Green;
    //        context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

    //        baseStyle.baseRect = Rect(20, 80, 40, 20);
    //        baseStyle.cornerRadius = CornerRadius(0);
    //        shadowStyle.shadowWidth = 10;
    //        shadowStyle.shadowBlur = 10;
    //        shadowStyle.shadowColor = Color::Blue;
    //        context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

    //        baseStyle.baseRect = Rect(100, 80, 40, 20);
    //        baseStyle.cornerRadius = CornerRadius(0);
    //        shadowStyle.shadowWidth = 10;
    //        shadowStyle.shadowBlur = 20;
    //        shadowStyle.shadowColor = Color::Yellow;
    //        context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);
    //    };

    //    TestEnv::updateFrame();
    //    ASSERT_SCREEN_S(LN_ASSETFILE("Rendering/Result/Test_Rendering_ShapeRenderer-ShadowOnly-1.png"));
    //}

    //// outset-corner
    //{
    //    element1->render = [](UIRenderingContext* context)
    //    {
    //        BoxElementShapeBaseStyle baseStyle;
    //        BoxElementShapeShadowStyle shadowStyle;
    //        shadowStyle.shadowInset = false;

    //        // non-blur
    //        baseStyle.baseRect = Rect(20, 20, 40, 20);
    //        baseStyle.cornerRadius = CornerRadius(10);
    //        shadowStyle.shadowWidth = 10;
    //        shadowStyle.shadowBlur = 0;
    //        shadowStyle.shadowColor = Color::Red;
    //        context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

    //        // mini-blur & big-radius
    //        // サイズより大きい CornerRadius を指定しても、サイズ内に収まるように比率を考慮して縮小する
    //        baseStyle.baseRect = Rect(100, 20, 40, 20);
    //        baseStyle.cornerRadius = CornerRadius(130);
    //        shadowStyle.shadowWidth = 10;
    //        shadowStyle.shadowBlur = 10;
    //        shadowStyle.shadowColor = Color::Green;
    //        context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

    //        // fit-blur
    //        baseStyle.baseRect = Rect(20, 80, 40, 20);
    //        baseStyle.cornerRadius = CornerRadius(10);
    //        shadowStyle.shadowWidth = 10;
    //        shadowStyle.shadowBlur = 20;
    //        shadowStyle.shadowColor = Color::Blue;
    //        context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

    //        // big-blur
    //        baseStyle.baseRect = Rect(100, 80, 40, 20);
    //        baseStyle.cornerRadius = CornerRadius(10);
    //        shadowStyle.shadowWidth = 5;
    //        shadowStyle.shadowBlur = 30;
    //        shadowStyle.shadowColor = Color::Yellow;
    //        context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);
    //    };

    //    TestEnv::updateFrame();
    //    ASSERT_SCREEN_S(LN_ASSETFILE("Rendering/Result/Test_Rendering_ShapeRenderer-ShadowOnly-2.png"));
    //}

    // outset-offset
    {
        element1->render = [](UIRenderingContext* context)
        {
            BoxElementShapeBaseStyle baseStyle;
            BoxElementShapeShadowStyle shadowStyle;
            shadowStyle.shadowInset = false;

            baseStyle.baseRect = Rect(20, 20, 40, 20);
            baseStyle.cornerRadius = CornerRadius(0);
            shadowStyle.shadowWidth = 10;
            shadowStyle.shadowBlur = 10;
            shadowStyle.shadowColor = Color::Red;
            shadowStyle.shadowOffset = Vector2(0, 10);
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

            baseStyle.baseRect = Rect(100, 20, 40, 20);
            baseStyle.cornerRadius = CornerRadius(10);
            shadowStyle.shadowWidth = 10;
            shadowStyle.shadowBlur = 10;
            shadowStyle.shadowColor = Color::Green;
            shadowStyle.shadowOffset = Vector2(0, 10);
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

            // [Limitation] Blur 幅以上のオフセットは描画が崩れる。一応動作を見ておく。
            baseStyle.baseRect = Rect(20, 80, 40, 20);
            baseStyle.cornerRadius = CornerRadius(0);
            shadowStyle.shadowWidth = 10;
            shadowStyle.shadowBlur = 10;
            shadowStyle.shadowColor = Color::Red;
            shadowStyle.shadowOffset = Vector2(0, 15);
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

            // [Limitation] Blur 幅以上のオフセットは描画が崩れる。一応動作を見ておく。
            baseStyle.baseRect = Rect(100, 80, 40, 20);
            baseStyle.cornerRadius = CornerRadius(10);
            shadowStyle.shadowWidth = 10;
            shadowStyle.shadowBlur = 10;
            shadowStyle.shadowColor = Color::Green;
            shadowStyle.shadowOffset = Vector2(0, 15);
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);
        };

        TestEnv::updateFrame();
        ASSERT_SCREEN_S(LN_ASSETFILE("Rendering/Result/Test_Rendering_ShapeRenderer-ShadowOnly-3.png"));
    }

    // inset
    {
        element1->render = [](UIRenderingContext* context)
        {
            BoxElementShapeBaseStyle baseStyle;
            BoxElementShapeShadowStyle shadowStyle;
            shadowStyle.shadowInset = true;

            baseStyle.baseRect = Rect(0, 0, 80, 60);
            baseStyle.cornerRadius = CornerRadius(0);
            shadowStyle.shadowWidth = 10;
            shadowStyle.shadowBlur = 0;
            shadowStyle.shadowColor = Color::Red;
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

            baseStyle.baseRect = Rect(80, 0, 80, 60);
            baseStyle.cornerRadius = CornerRadius(0);
            shadowStyle.shadowWidth = 10;
            shadowStyle.shadowBlur = 5;
            shadowStyle.shadowColor = Color::Green;
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

            baseStyle.baseRect = Rect(0, 60, 80, 60);
            baseStyle.cornerRadius = CornerRadius(0);
            shadowStyle.shadowWidth = 10;
            shadowStyle.shadowBlur = 10;
            shadowStyle.shadowColor = Color::Blue;
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

            baseStyle.baseRect = Rect(80, 60, 80, 60);
            baseStyle.cornerRadius = CornerRadius(0);
            shadowStyle.shadowWidth = 10;
            shadowStyle.shadowBlur = 20;
            shadowStyle.shadowColor = Color::Yellow;
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);
        };

        TestEnv::updateFrame();
        ASSERT_SCREEN_S(LN_ASSETFILE("Rendering/Result/Test_Rendering_ShapeRenderer-ShadowOnly-3.png"));
    }

    // inset-corner
    {
        element1->render = [](UIRenderingContext* context)
        {
            BoxElementShapeBaseStyle baseStyle;
            BoxElementShapeShadowStyle shadowStyle;
            shadowStyle.shadowInset = true;

            baseStyle.baseRect = Rect(0, 0, 80, 60);
            baseStyle.cornerRadius = CornerRadius(30);
            shadowStyle.shadowWidth = 10;
            shadowStyle.shadowBlur = 0;
            shadowStyle.shadowColor = Color::Red;
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

            baseStyle.baseRect = Rect(80, 0, 80, 60);
            baseStyle.cornerRadius = CornerRadius(20);
            shadowStyle.shadowWidth = 10;
            shadowStyle.shadowBlur = 5;
            shadowStyle.shadowColor = Color::Green;
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

            baseStyle.baseRect = Rect(0, 60, 80, 60);
            baseStyle.cornerRadius = CornerRadius(10);
            shadowStyle.shadowWidth = 10;
            shadowStyle.shadowBlur = 10;
            shadowStyle.shadowColor = Color::Blue;
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

            baseStyle.baseRect = Rect(80, 60, 80, 60);
            baseStyle.cornerRadius = CornerRadius(5);
            shadowStyle.shadowWidth = 10;
            shadowStyle.shadowBlur = 20;
            shadowStyle.shadowColor = Color::Yellow;
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);
        };

        TestEnv::updateFrame();
        ASSERT_SCREEN_S(LN_ASSETFILE("Rendering/Result/Test_Rendering_ShapeRenderer-ShadowOnly-4.png"));
    }

    // 0 size
    {
        element1->render = [](UIRenderingContext* context)
        {
            BoxElementShapeBaseStyle baseStyle;
            BoxElementShapeShadowStyle shadowStyle;

            baseStyle.baseRect = Rect(10, 10, 0, 0);
            shadowStyle.shadowWidth = 10;
            shadowStyle.shadowBlur = 5;
            shadowStyle.shadowColor = Color::Red;
            shadowStyle.shadowInset = false;
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

            baseStyle.baseRect = Rect(70, 10, 0, 0);
            baseStyle.cornerRadius = CornerRadius(5);
            shadowStyle.shadowWidth = 10;
            shadowStyle.shadowBlur = 5;
            shadowStyle.shadowColor = Color::Blue;
            shadowStyle.shadowInset = false;
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

            // Size 0 だと inset は描画されない
            baseStyle.baseRect = Rect(0, 60, 0, 0);
            shadowStyle.shadowWidth = 10;
            shadowStyle.shadowBlur = 5;
            shadowStyle.shadowColor = Color::Green;
            shadowStyle.shadowInset = true;
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

            baseStyle.baseRect = Rect(80, 60, 20, 20);
            baseStyle.cornerRadius = CornerRadius(10);
            shadowStyle.shadowWidth = 10;
            shadowStyle.shadowBlur = 5;
            shadowStyle.shadowColor = Color::Yellow;
            shadowStyle.shadowInset = true;
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);
        };

        TestEnv::updateFrame();
        ASSERT_SCREEN_S(LN_ASSETFILE("Rendering/Result/Test_Rendering_ShapeRenderer-ShadowOnly-5.png"));
    }

    // offset
    {
        element1->render = [](UIRenderingContext* context)
        {
            BoxElementShapeBaseStyle baseStyle;
            BoxElementShapeShadowStyle shadowStyle;

            //baseStyle.baseRect = Rect(20, 20, 20, 20);
            //baseStyle.cornerRadius = CornerRadius(5);
            //shadowStyle.shadowWidth = 20;
            //shadowStyle.shadowBlur = 10;
            //shadowStyle.shadowColor = Color::Red;
            //shadowStyle.shadowOffset = Vector2(5, 0);
            //shadowStyle.shadowInset = false;
            //context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

            baseStyle.baseRect = Rect(100, 20, 20, 20);
            baseStyle.cornerRadius = CornerRadius(0);
            shadowStyle.shadowWidth = 20;
            shadowStyle.shadowBlur = 10;
            shadowStyle.shadowColor = Color::Green;
            shadowStyle.shadowOffset = Vector2(0, 10);
            shadowStyle.shadowInset = false;
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

            //baseStyle.baseRect = Rect(20, 70, 40, 40);
            //baseStyle.cornerRadius = CornerRadius(5);
            //shadowStyle.shadowWidth = 20;
            //shadowStyle.shadowBlur = 10;
            //shadowStyle.shadowColor = Color::Red;
            //shadowStyle.shadowOffset = Vector2(5, 0);
            //shadowStyle.shadowInset = true;
            //context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

            //baseStyle.baseRect = Rect(100, 70, 40, 40);
            //baseStyle.cornerRadius = CornerRadius(5);
            //shadowStyle.shadowWidth = 20;
            //shadowStyle.shadowBlur = 10;
            //shadowStyle.shadowColor = Color::Green;
            //shadowStyle.shadowOffset = Vector2(15, 0);
            //shadowStyle.shadowInset = true;
            //context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);
        };

        TestEnv::updateFrame();
        ASSERT_SCREEN_S(LN_ASSETFILE("Rendering/Result/Test_Rendering_ShapeRenderer-ShadowOnly-6.png"));
    }

    LN_TEST_CLEAN_SCENE;
}

TEST_F(Test_Rendering_ShapeRenderer, BorderIlls)
{
	auto element1 = makeObject<TestElement>();
	element1->setHorizontalAlignment(HAlignment::Stretch);
	element1->setVerticalAlignment(VAlignment::Stretch);
	Engine::mainUIView()->addChild(element1);

	element1->render = [](UIRenderingContext* context)
	{
		BoxElementShapeBaseStyle baseStyle;
		BoxElementShapeBorderStyle borderStyle;

		// 厚さ 0 なので何も描かれない
		baseStyle.baseRect = Rect(0, 0, 80, 60);
		baseStyle.cornerRadius = CornerRadius(0);
		borderStyle.borderThickness = Thickness(0);
		context->drawBoxElement(baseStyle, nullptr, &borderStyle, nullptr);
	};

	TestEnv::updateFrame();
	TestEnv::updateFrame();
	TestEnv::updateFrame();
	ASSERT_SCREEN_S(LN_ASSETFILE("Rendering/Result/Test_Rendering_ShapeRenderer-BorderOnly-1.png"));
	LN_TEST_CLEAN_SCENE;
}


