
#include <LuminoEngine.hpp>
using namespace ln;

class App_Example_MeshViewer : public Application
{
    Ref<PlaneMesh> m_plane;
    Ref<Sprite> m_sprite;
    Ref<MeshNode> m_node;

    virtual void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        Engine::camera()->addComponent(CameraOrbitControlComponent::create());
        //Engine::renderView()->setBackgroundColor(Color::Gray);

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

        //auto mesh = SkinnedMesh::load(u"D:/Tech/Graphics/glTF-Sample-Models/2.0/AnimatedCube/glTF/AnimatedCube.gltf");
        //auto mesh = SkinnedMesh::load(u"D:/Tech/Graphics/glTF-Sample-Models/2.0/AnimatedMorphCube/glTF/AnimatedMorphCube.gltf");
        //auto mesh = SkinnedMesh::load(u"D:/Tech/Graphics/glTF-Sample-Models/2.0/Monster/glTF/Monster.gltf");
        
        
        //mesh->skinnedMeshComponent()->model()->animationController()->play(u"animation_AnimatedCube");

    
        auto mesh = SkinnedMesh::load(u"D:/Materials/MMD/Appearance Miku/Appearance Miku_BDEF.pmx");
        auto model = mesh->skinnedMeshComponent()->model();
        m_node = model->findNode(u"左腕");
        m_node->setRotation(0, 0, Math::PI / 8);

        mesh->setShadingModel(ShadingModel::Unlit);



        auto clip = AnimationClip::load(u"D:/Materials/MMD/Motion/■配布用（モーション）/歩き/歩行（歩幅5・直進）.vmd");
        auto state = model->animationController()->addClip(clip);
        model->animationController()->play(state);


        //auto model = mesh->skinnedMeshComponent()->model();
        //auto skeleton = model->skeleton(0);
        //auto node = model->meshNodes()[3];
        //node->setRotation(0, 0, 0.3);

        //auto li = SpotLight::create();
        //li->setPosition(0, 0.1, 0);

        auto window = UIWindow::create();
        window->setSize(200, 200);
        window->setAlignments(HAlignment::Left, VAlignment::Top);


    }

    virtual void onUpdate() override
    {
        //m_node->setRotation(0, 0, Engine::time());
        //m_node->setRotation(Quaternion(0.00719260797, -0.0159967896, -0.317227066, 0.948187351));
    }
};

void Example_MeshViewer()
{
    //Logger::setLevel(LogLevel::Verbose);
	App_Example_MeshViewer app;
	detail::ApplicationHelper::run(&app);
}




