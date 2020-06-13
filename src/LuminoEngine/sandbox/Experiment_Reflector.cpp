
#include <LuminoEngine.hpp>
#include <LuminoEngine/Scene/Reflection/Reflector.hpp>
using namespace ln;

class App_Experiment_Reflector : public Application
{

    virtual void onInit() override
    {
		Engine::renderView()->setGuideGridEnabled(true);
		Engine::camera()->addComponent(CameraOrbitControlComponent::create());

		auto reflector = Reflector::create();

	}

    virtual void onUpdate() override
    {
    }
};

void Experiment_Reflector()
{
	App_Experiment_Reflector app;
	detail::ApplicationHelper::run(&app);
}




