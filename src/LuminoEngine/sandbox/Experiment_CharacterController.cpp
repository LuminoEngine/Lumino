
#include <LuminoEngine.hpp>
#include <LuminoEngine/Scripting/Interpreter.hpp>
using namespace ln;

class App_Experiment_CharacterController : public Application
{
	

    void onInit() override
    {
		Engine::renderView()->setGuideGridEnabled(true);


		auto box = ln::makeObject<ln::BoxMesh>();
		//addComponent(box);

		auto controller = ln::makeObject<ln::CharacterController>();
		box->addComponent(controller);

	}

    void onUpdate() override
    {
    }

	bool onMessage(InterpreterCommandList* cmd)
	{

	}
};

void Experiment_CharacterController()
{
	App_Experiment_CharacterController app;
	detail::ApplicationHelper::run(&app);
}




