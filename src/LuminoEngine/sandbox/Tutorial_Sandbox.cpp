
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/UIComboBox.hpp>
#include <LuminoEngine/Scene/MeshTilemap/MeshTileset.hpp>
#include <LuminoEngine/Scene/MeshTilemap/MeshTilemapLayer.hpp>
#include <LuminoEngine/Scene/MeshTilemap/MeshTilemapModel.hpp>
#include <LuminoEngine/Scene/MeshTilemap/MeshTilemapComponent.hpp>
using namespace ln;



class TitleScene : public Scene
{
	void onStart() override
	{

	}

	void onUpdate() override
	{

	}
};


class App : public Application
{
	void onInit() override
	{
		Engine::renderView()->setGuideGridEnabled(true);
		Engine::camera()->addComponent(CameraOrbitControlComponent::create());

	}

	void onUpdate() override
	{
	}
};

void Tutorial_Sandbox()
{
    App app;
    detail::ApplicationHelper::run(&app);
}




