
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/UIComboBox.hpp>
using namespace ln;

class App_Example_SoundControl : public Application
{
	Ref<Sound> sound;

    virtual void onInit() override
    {
		sound = makeObject<Sound>(u"");
		sound->play();
    }

    virtual void onUpdate() override
    {
		auto size = Engine::mainWindow()->actualSize();

		if (Mouse::isPressed(MouseButtons::Left)) {
			float pitch = Mouse::position().x / size.width;
			sound->setPitch(pitch + 0.5);
		}
    }
};

void Example_SoundControl()
{
	App_Example_SoundControl app;
	detail::ApplicationHelper::run(&app);
}




