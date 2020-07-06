
#include <LuminoEngine.hpp>
using namespace ln;

class App_Example_MeshViewer : public Application
{
    Ref<PlaneMesh> m_plane;
    Ref<Sprite> m_sprite;
    Ref<MeshNode> m_node;
    Ref<SkinnedMesh> m_mesh;
    Ref<SkinnedMeshModel> m_model;
    Ref<AnimationState> m_idle;
    Ref<AnimationState> m_walk;

    Vector3 m_targetPos;
    Vector3 m_lookPos;

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
        //auto mesh = StaticMesh::create("D:/Materials/KitBash3D/WARZONE/ResolvedTextureReference/Exports/BldgMedTallC_grp.gltf");
        //auto mesh = StaticMesh::create("D:/Materials/KitBash3D/WARZONE/ResolvedTextureReference/Exports/2-OverpassBroken_grp.gltf");
        auto mesh = StaticMesh::create(u"D:/Materials/KitBash3D/WARZONE/ResolvedTextureReference/Exports/27-BldgRubbleB_grp.gltf");
        
        //mesh->setPosition(0, 10, 0);
#if 0
    
        m_mesh = SkinnedMesh::load(u"D:/Materials/MMD/Appearance Miku/Appearance Miku_BDEF.pmx");
        m_model = m_mesh->skinnedMeshComponent()->model();
        m_node = m_model->findNode(u"左腕");
        m_node->setRotation(0, 0, Math::PI / 8);

        m_mesh->setShadingModel(ShadingModel::Unlit);



        //auto clip = AnimationClip::load(u"D:/Materials/MMD/Motion/■配布用（モーション）/歩き/歩行（歩幅5・直進）.vmd");
        auto clip1 = AnimationClip::load(u"D:/Materials/MMD/Motion/MMO用stand/stand2.vmd");
        auto clip2 = AnimationClip::load(u"D:/Materials/MMD/Motion/走歩スv2.2full/歩く/A01_SO_女の子歩き_s591_p40.vmd");
        
        m_idle = m_model->animationController()->addClip(clip1);
        m_walk = m_model->animationController()->addClip(clip2);
        m_model->animationController()->play(m_idle);
#endif
    }

    virtual void onUpdate() override
    {
#if 0
        //m_node->setRotation(0, 0, Engine::time());
        //m_node->setRotation(Quaternion(0.00719260797, -0.0159967896, -0.317227066, 0.948187351));

        float velocity = 0.15;

        auto pos = m_mesh->position();

        //if (Input::pressed(u"left")) {
        //    pos.x -= 0.01;
        //}

        float h = -Input::getAxisValue(u"left") + Input::getAxisValue(u"right");
        float v = -Input::getAxisValue(u"up") + Input::getAxisValue(u"down");
        pos.x += h * velocity;
        pos.z += v * velocity;

        if (h != 0 || v != 0) {
            auto dir = Vector3::normalize(-h, 0, -v);
            m_mesh->lookAt(pos + dir);
            m_model->animationController()->play(m_walk);


            m_targetPos = pos - dir * 10;
        }
        else {
            m_model->animationController()->play(m_idle);
        }

        m_lookPos += (m_targetPos - m_lookPos) / 60;

        Vector3 lookat = m_lookPos +Vector3(0, 10, 0);
        Engine::camera()->setPosition(lookat + Vector3(0, 15, -30));
        Engine::camera()->lookAt(lookat);

        m_mesh->setPosition(pos);
#endif
    }
};

void Example_MeshViewer()
{
    //Logger::setLevel(LogLevel::Verbose);
	App_Example_MeshViewer app;
	detail::ApplicationHelper::run(&app);
}




