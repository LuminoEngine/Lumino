
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/Controls/UIComboBox.hpp>
using namespace ln;

class App_Example_GameAudio : public Application
{
	int m_count = 0;

    virtual void onInit() override
    {
		auto sprite = UISprite::load(_TT("picture1.jpg"));
		sprite->setAlignments(UIHAlignment::Left, UIVAlignment::Top);
		sprite->setPosition(100, 50);
		Engine::ui()->add(sprite);



		auto addButton = ln::UIButton::create(_TT("Play BGM"));
		addButton->connectOnClicked([this]() {
			Audio::playBGM(_TT("D:/Music/momentum/02 - momentum.wav"), 0.5, 1.0/*, 3.0*/);
			//Audio::playBGM(_TT("C:/Proj/LN/Lumino/src/LuminoEngine/test/Assets/Audio/sin_440_3s_S16L_48000_2ch.wav", 0.2);
		});
		auto button2 = ln::UIButton::create(_TT("Play BGS"));
		button2->connectOnClicked([this]() {
			Audio::playBGS(_TT("C:/Proj/LN/Lumino/src/LuminoEngine/sandbox/Assets/Audio/water02.wav"));
		});
		//Engine::mainUIView()->addChild(addButton);

		addButton->setAlignments(UIHAlignment::Left, UIVAlignment::Top);
		addButton->setPosition(0, 0);
    }

    virtual void onUpdate() override
    {
		m_count++;


		if (m_count > 60) {
			Audio::playSE(_TT("C:/Proj/LN/Lumino/src/LuminoEngine/sandbox/Assets/Audio/ln_cursor_1.wav"), 0.1);
			m_count = 0;
		}

    }
};

void Example_GameAudio()
{
	App_Example_GameAudio app;
	detail::ApplicationHelper::run(&app);
}




