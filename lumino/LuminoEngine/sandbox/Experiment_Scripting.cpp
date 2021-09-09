
#include <LuminoEngine.hpp>
#include <LuminoEngine/Scripting/Interpreter.hpp>
using namespace ln;

class App_Experiment_Scripting : public Application
{
	

    void onInit() override
    {
		Engine::renderView()->setGuideGridEnabled(true);
		Engine::mainCamera()->addComponent(CameraOrbitControlComponent::create());



	}

    void onUpdate() override
    {
    }

	bool onMessage(InterpreterCommandList* cmd)
	{

	}
};

void Experiment_Scripting()
{
	App_Experiment_Scripting app;
	detail::ApplicationHelper::run(&app);
}




