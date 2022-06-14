#include "Common.hpp"
#include <LuminoEngine/UI/UIRenderingContext.hpp>

//==============================================================================
//# Test_Rendering_BoxElement
class Test_Rendering_BoxElement : public LuminoSceneTest
{
public:
	class TestElement : public UIElement
	{
	public:
		std::function<void(UIRenderingContext* context)> render;

		virtual void onRender(UIRenderingContext* context) override { render(context); }
	};

};

#ifdef LN_BOX_ELEMENT_RENDER_FEATURE_TEST

//------------------------------------------------------------------------------
//## Basic
TEST_F(Test_Rendering_BoxElement, Basic)
{

    auto element1 = makeObject<TestElement>();
    element1->setAlignments(UIAlignment::Stretch);
	Engine::mainUIView()->addChild(element1);

	element1->render = [](UIRenderingContext* context)
    {
//		BoxElementShapeBaseStyle baseStyle;
//        BoxElementShapeBorderStyle borderStyle;
//        borderStyle.borderLeftColor = Color::Red;
//        borderStyle.borderTopColor = Color::Green;
//        borderStyle.borderRightColor = Color::Blue;
//        borderStyle.borderBottomColor = Color::Yellow;
//		BoxElementShapeShadowStyle shadowStyle;
//		shadowStyle.shadowInset = false;
///*
//		baseStyle.baseRect = Rect(30, 30, 60, 40);
//		baseStyle.cornerRadius = CornerRadius(0);
//		shadowStyle.shadowWidth = 20;
//		shadowStyle.shadowBlur = 10;
//		shadowStyle.shadowColor = Color::Red;
//		context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);*/
//
//        baseStyle.baseRect = Rect(20, 20, 80, 40);
//        baseStyle.cornerRadius = CornerRadius(10);
//        borderStyle.borderThickness = Thickness(0, 10, 0, 10);
//        borderStyle.borderInset = false;
//        context->drawBoxElement(baseStyle, nullptr, &borderStyle, nullptr);


        BoxElementShapeBaseStyle baseStyle;
        baseStyle.baseRect = Rect(20, 20, 80, 40);
        baseStyle.cornerRadius = CornerRadius(4);

        BoxElementShapeBackgroundStyle backgroundStyle;
        backgroundStyle.color = Color::White;

        BoxElementShapeShadowStyle shadowStyle;
        shadowStyle.shadowColor = Color::Black.withAlpha(0.5f);
        shadowStyle.shadowWidth = 0;
        shadowStyle.shadowBlur = 2;
        shadowStyle.shadowOffset = Vector2(0, 2);

        context->drawBoxElement(baseStyle, &backgroundStyle, nullptr, &shadowStyle);
	};

    TestEnv::updateFrame();
    ASSERT_SCREEN(LN_ASSETFILE("Rendering/Expects/ShapeRenderer-Basic-1.png"));
    LN_TEST_CLEAN_SCENE;
}

TEST_F(Test_Rendering_BoxElement, Background)
{
	auto element1 = makeObject<TestElement>();
	element1->setAlignments(UIAlignment::Stretch);
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
	ASSERT_SCREEN(LN_ASSETFILE("Rendering/Expects/ShapeRenderer-Background-1.png"));
	LN_TEST_CLEAN_SCENE;
}

TEST_F(Test_Rendering_BoxElement, Border)
{
	auto element1 = makeObject<TestElement>();
	element1->setAlignments(UIAlignment::Stretch);
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
        ASSERT_SCREEN(LN_ASSETFILE("Rendering/Expects/ShapeRenderer-Border-1.png"));
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
        ASSERT_SCREEN(LN_ASSETFILE("Rendering/Expects/ShapeRenderer-Border-2.png"));
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
        ASSERT_SCREEN(LN_ASSETFILE("Rendering/Expects/ShapeRenderer-Border-3.png"));
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
        ASSERT_SCREEN(LN_ASSETFILE("Rendering/Expects/ShapeRenderer-Border-4.png"));
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

			// Note: Box のサイズは、Border の合計サイズよりも小さくすることはできない。(Style で width: 0px とかはできるが、表示時に調整される)
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
        ASSERT_SCREEN(LN_ASSETFILE("Rendering/Expects/ShapeRenderer-Border-5.png"));
	}

	LN_TEST_CLEAN_SCENE;
}

TEST_F(Test_Rendering_BoxElement, ShadowOutset)
{
    auto element1 = makeObject<TestElement>();
    element1->setAlignments(UIAlignment::Stretch);
    Engine::mainUIView()->addChild(element1);

    // outset
    {
        element1->render = [](UIRenderingContext* context)
        {
            BoxElementShapeBaseStyle baseStyle;
            BoxElementShapeShadowStyle shadowStyle;
            shadowStyle.shadowInset = false;

            baseStyle.baseRect = Rect(20, 20, 40, 20);
            baseStyle.cornerRadius = CornerRadius(0);
            shadowStyle.shadowWidth = 10;
            shadowStyle.shadowBlur = 0;
            shadowStyle.shadowColor = Color::Red;
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

            baseStyle.baseRect = Rect(100, 20, 40, 20);
            baseStyle.cornerRadius = CornerRadius(0);
            shadowStyle.shadowWidth = 10;
            shadowStyle.shadowBlur = 5;
            shadowStyle.shadowColor = Color::Green;
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

            // equals blur area and slape area
            baseStyle.baseRect = Rect(20, 80, 40, 20);
            baseStyle.cornerRadius = CornerRadius(0);
            shadowStyle.shadowWidth = 10;
            shadowStyle.shadowBlur = 20;
            shadowStyle.shadowColor = Color::Blue;
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

            baseStyle.baseRect = Rect(100, 80, 40, 20);
            baseStyle.cornerRadius = CornerRadius(0);
            shadowStyle.shadowWidth = 0;
            shadowStyle.shadowBlur = 20;
            shadowStyle.shadowColor = Color::Yellow;
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);
        };

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Rendering/Expects/BoxElement-ShadowOutset-1.png"));
    }

    // outset-corner
    {
        element1->render = [](UIRenderingContext* context)
        {
            BoxElementShapeBaseStyle baseStyle;
            BoxElementShapeShadowStyle shadowStyle;
            shadowStyle.shadowInset = false;

            // non-blur
            baseStyle.baseRect = Rect(20, 20, 40, 20);
            baseStyle.cornerRadius = CornerRadius(10);
            shadowStyle.shadowWidth = 10;
            shadowStyle.shadowBlur = 0;
            shadowStyle.shadowColor = Color::Red;
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

            // mini-blur & big-radius
            // サイズより大きい CornerRadius を指定しても、サイズ内に収まるように比率を考慮して縮小する
            baseStyle.baseRect = Rect(100, 20, 40, 20);
            baseStyle.cornerRadius = CornerRadius(130);
            shadowStyle.shadowWidth = 10;
            shadowStyle.shadowBlur = 10;
            shadowStyle.shadowColor = Color::Green;
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

            // fit-blur
            baseStyle.baseRect = Rect(20, 80, 40, 20);
            baseStyle.cornerRadius = CornerRadius(10);
            shadowStyle.shadowWidth = 10;
            shadowStyle.shadowBlur = 20;
            shadowStyle.shadowColor = Color::Blue;
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

            // big-blur
            baseStyle.baseRect = Rect(100, 80, 40, 20);
            baseStyle.cornerRadius = CornerRadius(10);
            shadowStyle.shadowWidth = 5;
            shadowStyle.shadowBlur = 30;
            shadowStyle.shadowColor = Color::Yellow;
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);
        };

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Rendering/Expects/BoxElement-ShadowOutset-2.png"));
    }

    LN_TEST_CLEAN_SCENE;
}

TEST_F(Test_Rendering_BoxElement, OutsetShadowOffset)
{
    auto element1 = makeObject<TestElement>();
    element1->setAlignments(UIAlignment::Stretch);
    Engine::mainUIView()->addChild(element1);

    // FIXME: ちょっと上部の境界が怪しいが現状とする。小さければ問題は少ない。
    // もし解決するなら、CornerRadius 無しの特別対応が必要。少しでも CornerRadius があるときれいに出る。
    {
        element1->render = [](UIRenderingContext* context)
        {
            BoxElementShapeBaseStyle baseStyle;
            BoxElementShapeShadowStyle shadowStyle;
            shadowStyle.shadowInset = false;

            baseStyle.baseRect = Rect(40, 40, 80, 40);
            baseStyle.cornerRadius = CornerRadius(0);
            shadowStyle.shadowWidth = 10;
            shadowStyle.shadowBlur = 10;
            shadowStyle.shadowColor = Color::Red;
            shadowStyle.shadowOffset = Vector2(0, 10);
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);
        };

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Rendering/Expects/BoxElement-OutsetShadowOffset-1.png"));
    }

    // CornerRadius
    {
        element1->render = [](UIRenderingContext* context)
        {
            BoxElementShapeBaseStyle baseStyle;
            BoxElementShapeShadowStyle shadowStyle;
            shadowStyle.shadowInset = false;

            baseStyle.baseRect = Rect(40, 40, 80, 40);
            baseStyle.cornerRadius = CornerRadius(20);
            shadowStyle.shadowWidth = 10;
            shadowStyle.shadowBlur = 10;
            shadowStyle.shadowColor = Color::Green;
            shadowStyle.shadowOffset = Vector2(0, 10);
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);
        };

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Rendering/Expects/BoxElement-OutsetShadowOffset-2.png"));
    }

    // Width=0 で縮退する影と offset. Material-UI の Button.
    {
        element1->render = [](UIRenderingContext* context)
        {
            BoxElementShapeBaseStyle baseStyle;
            BoxElementShapeShadowStyle shadowStyle;
            shadowStyle.shadowInset = false;

            baseStyle.baseRect = Rect(40, 40, 80, 40);
            baseStyle.cornerRadius = CornerRadius(10);
            shadowStyle.shadowWidth = 0;
            shadowStyle.shadowBlur = 40;
            shadowStyle.shadowColor = Color::Blue;
            shadowStyle.shadowOffset = Vector2(0, 10);
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);
        };

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Rendering/Expects/BoxElement-OutsetShadowOffset-3.png"));
    }

    // 縮退 Shadow + 境界が消えるまでオフセットをかける
    {
        element1->render = [](UIRenderingContext* context)
        {
            BoxElementShapeBaseStyle baseStyle;
            BoxElementShapeShadowStyle shadowStyle;
            shadowStyle.shadowInset = false;

            baseStyle.baseRect = Rect(40, 40, 80, 40);
            baseStyle.cornerRadius = CornerRadius(10);
            shadowStyle.shadowWidth = 0;
            shadowStyle.shadowBlur = 40;
            shadowStyle.shadowColor = Color::Yellow;
            shadowStyle.shadowOffset = Vector2(0, 20);
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);
        };

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Rendering/Expects/BoxElement-OutsetShadowOffset-4.png"));
    }

    LN_TEST_CLEAN_SCENE;
}

TEST_F(Test_Rendering_BoxElement, ShadowInset)
{
    auto element1 = makeObject<TestElement>();
    element1->setAlignments(UIAlignment::Stretch);
    Engine::mainUIView()->addChild(element1);

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

            // (blur/2) is less than width
            baseStyle.baseRect = Rect(80, 0, 80, 60);
            baseStyle.cornerRadius = CornerRadius(0);
            shadowStyle.shadowWidth = 10;
            shadowStyle.shadowBlur = 10;
            shadowStyle.shadowColor = Color::Green;
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

            // ぼかしと ShapeInner の境界が一致する場合
            baseStyle.baseRect = Rect(0, 60, 80, 60);
            baseStyle.cornerRadius = CornerRadius(0);
            shadowStyle.shadowWidth = 10;
            shadowStyle.shadowBlur = 20;
            shadowStyle.shadowColor = Color::Blue;
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

            // (blur/2) is greater than width
            baseStyle.baseRect = Rect(80, 60, 80, 60);
            baseStyle.cornerRadius = CornerRadius(0);
            shadowStyle.shadowWidth = 10;
            shadowStyle.shadowBlur = 30;
            shadowStyle.shadowColor = Color::Yellow;
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);
        };

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Rendering/Expects/BoxElement-ShadowInset-1.png"));
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
            shadowStyle.shadowWidth = 5;
            shadowStyle.shadowBlur = 0;
            shadowStyle.shadowColor = Color::Red;
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

            baseStyle.baseRect = Rect(80, 0, 80, 60);
            baseStyle.cornerRadius = CornerRadius(20);
            shadowStyle.shadowWidth = 5;
            shadowStyle.shadowBlur = 5;
            shadowStyle.shadowColor = Color::Green;
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

            baseStyle.baseRect = Rect(0, 60, 80, 60);
            baseStyle.cornerRadius = CornerRadius(20);
            shadowStyle.shadowWidth = 5;
            shadowStyle.shadowBlur = 10;
            shadowStyle.shadowColor = Color::Blue;
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

            baseStyle.baseRect = Rect(80, 60, 80, 60);
            baseStyle.cornerRadius = CornerRadius(20);
            shadowStyle.shadowWidth = 5;
            shadowStyle.shadowBlur = 20;
            shadowStyle.shadowColor = Color::Yellow;
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);
        };

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Rendering/Expects/BoxElement-ShadowInset-2.png"));
    }


    // inset width 0
    {
        element1->render = [](UIRenderingContext* context)
        {
            BoxElementShapeBaseStyle baseStyle;
            BoxElementShapeShadowStyle shadowStyle;
            shadowStyle.shadowInset = true;

            baseStyle.baseRect = Rect(0, 0, 80, 60);
            baseStyle.cornerRadius = CornerRadius(0);
            shadowStyle.shadowWidth = 0;
            shadowStyle.shadowBlur = 10;
            shadowStyle.shadowColor = Color::Red;
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

            baseStyle.baseRect = Rect(80, 0, 80, 60);
            baseStyle.cornerRadius = CornerRadius(40);
            shadowStyle.shadowWidth = 0;
            shadowStyle.shadowBlur = 20;
            shadowStyle.shadowColor = Color::Green;
            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);
        };

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Rendering/Expects/BoxElement-ShadowInset-3.png"));
    }

    LN_TEST_CLEAN_SCENE;
}

//TEST_F(Test_Rendering_BoxElement, ShadowIllegals)
//{
//    auto element1 = makeObject<TestElement>();
//    element1->setHAlignment(UIHAlignment::Stretch);
//    element1->setVAlignment(UIVAlignment::Stretch);
//    Engine::mainUIView()->addChild(element1);
//
//    // 0 size
//    {
//        element1->render = [](UIRenderingContext* context)
//        {
//            BoxElementShapeBaseStyle baseStyle;
//            BoxElementShapeShadowStyle shadowStyle;
//
//            baseStyle.baseRect = Rect(10, 10, 0, 0);
//            shadowStyle.shadowWidth = 10;
//            shadowStyle.shadowBlur = 5;
//            shadowStyle.shadowColor = Color::Red;
//            shadowStyle.shadowInset = false;
//            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);
//
//            baseStyle.baseRect = Rect(70, 10, 0, 0);
//            baseStyle.cornerRadius = CornerRadius(5);
//            shadowStyle.shadowWidth = 10;
//            shadowStyle.shadowBlur = 5;
//            shadowStyle.shadowColor = Color::Blue;
//            shadowStyle.shadowInset = false;
//            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);
//
//            // Size 0 だと inset は描画されない
//            baseStyle.baseRect = Rect(0, 60, 0, 0);
//            shadowStyle.shadowWidth = 10;
//            shadowStyle.shadowBlur = 5;
//            shadowStyle.shadowColor = Color::Green;
//            shadowStyle.shadowInset = true;
//            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);
//
//            baseStyle.baseRect = Rect(80, 60, 20, 20);
//            baseStyle.cornerRadius = CornerRadius(10);
//            shadowStyle.shadowWidth = 10;
//            shadowStyle.shadowBlur = 5;
//            shadowStyle.shadowColor = Color::Yellow;
//            shadowStyle.shadowInset = true;
//            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);
//        };
//
//        TestEnv::updateFrame();
//        ASSERT_SCREEN(LN_ASSETFILE("Rendering/Expects/BoxElement-ShadowIllegals-1.png"));
//    }
//
//    LN_TEST_CLEAN_SCENE;
//}
//
//TEST_F(Test_Rendering_BoxElement, ShadowOffset)
//{
//    auto element1 = makeObject<TestElement>();
//    element1->setHAlignment(UIHAlignment::Stretch);
//    element1->setVAlignment(UIVAlignment::Stretch);
//    Engine::mainUIView()->addChild(element1);
//
//    // offset
//    {
//        element1->render = [](UIRenderingContext* context)
//        {
//            BoxElementShapeBaseStyle baseStyle;
//            BoxElementShapeShadowStyle shadowStyle;
//
//            //baseStyle.baseRect = Rect(20, 20, 20, 20);
//            //baseStyle.cornerRadius = CornerRadius(5);
//            //shadowStyle.shadowWidth = 20;
//            //shadowStyle.shadowBlur = 10;
//            //shadowStyle.shadowColor = Color::Red;
//            //shadowStyle.shadowOffset = Vector2(5, 0);
//            //shadowStyle.shadowInset = false;
//            //context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);
//
//            baseStyle.baseRect = Rect(100, 20, 20, 20);
//            baseStyle.cornerRadius = CornerRadius(0);
//            shadowStyle.shadowWidth = 20;
//            shadowStyle.shadowBlur = 10;
//            shadowStyle.shadowColor = Color::Green;
//            shadowStyle.shadowOffset = Vector2(0, 10);
//            shadowStyle.shadowInset = false;
//            context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);
//
//            //baseStyle.baseRect = Rect(20, 70, 40, 40);
//            //baseStyle.cornerRadius = CornerRadius(5);
//            //shadowStyle.shadowWidth = 20;
//            //shadowStyle.shadowBlur = 10;
//            //shadowStyle.shadowColor = Color::Red;
//            //shadowStyle.shadowOffset = Vector2(5, 0);
//            //shadowStyle.shadowInset = true;
//            //context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);
//
//            //baseStyle.baseRect = Rect(100, 70, 40, 40);
//            //baseStyle.cornerRadius = CornerRadius(5);
//            //shadowStyle.shadowWidth = 20;
//            //shadowStyle.shadowBlur = 10;
//            //shadowStyle.shadowColor = Color::Green;
//            //shadowStyle.shadowOffset = Vector2(15, 0);
//            //shadowStyle.shadowInset = true;
//            //context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);
//        };
//
//        TestEnv::updateFrame();
//        ASSERT_SCREEN_S(LN_ASSETFILE("Rendering/Expects/BoxElement-ShadowOffset-1.png"));
//    }
//
//    LN_TEST_CLEAN_SCENE;
//}

TEST_F(Test_Rendering_BoxElement, BorderIlls)
{
	auto element1 = makeObject<TestElement>();
	element1->setAlignments(UIAlignment::Stretch);
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
	ASSERT_SCREEN(LN_ASSETFILE("Rendering/Expects/BoxElement-BorderIlls-1.png"));
	LN_TEST_CLEAN_SCENE;
}


#endif
