
#include <LuminoEngine.hpp>
#include <LuminoEngine/Tilemap/Voxel.hpp>
using namespace ln;

class App_Experiment_Editor : public Application
{
	Ref<TransformControls> m_transformControls;

    virtual void onInit() override
    {
		Engine::renderView()->setGuideGridEnabled(true);
		Engine::camera()->addComponent(CameraOrbitControlComponent::create());

		auto box = BoxMesh::create();

		m_transformControls = Engine::renderView()->transformControls();
		m_transformControls->setTarget(box);
	}

    virtual void onUpdate() override
    {
    }
};

void Experiment_Editor()
{
	App_Experiment_Editor app;
	detail::ApplicationHelper::run(&app);
}




