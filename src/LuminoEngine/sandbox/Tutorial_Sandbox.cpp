
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/UIComboBox.hpp>
using namespace ln;

#include <Lumino.hpp>


class App : public Application
{
	Ref<SphereMesh> s;

	virtual void onInit() override
	{
		Engine::renderView()->setGuideGridEnabled(true);
		Engine::camera()->addComponent(CameraOrbitControlComponent::create());

		auto mesh = StaticMesh::create(u"D:/Tech/Graphics/glTF-Sample-Models/2.0/Box/glTF/Box.gltf");
	}

	void onUpdate() override
	{
	}
};

void Tutorial_Sandbox()
{
    App app;
	//EngineSettings::setGraphicsDebugEnabled(true);
 //   EngineSettings::setDebugToolEnabled(false);
	//EngineSettings::setMainWindowSize(1600, 800);
    detail::ApplicationHelper::run(&app);
}




