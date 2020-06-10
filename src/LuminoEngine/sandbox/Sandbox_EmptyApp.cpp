
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/UIComboBox.hpp>
#include <LuminoEngine/ImageEffect/SSRImageEffect.hpp>
using namespace ln;

class App_Sandbox_EmptyApp : public Application
{
    void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        Engine::camera()->addComponent(CameraOrbitControlComponent::create());


		auto plane1 = PlaneMesh::create();
		auto plane1Material = Material::create();
		plane1Material->setRoughness(0.0f);
		plane1Material->setMetallic(1.0f);
		plane1->setRotation(-Math::PI / 2, -Math::PI / 4, 0);
		plane1->setPosition(-2, 0, 0);
		plane1->planeMeshComponent()->setMaterial(plane1Material);

		auto box1 = BoxMesh::create();
		auto box1Material = Material::create();
		box1Material->setColor(Color::Green);
		box1Material->setRoughness(1.0f);
		box1Material->setMetallic(0.0f);
		box1->boxMeshComponent()->setMaterial(box1Material);

		box1->setScale(2.5);
		box1->setPosition(2, 0, 0);

		auto ssrEffect = makeObject<SSRImageEffect>();
		Engine::renderView()->addImageEffect(ssrEffect);
    }

    void onUpdate() override
    {
    }
};

void Sandbox_EmptyApp()
{
    App_Sandbox_EmptyApp app;
	detail::ApplicationHelper::run(&app);
}




