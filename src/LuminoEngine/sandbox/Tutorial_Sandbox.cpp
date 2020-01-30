
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/UIComboBox.hpp>
using namespace ln;

class App : public Application
{
	virtual void onInit() override
	{
		Debug::print(u"Hello, Lumino! 1", 1.0f);
		Debug::print(u"Hello, Lumino! 2", 2.0f);
		Debug::print(u"Hello, Lumino! 3", 1.0f);
		Debug::print(u"Hello, Lumino! 2", 2.5f);
		Debug::print(u"Hello, Lumino! 2", 2.0f);

		auto t = UITextBlock::create(u"Hello, Lumino!");
		//Engine::mainWindow()->addElement(t);

	}

	virtual void onUpdate() override
	{
		Debug::print(u"Hello, Lumino! 1", 0);
		Debug::print(u"Hello, Lumino! 2", 0);
		Debug::print(u"Hello, Lumino! 3", 0);
	}
};

void Tutorial_Sandbox()
{
    App app;
	detail::ApplicationHelper::run(&app);
}




