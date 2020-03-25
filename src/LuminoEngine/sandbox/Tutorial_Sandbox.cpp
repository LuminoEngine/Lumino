
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
		Debug::print(u"TitleScene");
		Engine::renderView()->setBackgroundColor(Color::White);
	}

	void onUpdate() override;
};

class GameScene : public Scene
{
	void onStart() override
	{
		Debug::print(u"GameScene");
		Engine::renderView()->setBackgroundColor(Color::Gray);
	}

	void onUpdate() override
	{
		if (Input::triggered(u"submit")) {
			Engine::world()->gotoScene(makeObject<TitleScene>());
		}
	}
};

void TitleScene::onUpdate()
{
	if (Input::triggered(u"submit")) {
		Engine::world()->gotoScene(makeObject<GameScene>());
	}
}

class App : public Application
{
	void onInit() override
	{
		Engine::renderView()->setGuideGridEnabled(true);
		Engine::camera()->addComponent(CameraOrbitControlComponent::create());

		Engine::world()->gotoScene(makeObject<TitleScene>());
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




