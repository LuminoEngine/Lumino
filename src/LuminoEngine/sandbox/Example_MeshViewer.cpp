
#include <LuminoEngine.hpp>
#include <LuminoEngine/Scene/SkinnedMesh.hpp>
using namespace ln;

class App_Example_MeshViewer : public Application
{

    virtual void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        Engine::camera()->addComponent(CameraOrbitControlComponent::create());



		//auto mesh = StaticMesh::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/sandbox/Assets/autotile-regions.glb");
        auto mesh = SkinnedMesh::load(u"C:/Proj/LN/Lumino/src/LuminoEngine/sandbox/Assets/SkinnedMesh1.glb");
        mesh->setShadingModel(ShadingModel::Unlit);
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




