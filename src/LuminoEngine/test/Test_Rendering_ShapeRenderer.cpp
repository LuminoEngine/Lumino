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
    element1->setHorizontalAlignment(HAlignment::Center);
    element1->setVerticalAlignment(VAlignment::Center);
    element1->setWidth(80);
    element1->setHeight(60);
	Engine::mainUIView()->addChild(element1);
    //element1->render = [](UIRenderingContext* context)
    //{
    //    context->drawBoxBackground(
    //        Rect(0, 0, 80, 60),
    //        Thickness::Zero,
    //        CornerRadius(),
    //        BrushImageDrawMode::Image,
    //        Rect::Zero,
    //        Color::Red);
    //};

    //TestEnv::updateFrame();
    //TestEnv::updateFrame();
    //TestEnv::updateFrame();
    //ASSERT_SCREEN_S(LN_ASSETFILE("Rendering/Result/Test_Rendering_ShapeRenderer-Basic-1.png"));
    //LN_TEST_CLEAN_SCENE;


    element1->render = [](UIRenderingContext* context)
    {
        BoxElementShapeBaseStyle baseStyle;
        baseStyle.baseRect = Rect(0, 0, 80, 60);
        baseStyle.cornerRadius = CornerRadius(10);

        BoxElementShapeBackgroundStyle backgroundStyle;
        backgroundStyle.color = Color::Red;

		BoxElementShapeShadowStyle shadowStyle;
		shadowStyle.shadowOffset.x = 6;
		shadowStyle.shadowOffset.y = 6;
		shadowStyle.shadowWidth = 10;
		shadowStyle.shadowBlur = 5;
		shadowStyle.shadowInset = true;

        //context->drawBoxElement(baseStyle, &backgroundStyle, nullptr, &shadowStyle);
		context->drawBoxElement(baseStyle, nullptr, nullptr, &shadowStyle);

        //BoxElementShapeBorderStyle borderStyle;
        //borderStyle.borderThickness = Thickness(5);
        //borderStyle.borderLeftColor =
        //    borderStyle.borderTopColor =
        //    borderStyle.borderRightColor =
        //    borderStyle.borderBottomColor = Color::Red;
        //borderStyle.borderInset = false;
        //context->drawBoxElement(baseStyle, nullptr, &borderStyle, nullptr);


        //context->drawBoxBorderLine(
        //    Rect(0, 0, 80, 60),
        //    Thickness(5),
        //    Color::Red, Color::Red, Color::Red, Color::Red,
        //    CornerRadius(),
        //    false);
    };
    TestEnv::updateFrame();
    TestEnv::updateFrame();
    TestEnv::updateFrame();
    ASSERT_SCREEN_S(LN_ASSETFILE("Rendering/Result/Test_Rendering_ShapeRenderer-Basic-2.png"));
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
	ASSERT_SCREEN(LN_ASSETFILE("Rendering/Result/Test_Rendering_ShapeRenderer-BackgroundOnly-1.png"));
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


