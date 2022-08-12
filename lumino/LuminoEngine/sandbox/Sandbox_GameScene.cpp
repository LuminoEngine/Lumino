
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/Controls/UIComboBox.hpp>
#include <LuminoEngine/Scene/MeshVoxelmap/MeshVoxelset.hpp>
#include <LuminoEngine/Scene/MeshVoxelmap/MeshVoxelmapLayer.hpp>
#include <LuminoEngine/Scene/MeshVoxelmap/MeshVoxelmapModel.hpp>
#include <LuminoEngine/Scene/MeshVoxelmap/MeshVoxelmapComponent.hpp>
#include <LuminoEngine/PostEffect/TransitionPostEffect.hpp>
using namespace ln;

TransitionPostEffect* g_TransitionPostEffect;

class TitleScene : public Level
{
	void onStart() override
	{
		//auto sprite = Sprite::create(Texture2D::load(_TT("picture1")));

		Debug::print(_TT("TitleScene"));
		Engine::renderView()->setBackgroundColor(Color::White);
		//g_TransitionPostEffect->start(5);
		Scene::setTransitionEffectMode(LevelTransitionEffectMode::CrossFade);
		Scene::setTransitionEffectColor(Color::Red);
		Scene::setTransitionEffectMaskTexture(Texture2D::load(_TT("Transition1")));
		//Scene::setTransitionEffectVague(0);
		//Scene::setTransitionEffectMaskTexture(Texture2D::load(_TT("Transition2"));

		//auto button = UIButton::create(_TT("button");
	}

	void onUpdate() override;
};

class GameScene : public Level
{
	void onStart() override
	{
		Debug::print(_TT("GameScene"));
		Engine::renderView()->setBackgroundColor(Color::Gray);
		//g_TransitionPostEffect->startFadeOut(5);
	}

	void onUpdate() override
	{
		if (!Scene::isTransitionEffectRunning() && Input::isTriggered(_TT("submit"))) {
			Scene::gotoLevel(makeObject_deprecated<TitleScene>());
		}
	}
};

void TitleScene::onUpdate()
{
	if (!Scene::isTransitionEffectRunning() && Input::isTriggered(_TT("submit"))) {
		Scene::gotoLevel(makeObject_deprecated<GameScene>());
	}
	if (Input::isTriggered(_TT("left"))) {
		Scene::startFadeOut();
	}
	if (Input::isTriggered(_TT("right"))) {
		Scene::startFadeIn();
	}
}

class App_Sandbox_GameScene : public Application
{
	void onInit() override
	{
		Engine::renderView()->setGuideGridEnabled(true);
		Engine::mainCamera()->addComponent(CameraOrbitControlComponent::create());


		//auto transitionPostEffect = TransitionPostEffect::create();
	 //   Engine::renderView()->addPostEffect(transitionPostEffect);
		//g_TransitionPostEffect = transitionPostEffect;

		Scene::gotoLevel(makeObject_deprecated<TitleScene>());
	}

	void onUpdate() override
	{
	}
};

void Sandbox_GameScene()
{
	App_Sandbox_GameScene app;
	//EngineSettings::setFrameRate(10);
    detail::ApplicationHelper::run(&app);
}




