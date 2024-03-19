
#include <LuminoEngine.hpp>
#include <LuminoEngine/Mesh/MeshProcessing.hpp>
#include <LuminoEngine/PostEffect/SSRPostEffect.hpp>
#include <LuminoEngine/PostEffect/SSAOPostEffect.hpp>
#include <LuminoEngine/PostEffect/TonemapPostEffect.hpp>
#include <LuminoEngine/PostEffect/DepthOfFieldPostEffect.hpp>
#include <LuminoEngine/PostEffect/FXAAPostEffect.hpp>
using namespace ln;

class App_Sandbox_PostEffect : public Application
{
    void onInit() override
    {
        //Engine::renderView()->setGuideGridEnabled(true);
        Engine::mainCamera()->addComponent(CameraOrbitControlComponent::create());
        Engine::renderView()->setBackgroundColor(Color::Black);
		Engine::renderView()->setClearMode(SceneClearMode::SkyDome);
		//Engine::ambientLight()->setIntensity(0.5);

		Engine::mainCamera()->setFarClip(100);
        //auto mesh = Mesh::create(u"D:/Tech/Tools/cornellBox/BJS-2.79-Cycles-gltf/assets/cornellBox-2.79-Cycles-gltf.bin");

		float size = 10.0f;
		float hs = 10.0f / 2.0f;
        auto builder = makeObject_deprecated<MeshGeometryBuilder>();
        builder->beginSection();
        builder->addPlane(Vector2(size, size), 1, 1);

		builder->setPosition(Vector3(0, hs, hs));
		builder->setRotation(Vector3(-Math::PI / 2, 0, 0));
        builder->addPlane(Vector2(size, size), 1, 1);

		builder->setPosition(Vector3(0, size, 0));
		builder->setRotation(Vector3(Math::PI, 0, 0));
		builder->addPlane(Vector2(size, size), 1, 1);

		builder->setPosition(Vector3(-hs, hs, 0));
		builder->setRotation(Vector3(0, 0, -Math::PI / 2));
		builder->setColor(Color::Red);
		builder->addPlane(Vector2(size, size), 1, 1);

		builder->setPosition(Vector3(hs, hs, 0));
		builder->setRotation(Vector3(0, 0, Math::PI / 2));
		builder->setColor(Color::Green);
		builder->addPlane(Vector2(size, size), 1, 1);

		builder->setPosition(Vector3(-2.0f, 3.0f, 1.0f));
		builder->setRotation(Vector3(0, -Math::PI / 16, 0));
		builder->setColor(Color::White);
		builder->addBox(Vector3(3.0f, 6.0f, 3.0f));

		builder->setPosition(Vector3(2.0f, 1.5f, -1.0f));
		builder->setRotation(Vector3(0, 0, 0));
		builder->setColor(Color::White);
		builder->addSphere(1.5f, 16, 16);

        builder->endSection();
        auto mesh = builder->buildMesh();

        auto model = makeObject_deprecated<MeshModel>();
        model->addMeshContainerNode(mesh);
        model->addMaterial(Material::create());
        model->updateNodeTransforms();

        auto obj = makeObject_deprecated<StaticMesh>(model);


		//auto effect2 = makeObject_deprecated<SSRPostEffect>();
		//Engine::renderView()->addPostEffect(effect2);

		//auto effect1 = makeObject_deprecated<SSAOPostEffect>();
		//Engine::renderView()->addPostEffect(effect1);

		//auto effect6 = makeObject_deprecated<FXAAPostEffect>();
		//Engine::renderView()->addPostEffect(effect6);


		//auto effect5 = makeObject_deprecated<DepthOfFieldPostEffect>();
		//Engine::renderView()->addPostEffect(effect5);
		

		//auto effect4 = makeObject_deprecated<BloomPostEffect>();
		//effect4->setStrength(0.1);
		//Engine::renderView()->addPostEffect(effect4);

  //      auto effect3 = makeObject_deprecated<TonemapPostEffect>();
  //      Engine::renderView()->addPostEffect(effect3);


    }

    void onUpdate() override
    {
    }
};

void Sandbox_PostEffect()
{
    App_Sandbox_PostEffect app;
	detail::ApplicationHelper::run(&app);
}




