
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/UIComboBox.hpp>
using namespace ln;

class App : public Application
{
	virtual void onInit() override
	{
		Camera* camera = Engine::camera();
		camera->addComponent(makeObject<CameraOrbitControlComponent>());
		//Engine::camera()->setPosition(10, 10, -10);
		//Engine::camera()->lookAt(0, 0, 0);
		Engine::mainRenderView()->setDebugGridEnabled(true);

		auto box = BoxMesh::create();
		Engine::world()->add(box);
	}

	virtual void onUpdate() override
	{
		//Debug::print(0, String::format(u"X: {0}", Mouse::position().x));
		//Debug::print(0, String::format(u"Y: {0}", Mouse::position().y));
	}
};

void Tutorial_Sandbox()
{
    App app;
	EngineSettings::setDebugToolEnabled(false);
	detail::ApplicationHelper::run(&app);
}




