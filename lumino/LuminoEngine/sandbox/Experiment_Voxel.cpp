
#include <LuminoEngine.hpp>
#include <LuminoEngine/Tilemap/Voxel.hpp>
using namespace ln;

class App_Sandbox_Voxel : public Application
{

    void onInit() override
    {
		Engine::renderView()->setGuideGridEnabled(true);
		Engine::mainCamera()->addComponent(CameraOrbitControlComponent::create());

		auto voxelmap1 = makeObject<VisualObject>();
		auto voxelmapComponent1 = makeObject<VoxelmapComponent>();
		voxelmap1->addComponent(voxelmapComponent1);


		// 組み合わせの数は 2**(8+9+9) = ‭67,108,863‬
	}

    void onUpdate() override
    {
    }
};

void Sandbox_Voxel()
{
	App_Sandbox_Voxel app;
	detail::ApplicationHelper::run(&app);
}




