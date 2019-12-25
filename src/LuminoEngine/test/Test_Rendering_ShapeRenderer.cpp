#include "Common.hpp"
#include <LuminoEngine/UI/UIRenderingContext.hpp>

//==============================================================================
//# ShapeRenderer
class Test_Rendering_ShapeRenderer : public LuminoSceneTest {};

//------------------------------------------------------------------------------
//## Basic
TEST_F(Test_Rendering_ShapeRenderer, Basic)
{
    class TestElement : public UIElement
    {
    public:
        std::function<void(UIRenderingContext* context)> render;

        virtual void onRender(UIRenderingContext* context) override
        {
            render(context);
        }
    };

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

        BoxElementShapeBorderStyle borderStyle;
        borderStyle.borderThickness = Thickness(5);
        borderStyle.borderLeftColor =
            borderStyle.borderTopColor =
            borderStyle.borderRightColor =
            borderStyle.borderBottomColor = Color::Red;
        borderStyle.borderInset = false;

        context->drawBoxElement(baseStyle, nullptr, &borderStyle, nullptr);
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

