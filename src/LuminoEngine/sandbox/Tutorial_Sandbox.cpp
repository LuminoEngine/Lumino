
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/UIComboBox.hpp>
using namespace ln;

class App : public Application
{
	virtual void onInit() override
	{
		//Debug::print(u"Hello, Lumino!");

		Engine::mainCamera()->addComponent(makeObject<CameraOrbitControlComponent>());
		Engine::mainRenderView()->setDebugGridEnabled(true);
	}

	virtual void onUpdate() override
	{
		Debug::print(0, String::format(u"X: {0}, Y:{1}", Mouse::position().x, Mouse::position().y));
	}
};

void Tutorial_Sandbox()
{
    App app;
	EngineSettings::setDebugToolEnabled(false);
	detail::ApplicationHelper::run(&app);
}




