
#include <LuminoEngine.hpp>
#include <LuminoEngine/Scene/Reflection/Reflector.hpp>
#include <LuminoEngine/Scene/Reflection/ReflectorComponent.hpp>
using namespace ln;

class App_Experiment_Reflector : public Application
{
    Ref<BoxMesh> m_box;
    Ref<SphereMesh> m_sphere;

    virtual void onInit() override
    {
		//Engine::renderView()->setGuideGridEnabled(true);
		Engine::mainCamera()->addComponent(CameraOrbitControlComponent::create());

        auto boxMaterial = Material::create();
        boxMaterial->setColor(Color::Green);
        m_box = BoxMesh::With()
            .material(boxMaterial)
            .build();
        m_box->setScale(5, 10, 5);

        auto sphereMaterial = Material::create();
        sphereMaterial->setColor(Color::Red);
        m_sphere = SphereMesh::create();
        m_sphere->sphereMeshComponent()->setMaterial(sphereMaterial);
        m_sphere->setScale(5);


        auto reflector2 = Reflector::create();
        reflector2->setScale(2);
        reflector2->setPosition(0, 10, 10);
        reflector2->setRotation(-Math::PI / 2, 0, 0);

		auto reflector = Reflector::create();
        reflector->setScale(2);
        reflector->setPosition(0, 0.5);
        //auto sprite = UISprite::create(reflector->reflectorComponent()->renderTarget());
	}

    virtual void onUpdate() override
    {
        m_sphere->setPosition(4, 3.0 * std::sin(Engine::time()) + 10, 0);

        m_box->setPosition(-4, 10, 0);
        m_box->setRotation(0, Engine::time(), 0);

    }
};

void Experiment_Reflector()
{
	App_Experiment_Reflector app;
	detail::ApplicationHelper::run(&app);
}




