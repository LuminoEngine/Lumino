
#include <LuminoFramework.hpp>
#include <LuminoFramework/UI/Controls/UIComboBox.hpp>
using namespace ln;

class App_Example_UIControls : public Application
{

    void onInit() override
    {
        //Engine::mainCamera()->setBackgroundColor(Color::Gray);

        auto combobox1 = ln::makeObject_deprecated<UIComboBox>();
        combobox1->setWidth(200);
        //combobox1->setBackgroundColor(Color::BurlyWood);
        Engine::mainUIView()->addElement(combobox1);
    }

    void onUpdate() override
    {
    }
};

void Example_UIControls()
{
    App_Example_UIControls app;
	detail::ApplicationHelper::run(&app);
}




