
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/UIComboBox.hpp>
using namespace ln;

class App : public Application
{
	Ref<BoxMesh> box;

	virtual void onInit() override
	{
		box = BoxMesh::create();

		auto camera = Engine::camera();
		camera->setPosition(5, 5, -5);
		camera->lookAt(0, 0, 0);

		//auto light = Engine::light();
		//light->lookAt(0, -1, 0);
	}

	virtual void onUpdate() override
	{
		//Debug::print(0, String::format(u"X: {0}", Mouse::position().x));
		//Debug::print(0, String::format(u"Y: {0}", Mouse::position().y));

		auto raycaster = Raycaster::fromScreen(Mouse::position());
		if (auto result = raycaster->intersectPlane(0, 1, 0)) {
			box->setPosition(result->point());
		}

	}
};

void Tutorial_Sandbox()
{
    App app;
	EngineSettings::setDebugToolEnabled(false);
	detail::ApplicationHelper::run(&app);
}




