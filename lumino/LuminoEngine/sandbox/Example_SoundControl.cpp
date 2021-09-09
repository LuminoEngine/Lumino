
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/Controls/UIComboBox.hpp>
using namespace ln;

class App_Example_SoundControl : public Application
{
	Ref<Sound> sound;
	Ref<Sound> se;

    virtual void onInit() override
    {
		sound = makeObject<Sound>(_TT("D:/Music/momentum/02 - momentum.wav"));
		//sound->setVolume(0.05);
		sound->play();
		//sound = nullptr;

		se = makeObject<Sound>(_TT("C:/Proj/LN/Lumino/src/LuminoEngine/sandbox/Assets/Audio/ln_cursor_1.wav"));
		


		auto addButton = ln::UIButton::create(_TT("Add"));
		addButton->connectOnClicked([this]() {
			se->play();
		});
		Engine::mainUIView()->addChild(addButton);
    }

    virtual void onUpdate() override
    {
		auto size = Engine::mainWindow()->actualSize();

		if (Mouse::pressed(MouseButtons::Left)) {
			float volume = Mouse::position().y / size.height;
			float pitch = Mouse::position().x / size.width;
			sound->setVolume(volume);
			sound->setPitch(pitch + 0.5);
		}
    }
};

void Example_SoundControl()
{
	App_Example_SoundControl app;
	detail::ApplicationHelper::run(&app);
}




