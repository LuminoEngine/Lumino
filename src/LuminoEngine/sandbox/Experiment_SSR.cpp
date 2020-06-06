﻿
#include <LuminoEngine.hpp>
#include <LuminoEngine/ImageEffect/SSRImageEffect.hpp>
using namespace ln;

namespace ln {
    extern Texture* g_viewNormalMap;
    extern Texture* g_viewMaterialMap;
}

class App_Experiment_SSR : public Application
{
    Ref<PlaneMesh> m_ground;
    Ref<PlaneMesh> m_walls[4];
    Ref<BoxMesh> m_box;
    Ref<SphereMesh> m_sphere;

    Ref<Sprite> m_sprite;

    virtual void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        Engine::camera()->addComponent(CameraOrbitControlComponent::create());
        Engine::renderView()->setBackgroundColor(Color::Gray);

        m_sprite = Sprite::create(Texture2D::whiteTexture());
        m_sprite->setBlendMode(BlendMode::Normal);
        //m_sprite->setAlignments(HAlignment::Left, VAlignment::Top);

        auto groundMaterial = Material::create(Texture2D::load(u"D:/Tech/Graphics/ssr/assets/textures/tidal-pool1-ue/tidal-pool1-albedo.jpg"));
        groundMaterial->setRoughnessMap(Texture2D::load(u"D:/Tech/Graphics/ssr/assets/textures/tidal-pool1-ue/tidal-pool1-roughness.jpg"));
        m_ground = PlaneMesh::create(groundMaterial);

        auto wallMaterial = Material::create();
        //wallMaterial.bumpMap = textureLoader.load('assets/textures/brick_bump.jpg');
        wallMaterial->setMainTexture(Texture2D::load(u"D:/Tech/Graphics/ssr/assets/textures/brick_diffuse.jpg"));
        //wallMaterial.roughnessMap = textureLoader.load('assets/textures/brick_roughness.jpg');
        //wallMaterial.bumpScale = 0.1;
        //wallMaterial->shadingModel = ShadingModel::Unlit;

        // X-
        m_walls[0] = PlaneMesh::create(wallMaterial);
        m_walls[0]->setPosition(-5, 5, 0);
        m_walls[0]->setRotation(-Math::PI / 2, -Math::PI / 2, 0);

        // X+
        m_walls[1] = PlaneMesh::create(wallMaterial);
        m_walls[1]->setPosition(5, 5, 0);
        m_walls[1]->setRotation(-Math::PI / 2, Math::PI / 2, 0);

        // Z-
        m_walls[2] = PlaneMesh::create(wallMaterial);
        m_walls[2]->setPosition(0, 5, -5);
        m_walls[2]->setRotation(-Math::PI / 2, Math::PI, 0);

        // Z+
        m_walls[3] = PlaneMesh::create(wallMaterial);
        m_walls[3]->setPosition(0, 5, 5);
        m_walls[3]->setRotation(-Math::PI / 2, 0, 0);

        auto boxMaterial = Material::create();
        boxMaterial->setColor(Color::Green);
        m_box = BoxMesh::create();
        m_box->boxMeshComponent()->setMaterial(boxMaterial);

        auto sphereMaterial = Material::create();
        sphereMaterial->setColor(Color::Red);
        m_sphere = SphereMesh::create();
        m_sphere->sphereMeshComponent()->setMaterial(sphereMaterial);


        auto ssrImageEffect = makeObject<SSRImageEffect>();
        Engine::renderView()->addImageEffect(ssrImageEffect);

        //m_walls[0] = PlaneMesh::create(groundMaterial);
        //m_walls[0]->setPosition(0.0, 20.0, 20.0);

        //auto planeMaterial = Material::create();
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
        //printf("==========/n");
        //m_plane->setRotation(0, Engine::time() * 0.1, 0);

        m_sprite->setTexture(ln::g_viewNormalMap);
        //m_sprite->setSize(320, 240);
        m_sprite->setPosition(0, 1, 0);


        m_box->setPosition(-2.5, 0.5, 0);
        m_box->setRotation(0, Engine::time(), 0);

        m_sphere->setPosition(2.5, std::sin(Engine::time()), 0);
        
    }
};

void Experiment_SSR()
{
    App_Experiment_SSR app;
	detail::ApplicationHelper::run(&app);
}




