
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/UIComboBox.hpp>
#include <LuminoEngine/Scene/MeshTilemap/MeshTileset.hpp>
#include <LuminoEngine/Scene/MeshTilemap/MeshTilemapLayer.hpp>
#include <LuminoEngine/Scene/MeshTilemap/MeshTilemapModel.hpp>
#include <LuminoEngine/Scene/MeshTilemap/MeshTilemapComponent.hpp>
#include <LuminoEngine/ImageEffect/TransitionImageEffect.hpp>
using namespace ln;

TransitionImageEffect* g_TransitionImageEffect;

class TitleScene : public Scene
{
	void onStart() override
	{
		Debug::print(u"TitleScene");
		Engine::renderView()->setBackgroundColor(Color::White);
		g_TransitionImageEffect->start(5);
	}

	void onUpdate() override;
};

class GameScene : public Scene
{
	void onStart() override
	{
		Debug::print(u"GameScene");
		Engine::renderView()->setBackgroundColor(Color::Gray);
		g_TransitionImageEffect->start(5);
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


		auto transitionImageEffect = TransitionImageEffect::create();
	    Engine::renderView()->addImageEffect(transitionImageEffect);
		g_TransitionImageEffect = transitionImageEffect;

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




