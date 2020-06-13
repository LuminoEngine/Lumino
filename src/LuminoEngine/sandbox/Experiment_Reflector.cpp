
#include <LuminoEngine.hpp>
#include <LuminoEngine/Scene/Reflection/Reflector.hpp>
using namespace ln;

class App_Experiment_Reflector : public Application
{
    Ref<BoxMesh> m_box;
    Ref<SphereMesh> m_sphere;

    virtual void onInit() override
    {
		Engine::renderView()->setGuideGridEnabled(true);
		Engine::camera()->addComponent(CameraOrbitControlComponent::create());

        auto boxMaterial = Material::create();
        boxMaterial->setColor(Color::Green);
        m_box = BoxMesh::create();
        m_box->boxMeshComponent()->setMaterial(boxMaterial);
        m_box->setScale(5, 10, 5);

        auto sphereMaterial = Material::create();
        sphereMaterial->setColor(Color::Red);
        m_sphere = SphereMesh::create();
        m_sphere->sphereMeshComponent()->setMaterial(sphereMaterial);
        m_sphere->setScale(5);

		auto reflector = Reflector::create();

	}

    virtual void onUpdate() override
    {
        m_sphere->setPosition(4, 3.0 * std::sin(Engine::time()), 0);

        m_box->setPosition(-4, 5, 0);
        m_box->setRotation(0, Engine::time(), 0);

    }
};

void Experiment_Reflector()
{
	App_Experiment_Reflector app;
	detail::ApplicationHelper::run(&app);
}




