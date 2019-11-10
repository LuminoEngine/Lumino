
#include <LuminoEngine.hpp>
#include <LuminoCore/Testing/TestHelper.hpp>
#include <LuminoEngine/UI/UIButton.hpp>
#include <LuminoEngine/UI/UIScrollView.hpp>
#include <LuminoEngine/UI/UIItemsModel.hpp>
#include <LuminoEngine/UI/UIItemsElement.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIFlexMessageTextArea.hpp>
using namespace ln;

class App_Example_MessageWindow : public Application
{
    Ref<UIButton> m_button1;

    virtual void onInit() override
    {
        Engine::mainCamera()->setBackgroundColor(Color::Gray);

        auto windowSkin = Assets::loadTexture(u"Window1");

        auto window1 = UIWindow::create();
        window1->setPosition(10, 10);
        window1->setWidth(200);
        window1->setHeight(100);
        window1->setBackgroundImage(windowSkin);
        window1->setBackgroundImageRect(Rect(0, 0, 48, 48));
        window1->setBackgroundImageBorder(Thickness(8));
        window1->setBackgroundDrawMode(BrushImageDrawMode::BoxFrame);
        Engine::mainUIView()->addElement(window1);

        auto message1 = UIMessageTextArea::create();
        window1->addElement(message1);

    }
};

void Example_MessageWindow()
{
    App_Example_MessageWindow app;
	detail::ApplicationHelper::init(&app);
	detail::ApplicationHelper::run(&app);
}




