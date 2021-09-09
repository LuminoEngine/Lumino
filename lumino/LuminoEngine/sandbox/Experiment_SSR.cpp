
#include <LuminoEngine.hpp>
#include <LuminoEngine/PostEffect/SSRPostEffect.hpp>
using namespace ln;

class App_Experiment_SSR : public Application
{
    Ref<PlaneMesh> m_ground;
    Ref<PlaneMesh> m_walls[4];
    Ref<BoxMesh> m_box;
    Ref<SphereMesh> m_sphere;

    Ref<Sprite> m_sprite;

    virtual void onInit() override
    {
        //Engine::renderView()->setGuideGridEnabled(true);
        Engine::mainCamera()->addComponent(CameraOrbitControlComponent::create());
        Engine::renderView()->setBackgroundColor(Color::Gray);

        //Engine::camera()->setFarClip(100);
        Engine::mainCamera()->setFov(Math::degreesToRadians(50));
        Engine::mainCamera()->setPosition(0, 20, -50);
        Engine::mainCamera()->lookAt(0, 0, 0);

        Engine::mainLight()->setIntensity(3);

#ifdef LN_COORD_RH
        auto tmp = Vector3::transformCoord(Vector3(0, 0, 10), Matrix::makeLookAtRH(Vector3(0, 0, -1), Vector3::Zero, Vector3::UnitY));
#else
        auto tmp = Vector3::transformCoord(Vector3(0, 0, 10), Matrix::makeLookAtLH(Vector3(0, 0, -1), Vector3::Zero, Vector3::UnitY));
#endif

        m_sprite = Sprite::create(Texture2D::whiteTexture());
        m_sprite->setBlendMode(BlendMode::Normal);
        //m_sprite->setAlignments(UIHAlignment::Left, UIVAlignment::Top);

        auto groundMaterial = Material::create(Texture2D::load(_TT("D:/Tech/Graphics/ssr/assets/textures/tidal-pool1-ue/tidal-pool1-albedo.jpg")));
        groundMaterial->setMetallicRoughnessTexture(Texture2D::load(_TT("D:/Tech/Graphics/ssr/assets/textures/tidal-pool1-ue/tidal-pool1-roughness.jpg")));
        //groundMaterial->setNormalMap(Texture2D::load(_TT("D:/Tech/Graphics/ssr/assets/textures/tidal-pool1-ue/tidal-pool1-normal-dx.jpg")));
        m_ground = PlaneMesh::create(groundMaterial);
        m_ground->setScale(4);
        m_ground->addInto();

        auto wallMaterial = Material::create();
        //wallMaterial.bumpMap = textureLoader.load('assets/textures/brick_bump.jpg');
        wallMaterial->setMainTexture(Texture2D::load(_TT("D:/Tech/Graphics/ssr/assets/textures/brick_diffuse.jpg")));
        //wallMaterial.metallicRoughnessTexture = textureLoader.load('assets/textures/brick_roughness.jpg');
        //wallMaterial.bumpScale = 0.1;
        //wallMaterial->shadingModel = ShadingModel::Unlit;

        // X-
        m_walls[0] = PlaneMesh::create(wallMaterial);
        m_walls[0]->setPosition(-20, 20, 0);
        m_walls[0]->setRotation(-Math::PI / 2, -Math::PI / 2, 0);
        m_walls[0]->setScale(4);
        m_walls[0]->addInto();

        // X+
        m_walls[1] = PlaneMesh::create(wallMaterial);
        m_walls[1]->setPosition(20, 20, 0);
        m_walls[1]->setRotation(-Math::PI / 2, Math::PI / 2, 0);
        m_walls[1]->setScale(4);
        m_walls[1]->addInto();

        // Z-
        m_walls[2] = PlaneMesh::create(wallMaterial);
        m_walls[2]->setPosition(0, 20, -20);
        m_walls[2]->setRotation(-Math::PI / 2, Math::PI, 0);
        m_walls[2]->setScale(4);
        m_walls[2]->addInto();

        // Z+
        m_walls[3] = PlaneMesh::create(wallMaterial);
        m_walls[3]->setPosition(0, 20, 20);
        m_walls[3]->setRotation(-Math::PI / 2, 0, 0);
        m_walls[3]->setScale(4);
        m_walls[3]->addInto();

        auto boxMaterial = Material::create();
        boxMaterial->setColor(Color::Green);
        m_box = BoxMesh::With()
            .material(boxMaterial)
            .build();
        m_box->setScale(5, 10, 5);
        m_box->addInto();

        auto sphereMaterial = Material::create();
        sphereMaterial->setColor(Color::Red);
        m_sphere = SphereMesh::create();
        //m_sphere->setScale(1.5);
        m_sphere->sphereMeshComponent()->setMaterial(sphereMaterial);
        m_sphere->setScale(5);
        m_sphere->addInto();


        auto ssrPostEffect = makeObject<SSRPostEffect>();
        Engine::renderView()->addPostEffect(ssrPostEffect);

        //m_walls[0] = PlaneMesh::create(groundMaterial);
        //m_walls[0]->setPosition(0.0, 20.0, 20.0);

        //auto planeMaterial = Material::create();
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
        //mesh->setShadingModel(ShadingModel::Unlit);

        //auto model = mesh->skinnedMeshComponent()->model();
        //auto skeleton = model->skeleton(0);
        //auto node = model->meshNodes()[3];
        //node->setRotation(0, 0, 0.3);

        //auto li = SpotLight::create();
        //li->setPosition(0, 0.1, 0);

#if 0	// 雨
        auto particleModel = makeObject<ParticleModel>();
        auto m1 = particleModel->emitters()[0];
        m1->m_maxParticles = 10000;
        m1->setSpawnRate(1000);
        //m1->setSpawnRate(10);
        m1->setLifeTime(1.0);
        particleModel->m_loop = true;

        m1->m_shapeType = ParticleEmitterShapeType::Box;
        m1->m_shapeParam.set(10, 0, 10);
        m1->m_forwardVelocity.minValue = 12;
        m1->m_forwardVelocity.maxValue = 12;

        //m1->m_size.set(0.05f);
        //m1->m_size.set(0.5f);
        //m1->m_forwardScale.set(10);

        //m1->m_particleDirection = ParticleDirectionType::MovementDirection;
        m1->m_sortMode = ParticleSortMode::DistanceToView;

        auto material = Material::create();
        material->setMainTexture(Texture2D::load("C:/Proj/LN/Lumino/src/LuminoEngine/test/Assets/Effect/Particle1-alpha.png"));
        material->shadingModel = ShadingModel::Unlit;
        material->setShader(Shader::create(_TT("C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/Sprite.fx"));
        m1->setSpriteModule(material);

        auto particle1 = makeObject<ParticleEmitterComponent2>(particleModel);
        //particle1->setBlendMode(BlendMode::Add);
        particle1->setBlendMode(BlendMode::Alpha);
        auto obj1 = makeObject<WorldObject>();
        obj1->addComponent(particle1);
        //obj1->setPosition(0, 5, 0);
        //particle1->setAngles(Math::PI, 0, 0);



        /*
        // 波紋
        auto m2 = SpriteParticleModel::create();
        m2->m_maxParticles = 1000;
        m2->setSpawnRate(200);
        m2->setLifeTime(0.2);
        m2->m_loop = true;
        m2->setSize(0.1, 0.1);
        m2->m_minSizeVelocity = 3;
        m2->m_maxSizeVelocity = 3;
        m2->m_shapeType = ParticleEmitterShapeType::Box;
        m2->m_shapeParam.set(10, 0, 10);
        m2->m_particleDirection = ParticleDirectionType::Horizontal;
        m2->setMaterial(material);

        //auto particle2 = ParticleEmitter3D::create(m2);
        //particle2->SetBlendMode(BlendMode::Add);
        auto particle2 = makeObject<ParticleEmitterComponent>(m2);
        particle2->setBlendMode(BlendMode::Add);
        auto obj2 = makeObject<WorldObject>();
        obj2->addComponent(particle2);
        //obj2->setPosition(0, 12, 0);
        */
#endif
    }

    virtual void onUpdate() override
    {
        //printf("==========/n");
        //m_plane->setRotation(0, Engine::time() * 0.1, 0);

        //m_sprite->setTexture(ln::g_srTarget);
        //m_sprite->setSize(320, 240);
        //m_sprite->setTexture(ln::g_viewMaterialMap);
        m_sprite->setPosition(0, -10, 0);
        m_sprite->setScale(10);

        m_box->setPosition(-4, 0.5, 0);
        m_box->setRotation(0, Engine::time(), 0);

        m_sphere->setPosition(4, 3.0 * std::sin(Engine::time()), 0);

        
    }
};

void Experiment_SSR()
{
    App_Experiment_SSR app;
	detail::ApplicationHelper::run(&app);
}




