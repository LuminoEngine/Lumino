
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/UIComboBox.hpp>
using namespace ln;

class App_Example_GameAudio : public Application
{
    virtual void onInit() override
    {
		auto addButton = ln::UIButton::create(u"Play BGM");
		addButton->connectOnClicked([this]() {
			GameAudio::playBGM(u"D:/Music/momentum/02 - momentum.wav", 0.5, 1.0/*, 3.0*/);
		});
		Engine::mainUIView()->addChild(addButton);
    }

    virtual void onUpdate() override
    {
    }
};

void Example_GameAudio()
{
	App_Example_GameAudio app;
	detail::ApplicationHelper::run(&app);
}




