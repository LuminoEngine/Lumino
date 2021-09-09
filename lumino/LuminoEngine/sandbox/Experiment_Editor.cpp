
#include <LuminoEngine.hpp>
#include <LuminoEngine/Tilemap/Voxel.hpp>
#include "../../../lumino/LuminoEngine/src/Engine/EngineDomain.hpp"
#include "../../../lumino/LuminoEngine/src/Scene/SceneManager.hpp"
using namespace ln;

class App_Experiment_Editor : public Application
{
public:
	Ref<TransformControls> m_transformControls;

	App_Experiment_Editor()
	{
		EngineSettings::setDevelopmentToolsEnabled(true);
	}

    virtual void onInit() override
    {
		Engine::renderView()->setGuideGridEnabled(true);
		Engine::mainCamera()->addComponent(CameraOrbitControlComponent::create());
		//ln::detail::EngineDomain::sceneManager()->m_editorMode = true;
		Engine::renderView()->setClearMode(SceneClearMode::SkyDome);

		auto box1 = BoxMesh::With().build();
		box1->setPosition(-2, 0, 0);

		auto box2 = BoxMesh::With().build();
		box2->setPosition(2, 0, 0);

		//m_transformControls = Engine::renderView()->transformControls();
		//m_transformControls->setTarget(box);

	}

    virtual void onUpdate() override
    {
		//if (Mouse::triggered(MouseButtons::Left)) {
		//	auto pos = Mouse::position();
		//	Engine::renderView()->findObjectInPoint(pos.x, pos.y);
		//}

		//auto camera = Engine::camera();
		//camera->viewMatrix();
		//auto pp = Vector3::transformCoord(Vector3(10, 0, 0), camera->viewMatrix());
		//printf("");
    }
};

void Experiment_Editor()
{
	App_Experiment_Editor app;
	detail::ApplicationHelper::run(&app);
}




