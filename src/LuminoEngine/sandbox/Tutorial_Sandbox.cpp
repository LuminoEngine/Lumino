
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/UIComboBox.hpp>
using namespace ln;

class App : public Application
{
	Ref<BoxMesh> box;

	virtual void onInit() override
	{
		Engine::renderView()->setGuideGridEnabled(true);
		Engine::camera()->addComponent(CameraOrbitControlComponent::create());

		box = BoxMesh::create();
		box->setColorScale(Color::Red);
	}

	virtual void onUpdate() override
	{
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




