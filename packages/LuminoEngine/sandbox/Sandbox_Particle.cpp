
#include <LuminoEngine.hpp>
using namespace ln;

class App_Sandbox_Particle : public Application
{
    Ref<WorldObject> m_particleObj;

    void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        Engine::mainCamera()->addComponent(CameraOrbitControlComponent::create());
        Engine::renderView()->setBackgroundColor(Color::Gray);
#if 0
        auto material = Material::create();
        material->setMainTexture(Texture2D::load("C:/Proj/LN/Lumino/src/LuminoEngine/test/Assets/Effect/ColorGrid.png"));
        material->shadingModel = ShadingModel::Unlit;
        material->setShader(Shader::create(_TT("C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/Sprite.fx")));
        

        auto particleModel = makeObject_deprecated<ParticleModel>();
        auto m1 = particleModel->emitters()[0];
        m1->setSpriteModule(material);
        m1->setLifeTime(10);
        m1->setMaxParticles(2);
        m1->setSpawnRate(1);
        m1->m_forwardVelocity.set(0);
        m1->m_velocity.set(Vector3(0, 0, 1));

        //m1->setMaterial(material);
        //m1->setSpawnRate(1);
        //m1->setLifeTime(2.0f);
        //m1->m_maxParticles = 1;
        //m1->m_shapeType = ParticleEmitterShapeType::Cone;
        //m1->m_shapeParam.x = Math::PI * 0.1;
        //m1->m_shapeParam.y = 2;
        auto cmp1 = makeObject_deprecated<ParticleEmitterComponent2>(particleModel);
        cmp1->setCullMode(CullMode::None);
        cmp1->setBlendMode(BlendMode::Alpha);

        auto obj1 = makeObject_deprecated<WorldObject>();
        obj1->addComponent(cmp1);
#endif

#if 0
        auto material = Material::create();
        material->setMainTexture(Texture2D::load("C:/Proj/LN/Lumino/src/LuminoEngine/test/Assets/Effect/ColorGrid.png"));
        material->shadingModel = ShadingModel::Unlit;
        material->setShader(Shader::create(_TT("C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/Sprite.fx")));


        auto particleModel = makeObject_deprecated<ParticleModel>();
        auto m1 = particleModel->emitters()[0];
        m1->setSpriteModule(material);
        m1->setLifeTime(10);
        m1->setMaxParticles(10);
        m1->setSpawnRate(1);
        //m1->m_forwardVelocity.set(0);
        //m1->m_velocity.set(Vector3(0, 0, 1));
        m1->m_acceleration.set(Vector3(0, -0.5, 0));
        m1->m_trailSeconds = 1.0f;

        //m1->setMaterial(material);
        //m1->setSpawnRate(1);
        //m1->setLifeTime(2.0f);
        //m1->m_maxParticles = 1;
        //m1->m_shapeType = ParticleEmitterShapeType::Cone;
        //m1->m_shapeParam.x = Math::PI * 0.1;
        //m1->m_shapeParam.y = 2;
        auto cmp1 = makeObject_deprecated<ParticleEmitterComponent2>(particleModel);
        cmp1->setCullMode(CullMode::None);
        cmp1->setBlendMode(BlendMode::Alpha);

        auto obj1 = makeObject_deprecated<WorldObject>();
        obj1->addComponent(cmp1);
#endif
#if 0
        auto material = Material::create();
        material->setMainTexture(Texture2D::load(_TT("Sprite1")));

        auto m1 = SpriteParticleModel::create();
        m1->setMaterial(material);
        m1->setSpawnRate(1);
        m1->setLifeTime(2.0f);
        m1->m_maxParticles = 1;
        m1->m_shapeType = ParticleEmitterShapeType::Cone;
        m1->m_shapeParam.x = Math::PI * 0.1;
        m1->m_shapeParam.y = 2;
        auto cmp1 = makeObject_deprecated<ParticleEmitterComponent>(m1);
        cmp1->setCullMode(CullMode::None);
        cmp1->setBlendMode(BlendMode::Alpha);

        auto obj1 = makeObject_deprecated<WorldObject>();
        obj1->addComponent(cmp1);
#endif
#if 0	// 雨
        auto m1 = makeObject_deprecated<ParticleEmitterModel>();
        m1->m_maxParticles = 10000;
        m1->setSpawnRate(1000);
        //m1->setSpawnRate(10);
        m1->setLifeTime(1.0);


        m1->m_shapeType = ParticleEmitterShapeType::Box;
        m1->m_shapeParam.set(20, 0, 20);
        //m1->m_forwardVelocity.minValue = -10;
        //m1->m_forwardVelocity.maxValue = -10;
        m1->setForwardVelocityMin(-10);
        m1->setForwardVelocityMax(-10);

        m1->setSize(0.05f);
        //m1->m_size.set(0.5f);
        m1->setForwardScale(20);

        m1->m_geometryDirection = ParticleGeometryDirection::VerticalBillboard;
        m1->m_sortMode = ParticleSortMode::DistanceToView;


        auto particleModel = makeObject_deprecated<ParticleModel>();
        particleModel->m_loop = true;
        particleModel->addEmitter(m1);

        auto material = Material::create();
        //material->setMainTexture(Texture2D::load("C:/Proj/LN/Lumino/src/LuminoEngine/test/Assets/Effect/Particle1-alpha.png"));
        material->setMainTexture(Texture2D::load("C:/Proj/LN/Lumino/src/LuminoEngine/test/Assets/Effect/Particle1.png"));
        //material->setMainTexture(Texture2D::load("C:/Proj/LN/PrivateProjects/HC4/assets/Graphics/WaterDrop-1.png"));
        material->setShadingModel(ShadingModel::Unlit);
        material->setShader(Shader::create(_TT("C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/Sprite.fx")));
        m1->setupSpriteModule(material);

        //detail::AssetObjectInternal::setAssetPath(particleModel, _TT("Particle.yml");
        //Assets::save(particleModel);

        m_particleObj = makeObject_deprecated<ParticleEmitter>(particleModel);
        m_particleObj->setPosition(0, 10, 0);

        //auto particle1 = makeObject_deprecated<ParticleEmitterComponent2>(particleModel);
        ////particle1->setBlendMode(BlendMode::Add);
        //particle1->setBlendMode(BlendMode::Alpha);
        //m_particleObj = makeObject_deprecated<WorldObject>();
        //m_particleObj->addComponent(particle1);
        //m_particleObj->setPosition(0, 10, 0);
        ////particle1->setAngles(Math::PI, 0, 0);



#endif
#if 1

        auto material2 = Material::create();
        //material->setMainTexture(Texture2D::load("C:/Proj/LN/Lumino/src/LuminoEngine/test/Assets/Effect/Particle1-alpha.png"));
        material2->setMainTexture(Texture2D::load(_TT("C:/Proj/LN/Lumino/src/LuminoEngine/test/Assets/Effect/Particle1.png")));
        //material->setMainTexture(Texture2D::load("C:/Proj/LN/PrivateProjects/HC4/assets/Graphics/WaterDrop-1.png"));
        material2->setShadingModel(ShadingModel::Unlit);
        material2->setShader(Shader::create(_TT("C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/Sprite.fx")));


        // 波紋
        auto particleModel2 = makeObject_deprecated<ParticleModel>();
        auto m2 = makeObject_deprecated<ParticleEmitterModel>();
        m2->m_maxParticles = 1000;
        m2->setSpawnRate(200);
        m2->setLifeTime(0.2);
        particleModel2->m_loop = true;
        //m2->m_size.set(1);
        m2->m_size.set(0.1);
        m2->m_sizeVelocity.set(3);
        m2->m_sizeAcceleration.set(3);
        m2->m_shapeType = ParticleEmitterShapeType::Box;
        m2->m_shapeParam.set(10, 0, 10);
        m2->m_geometryDirection = ParticleGeometryDirection::HorizontalBillboard;
        m2->setupSpriteModule(material2);
        particleModel2->addEmitter(m2);

        //auto particle2 = ParticleEmitter3D::create(m2);
        //particle2->SetBlendMode(BlendMode::Add);
        auto particle2 = makeObject_deprecated<ParticleEmitterComponent2>(particleModel2);
        particle2->setBlendMode(BlendMode::Add);
        auto obj2 = makeObject_deprecated<WorldObject>();
        obj2->addComponent(particle2);
        //obj2->setPosition(0, 12, 0);
#endif
#if 0
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
        auto particle2 = makeObject_deprecated<ParticleEmitterComponent>(m2);
        particle2->setBlendMode(BlendMode::Add);
        auto obj2 = makeObject_deprecated<WorldObject>();
        obj2->addComponent(particle2);
        //obj2->setPosition(0, 12, 0);
#endif
#if 0	// 旧雨
        auto m1 = SpriteParticleModel::create();
        m1->m_maxParticles = 10000;
        m1->setSpawnRate(1000);
        m1->setLifeTime(1.0);
        m1->m_loop = true;

        m1->setSize(0.05, 0.05);

        m1->m_shapeType = ParticleEmitterShapeType::Box;
        m1->m_shapeParam.set(10, 0, 10);

        m1->m_particleDirection = ParticleDirectionType::MovementDirection;
        m1->m_forwardVelocity.minValue = -12;
        m1->m_forwardVelocity.maxValue = -12;
        m1->m_lengthScale = 10;

        auto material = Material::create();
        material->setMainTexture(Texture2D::load("D:/Proj/LN/Lumino/src/LuminoEngine/test/Assets/Effect/Particle1.png"));
        m1->setMaterial(material);

        auto particle1 = makeObject_deprecated<ParticleEmitterComponent>(m1);
        particle1->setBlendMode(BlendMode::Add);
        auto obj1 = makeObject_deprecated<WorldObject>();
        obj1->addComponent(particle1);
        obj1->setPosition(0, 5, 0);
        //particle1->setAngles(Math::PI, 0, 0);


        /*
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
        auto particle2 = makeObject_deprecated<ParticleEmitterComponent>(m2);
        particle2->setBlendMode(BlendMode::Add);
        auto obj2 = makeObject_deprecated<WorldObject>();
        obj2->addComponent(particle2);
        //obj2->setPosition(0, 12, 0);
        */
#endif



    }

    void onUpdate() override
    {
        if (m_particleObj) {
            auto pos = Engine::mainCamera()->position();
            pos.y = 10;

            //pos.x = std::cos(Engine::time()) * 10;
            //pos.z = std::sin(Engine::time()) * 10;

            m_particleObj->setPosition(pos);
            //m_particleObj->setPosition(10, 0, 0);
        }
    }
};

void Sandbox_Particle()
{
    App_Sandbox_Particle app;
	detail::ApplicationHelper::run(&app);
}




