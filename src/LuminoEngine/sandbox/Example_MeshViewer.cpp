
#include <LuminoEngine.hpp>
using namespace ln;

class App_Example_MeshViewer : public Application
{

    virtual void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        Engine::camera()->addComponent(CameraOrbitControlComponent::create());



		auto mesh = StaticMesh::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/sandbox/Assets/autotile-regions.glb");
    }

    virtual void onUpdate() override
    {
    }
};

void Example_MeshViewer()
{
	App_Example_MeshViewer app;
	detail::ApplicationHelper::run(&app);
}




