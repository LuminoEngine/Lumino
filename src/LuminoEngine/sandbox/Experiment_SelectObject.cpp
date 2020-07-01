
#include <LuminoEngine.hpp>
#include <LuminoEngine/Tilemap/Voxel.hpp>
using namespace ln;


class App_Experiment_SelectObject : public Application
{
    void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        Engine::camera()->addComponent(CameraOrbitControlComponent::create());

        auto box = BoxMesh::create();
	}

    void onUpdate() override
    {
    }
};

void Experiment_SelectObject()
{
	App_Experiment_SelectObject app;
	detail::ApplicationHelper::run(&app);
}




