
#include <LuminoEngine.hpp>
using namespace ln;

class App_Example_MeshViewer : public Application
{

    virtual void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        Engine::camera()->addComponent(CameraOrbitControlComponent::create());
        Engine::renderView()->setBackgroundColor(Color::Gray);

        auto plane = PlaneMesh::create();
        auto texture = Texture2D::load(u"D:/Materials/KitBash3D/WARZONE/Blender/KB3D_Debris_Diffuse.jpg");
        texture->setMipmapEnabled(true);
        auto planeMaterial = Material::create(texture);
        planeMaterial->setNormalMap(Texture2D::load(u"D:/Materials/KitBash3D/WARZONE/Blender/KB3D_Debris_Normal.jpg"));
        plane->planeMeshComponent()->setMaterial(planeMaterial);
        plane->planeMeshComponent()->setUVParUnit(Vector2(0.5f, 0.5f));

        auto mesh = StaticMesh::create(u"D:/Materials/KitBash3D/WARZONE/Blender/untitled.glb");


        //auto mesh = StaticMesh::create(u"D:/Tech/Graphics/glTF-Sample-Models/2.0/2CylinderEngine/glTF-Binary/2CylinderEngine.glb");
        
		//auto mesh = StaticMesh::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/sandbox/Assets/autotile-regions.glb");
        //auto mesh = SkinnedMesh::load(u"C:/Proj/LN/Lumino/src/LuminoEngine/sandbox/Assets/SkinnedMesh2.glb");
        //auto mesh = SkinnedMesh::load(u"D:/LocalProj/UnityChanSD/Misaki_sum_humanoid.fbx");
        //mesh->setShadingModel(ShadingModel::Unlit);

        //auto model = mesh->skinnedMeshComponent()->model();
        //auto skeleton = model->skeleton(0);
        //auto node = model->meshNodes()[3];
        //node->setRotation(0, 0, 0.3);
    }

    virtual void onUpdate() override
    {
    }
};

void Example_MeshViewer()
{
    //Logger::setLevel(LogLevel::Verbose);
	App_Example_MeshViewer app;
	detail::ApplicationHelper::run(&app);
}




