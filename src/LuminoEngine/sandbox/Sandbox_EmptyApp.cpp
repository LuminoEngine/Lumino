
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/UIComboBox.hpp>
using namespace ln;

class App_Sandbox_EmptyApp : public Application
{
    void onInit() override
    {
    }

    void onUpdate() override
    {
    }
};

void Sandbox_EmptyApp()
{
    App_Sandbox_EmptyApp app;
	detail::ApplicationHelper::run(&app);
}




