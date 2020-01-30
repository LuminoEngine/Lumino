
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/UIComboBox.hpp>
using namespace ln;

class App : public Application
{
};

void Tutorial_Sandbox()
{
    App app;
	detail::ApplicationHelper::init(&app);
	detail::ApplicationHelper::run(&app);
}




