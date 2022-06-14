
#include <LuminoEngine.hpp>
using namespace ln;

class App_Example_MeshViewer : public Application
{
public:
    Ref<PlaneMesh> m_plane;
    Ref<Sprite> m_sprite;
    Ref<MeshNode> m_node;
    Ref<SkinnedMesh> m_mesh;
    Ref<MeshModel> m_model;
    Ref<AnimationState> m_idle;
    Ref<AnimationState> m_walk;

    Vector3 m_targetPos;
    Vector3 m_lookPos;

    App_Example_MeshViewer()
    {
        EngineSettings::setDevelopmentToolsEnabled(true);
    }

    virtual void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        Engine::mainCamera()->addComponent(CameraOrbitControlComponent::create());
        Engine::renderView()->setBackgroundColor(Color::Gray);

        //m_sprite = Sprite::create(Texture2D::whiteTexture());
        //m_sprite->setBlendMode(BlendMode::Normal);
        //m_sprite->setAlignments(UIHAlignment::Left, UIVAlignment::Top);

        //m_plane = PlaneMesh::create();
        //auto planeMaterial = Material::create(Texture2D::load(_TT("D:/Materials/KitBash3D/WARZONE/Blender/KB3D_Debris_Diffuse.jpg"));
        //m_plane->planeMeshComponent()->setMaterial(planeMaterial);

        //auto sprite2 = Sprite::create(Texture2D::whiteTexture());

        //m_plane = PlaneMesh::create();
        //m_plane->planeMeshComponent()->setSize(10000, 10000);
        //auto texture = Texture2D::load(_TT("D:/Materials/KitBash3D/WARZONE/Blender/KB3D_Debris_Diffuse.jpg");
        //texture->setMipmapEnabled(true);
        //auto planeMaterial = Material::create(texture);
        //auto normalMap = Texture2D::load(_TT("D:/Materials/KitBash3D/WARZONE/Blender/KB3D_Debris_Normal.jpg");
        //normalMap->setMipmapEnabled(true);
        ////planeMaterial->setNormalMap(normalMap);
        //m_plane->planeMeshComponent()->setMaterial(planeMaterial);
        ////m_plane->planeMeshComponent()->setUVParUnit(Vector2(0.5f, 0.5f));
        //m_plane->planeMeshComponent()->setUVParUnit(Vector2(5, 5));

        //auto mesh = Mesh::create(_TT("D:/Materials/KitBash3D/WARZONE/Blender/untitled4.glb");


        //auto mesh = Mesh::create(_TT("D:/Tech/Graphics/glTF-Sample-Models/2.0/2CylinderEngine/glTF-Binary/2CylinderEngine.glb");
        
		//auto mesh = Mesh::create(_TT("C:/Proj/LN/Lumino/src/LuminoEngine/sandbox/Assets/autotile-regions.glb");
        //auto mesh = SkinnedMesh::load(_TT("C:/Proj/LN/Lumino/src/LuminoEngine/sandbox/Assets/SkinnedMesh2.glb");
        //auto mesh = SkinnedMesh::load(_TT("D:/LocalProj/UnityChanSD/Misaki_sum_humanoid.fbx");
        //auto mesh = Mesh::create(_TT("D:/Materials/VRM/Alicia_VRM/Alicia/VRM/AliciaSolid.glb");
        //auto mesh = Mesh::create(_TT("D:/Materials/VRM/PronamaChan/PronamaChan.glb");

        //auto mesh = SkinnedMesh::load(_TT("D:/Tech/Graphics/glTF-Sample-Models/2.0/AnimatedCube/glTF/AnimatedCube.gltf");
        //auto mesh = SkinnedMesh::load(_TT("D:/Tech/Graphics/glTF-Sample-Models/2.0/AnimatedMorphCube/glTF/AnimatedMorphCube.gltf");
        //auto mesh = SkinnedMesh::load(_TT("D:/Tech/Graphics/glTF-Sample-Models/2.0/Monster/glTF/Monster.gltf");
        
        
        //mesh->skinnedMeshComponent()->model()->animationController()->play(_TT("animation_AnimatedCube");
        //auto mesh = Mesh::create("D:/Materials/KitBash3D/WARZONE/ResolvedTextureReference/Exports/BldgMedTallC_grp.gltf");
        //auto mesh = Mesh::create("D:/Materials/KitBash3D/WARZONE/ResolvedTextureReference/Exports/2-OverpassBroken_grp.gltf");
        //auto mesh = Mesh::create(_TT("D:/Materials/KitBash3D/WARZONE/ResolvedTextureReference/Exports/27-BldgRubbleB_grp.gltf");
        
        //mesh->setPosition(0, 10, 0);
		//auto mesh = Mesh::create(_TT("C:/Proj/LN/Lumino/src/LuminoEngine/sandbox/Assets/Models/Axis.glb");
		//auto mesh = Mesh::create(_TT("C:/Proj/LN/PrivateProjects/HC4/assets/Map/Exported/MainMap1-Start.gltf");
		//auto mesh = Mesh::create(_TT("C:/Proj/LN/Lumino/src/LuminoEngine/test/Assets/Mesh/Axis1.glb");
        //auto mesh = Mesh::create(_TT("C:/Proj/LN/PrivateProjects/HC4/assets/Graphics/Spear1.gltf");
        //mesh->model()->material(1)->setColor(Color(0, 1, 0));
        //mesh->model()->material(1)->setEmissive(Color(0.5, 2, 0.5));
        Engine::renderView()->setHDREnabled(true);
        Scene::setSSAOEnabled(true);
#if 1

        Engine::mainLight()->setPosition(30, 20, 10);
        Engine::mainLight()->lookAt(0, 0, 0);
        Engine::mainLight()->setAmbientColor(Color::White);
        Engine::mainLight()->setColor(Color::White);
        Engine::mainLight()->setIntensity(1.0);
        Engine::mainLight()->setShadowEffectiveDepth(100);
        Engine::mainLight()->setShadowEffectiveDistance(100);


        //m_mesh = SkinnedMesh::load(_TT("D:/Materials/MMD/Appearance Miku/Appearance Miku_BDEF.pmx");
        //m_mesh = SkinnedMesh::load(_TT("D:/Materials/VRM/Alicia_VRM/Alicia/VRM/AliciaSolid.glb");
        //m_mesh = SkinnedMesh::load(_TT("D:/Materials/VRM/Alicia_VRM/Alicia/VRM/AliciaSolid_BlenderGLTFExported.glb");
        //m_mesh = SkinnedMesh::load(_TT("D:/Materials/VRM/Sendagaya_Shibu.vrm");
        //m_mesh = SkinnedMesh::load(_TT("D:/Documents/Modeling/HC4-1.glb");
        m_mesh = SkinnedMesh::load(_TT("D:/Documents/Modeling/HC5-6-export.glb"));



        m_model = m_mesh->meshComponent()->model();
        MeshDiag::printNodes(m_model);
        //MeshDiag::clearBoneInitialRotations(m_model);
        //MeshDiag::printNodes(m_model);
        m_model->verifyHumanoidBones();

        if (auto node = m_model->findNode(_TT("Icosphere.001"))) {
            node->setVisible(false);
        }
        if (auto material = m_model->findMaterial(_TT("Stone"))) {
            material->setEmissive(Color(0.5, 2, 0.5));
        }

        //m_node = m_model->findNode(_TT("左腕");
        m_node = m_model->findHumanoidBone(HumanoidBones::Hips);
        m_node = m_model->findHumanoidBone(HumanoidBones::LeftUpperArm);
        m_node = m_model->findHumanoidBone(HumanoidBones::LeftLowerArm);
        m_node = m_model->findHumanoidBone(HumanoidBones::LeftLowerLeg);
        //m_node = m_model->findHumanoidBone(HumanoidBones::Head);
        //auto mat = m_node->initialLocalTransform();
        //m_node->setInitialLocalTransform();
        //m_node->setRotation(0, 0, -Math::PI);
        // Note: ここで m_node->setRotation しても、次のフレーム更新時にリセットされるので意味なし

        //m_mesh->setShadingModel(ShadingModel::Unlit);
        m_mesh->setCullMode(CullMode::None);


        //auto clip = AnimationClip::load(_TT("D:/Materials/VRM/walk2.bvh");
        //auto clip = AnimationClip::load(_TT("D:/Materials/Mixamo/FemaleStandingPose5.bvh");
        //auto clip = AnimationClip::load(_TT("D:/Documents/Modeling/BVH/XYZ-Test.bvh");
        //auto clip = AnimationClip::load(_TT("D:/Documents/Modeling/BVH/XYZ-Test2.bvh");
        //auto clip = AnimationClip::load(_TT("D:/Documents/Modeling/BVH/RightArm.bvh");
        //auto clip = AnimationClip::load(_TT("D:/Materials/Mixamo/FemaleStandingPose7.bvh");
        //auto clip = AnimationClip::load(_TT("D:/Materials/Mixamo/FemaleSittingPose.bvh");
        //auto clip = AnimationClip::load(_TT("D:/Documents/Modeling/BVH/Arm_R-Test1-BoneVert.bvh");
        auto clip = AnimationClip::load(_TT("D:/Materials/Mixamo/Idle.bvh"));
        //auto clip = AnimationClip::load(_TT("D:/Materials/Mixamo/Walk.bvh");
        //auto clip = AnimationClip::load(_TT("D:/Materials/Mixamo/Standing2.bvh");
        //auto clip = AnimationClip::load(_TT("D:/Documents/Modeling/BVH/HC5-3.bvh");
        //clip->setHierarchicalAnimationMode(HierarchicalAnimationMode::AllowTranslationOnlyRootY);
        
        //auto clip = AnimationClip::load(_TT("D:/Materials/MMD/Motion/■配布用（モーション）/歩き/歩行（歩幅5・直進）.vmd");
        //auto clip1 = AnimationClip::load(_TT("D:/Materials/MMD/Motion/MMO用stand/stand2.vmd");
        //auto clip2 = AnimationClip::load(_TT("D:/Materials/MMD/Motion/走歩スv2.2full/歩く/A01_SO_女の子歩き_s591_p40.vmd");
        //
        //m_idle = m_model->animationController()->addClip(clip1);
        //m_walk = m_model->animationController()->addClip(clip2);
        //m_model->animationController()->play(m_idle);

        auto state = m_model->animationController()->addClip(clip);
        m_model->animationController()->play(state);

        //auto box = BoxMesh::With().build();
        //box->setScale(7.8);
        //box->setPosition(0, 5, 0);
#endif

#if 1
        auto label = makeObject<UIControl>();
        //label->setBackgroundColor(Color::Blue);
        label->setAlignments(UIAlignment::Center);

        auto textblock = UIText::create(_T("E"));
        textblock->setBackgroundColor(Color::Gray);
        textblock->setBorderThickness(1);
        textblock->setBorderColor(Color::Red);
        textblock->setPadding(Thickness(4, 2));
        textblock->setCornerRadius(3);
        //textblock->setSize(20, 20);
        label->addInlineVisual(textblock, UIInlinePlacement::Left);

        auto text2 = UIText::create(_T("Examine"));
        text2->setAlignments(UIAlignment::Center);
        text2->setMargin(Thickness(8, 0, 0, 0));
        label->addChild(text2);

        UI::add(label);
#endif
    }

    virtual void onUpdate() override
    {
        //if (m_node) {
        //    //m_node->setRotation(0, 0, Engine::time());
        //    //m_node->setRotation(Engine::time(), 0, 0);
        //    m_node->setRotation(0, Engine::time(), 0);
        //    //m_node->setRotation(Quaternion(0.00719260797, -0.0159967896, -0.317227066, 0.948187351));
        //}

        if (0) {

            // 左右の腕を前に伸ばす
            m_model->findHumanoidBone(HumanoidBones::LeftUpperArm)->setRotation(0, Math::PI / 2, 0);
            m_model->findHumanoidBone(HumanoidBones::RightUpperArm)->setRotation(0, -Math::PI / 2, 0);

            // かがむ
            m_model->findHumanoidBone(HumanoidBones::Spine)->setRotation(Math::PI / 4, 0, 0);

            // 右足を前に出す・膝を曲げる・内また
            m_model->findHumanoidBone(HumanoidBones::RightUpperLeg)->setRotation(-Math::PI / 4, 0, 0);
            m_model->findHumanoidBone(HumanoidBones::RightLowerLeg)->setRotation(Math::PI / 4, 0, 0);
            m_model->findHumanoidBone(HumanoidBones::RightFoot)->setRotation(0, -Math::PI / 4, 0);

        }
           
        //m_model->findHumanoidBone(HumanoidBones::Hips)->setRotation(0, 0, 0);
#if 0
        float velocity = 0.15;

        auto pos = m_mesh->position();

        //if (Input::pressed(_TT("left")) {
        //    pos.x -= 0.01;
        //}

        const float cameraFocusDistance = 2;    // 10
        const float cameraDistance = 3;

        float h = -Input::getAxisValue(_TT("left") + Input::getAxisValue(_TT("right");
        float v = -Input::getAxisValue(_TT("up") + Input::getAxisValue(_TT("down");
        pos.x += h * velocity;
        pos.z += v * velocity;

        if (h != 0 || v != 0) {
            auto dir = Vector3::normalize(-h, 0, -v);
            m_mesh->lookAt(pos + dir);
            m_model->animationController()->play(m_walk);


            m_targetPos = pos - dir * cameraFocusDistance;
        }
        else {
            m_model->animationController()->play(m_idle);
        }

        m_lookPos += (m_targetPos - m_lookPos) / 60;


        Vector3 lookat = m_lookPos + Vector3(0, 1, 0);
        Engine::camera()->setPosition(lookat + Vector3::normalize(0, 15, -30) * cameraDistance);
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




