
#include <LuminoEngine.hpp>
using namespace ln;

class App_Example_MeshViewer : public Application
{
    Ref<PlaneMesh> m_plane;
    Ref<Sprite> m_sprite;

    virtual void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        Engine::camera()->addComponent(CameraOrbitControlComponent::create());
        Engine::renderView()->setBackgroundColor(Color::Gray);

        //m_sprite = Sprite::create(Texture2D::whiteTexture());
        //m_sprite->setBlendMode(BlendMode::Normal);
        //m_sprite->setAlignments(HAlignment::Left, VAlignment::Top);

        //m_plane = PlaneMesh::create();
        //auto planeMaterial = Material::create(Texture2D::load(u"D:/Materials/KitBash3D/WARZONE/Blender/KB3D_Debris_Diffuse.jpg"));
        //m_plane->planeMeshComponent()->setMaterial(planeMaterial);

        //auto sprite2 = Sprite::create(Texture2D::whiteTexture());

        //m_plane = PlaneMesh::create();
        //m_plane->planeMeshComponent()->setSize(10000, 10000);
        //auto texture = Texture2D::load(u"D:/Materials/KitBash3D/WARZONE/Blender/KB3D_Debris_Diffuse.jpg");
        //texture->setMipmapEnabled(true);
        //auto planeMaterial = Material::create(texture);
        //auto normalMap = Texture2D::load(u"D:/Materials/KitBash3D/WARZONE/Blender/KB3D_Debris_Normal.jpg");
        //normalMap->setMipmapEnabled(true);
        ////planeMaterial->setNormalMap(normalMap);
        //m_plane->planeMeshComponent()->setMaterial(planeMaterial);
        ////m_plane->planeMeshComponent()->setUVParUnit(Vector2(0.5f, 0.5f));
        //m_plane->planeMeshComponent()->setUVParUnit(Vector2(5, 5));

        //auto mesh = StaticMesh::create(u"D:/Materials/KitBash3D/WARZONE/Blender/untitled4.glb");


        //auto mesh = StaticMesh::create(u"D:/Tech/Graphics/glTF-Sample-Models/2.0/2CylinderEngine/glTF-Binary/2CylinderEngine.glb");
        
		//auto mesh = StaticMesh::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/sandbox/Assets/autotile-regions.glb");
        //auto mesh = SkinnedMesh::load(u"C:/Proj/LN/Lumino/src/LuminoEngine/sandbox/Assets/SkinnedMesh2.glb");
        //auto mesh = SkinnedMesh::load(u"D:/LocalProj/UnityChanSD/Misaki_sum_humanoid.fbx");
        //auto mesh = StaticMesh::create(u"D:/Materials/VRM/Alicia_VRM/Alicia/VRM/AliciaSolid.glb");
        //auto mesh = StaticMesh::create(u"D:/Materials/VRM/PronamaChan/PronamaChan.glb");

        auto mesh = SkinnedMesh::load(u"D:/Tech/Graphics/glTF-Sample-Models/2.0/AnimatedCube/glTF/AnimatedCube.gltf");
        //auto mesh = SkinnedMesh::load(u"D:/Tech/Graphics/glTF-Sample-Models/2.0/AnimatedMorphCube/glTF/AnimatedMorphCube.gltf");
        //auto mesh = SkinnedMesh::load(u"D:/Tech/Graphics/glTF-Sample-Models/2.0/Monster/glTF/Monster.gltf");
        
        mesh->skinnedMeshComponent()->model()->animationController()->play(u"animation_AnimatedCube");

        //mesh->setShadingModel(ShadingModel::Unlit);

        //auto model = mesh->skinnedMeshComponent()->model();
        //auto skeleton = model->skeleton(0);
        //auto node = model->meshNodes()[3];
        //node->setRotation(0, 0, 0.3);

        //auto li = SpotLight::create();
        //li->setPosition(0, 0.1, 0);
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




