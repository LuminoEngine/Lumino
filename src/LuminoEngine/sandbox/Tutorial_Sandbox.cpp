
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/UIComboBox.hpp>
using namespace ln;

class App : public Application
{
	virtual void onInit() override
	{
		//Debug::print(u"Hello, Lumino!");

		//Engine::mainCamera()->addComponent(makeObject<CameraOrbitControlComponent>());
		Engine::mainRenderView()->setDebugGridEnabled(false);
	}

	virtual void onUpdate() override
	{
		Debug::print(0, String::format(u"X: {0}", Mouse::position().x));
		Debug::print(0, String::format(u"Y: {0}", Mouse::position().y));
	}
};

void Tutorial_Sandbox()
{
    App app;
	EngineSettings::setDebugToolEnabled(true);
	detail::ApplicationHelper::run(&app);
}




