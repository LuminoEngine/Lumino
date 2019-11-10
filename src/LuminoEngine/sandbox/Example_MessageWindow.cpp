
#include <LuminoEngine.hpp>
#include <LuminoCore/Testing/TestHelper.hpp>
#include <LuminoEngine/UI/UIButton.hpp>
#include <LuminoEngine/UI/UIScrollView.hpp>
#include <LuminoEngine/UI/UIItemsModel.hpp>
#include <LuminoEngine/UI/UIItemsElement.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
using namespace ln;

class App_Example_MessageWindow : public Application
{
    Ref<UIButton> m_button1;

    virtual void onCreate()
    {
        Engine::mainCamera()->setBackgroundColor(Color::Gray);
    }
};

void Example_MessageWindow()
{
    App_Example_MessageWindow app;
	detail::ApplicationHelper::init(&app);
	detail::ApplicationHelper::run(&app);
}




