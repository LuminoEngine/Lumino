
#include <LuminoFramework.hpp>
#include <LuminoFramework/UI/Controls/UIComboBox.hpp>
using namespace ln;

class App_Example_Sprite : public Application
{
    virtual void onInit() override
    {
		auto texture = Texture2D::load(_TT("picture1.jpg"));
        auto sprite = Sprite::create(texture);
        sprite->addInto();

        //auto sprite = UISprite::create(texture);
        //sprite->setAlignments(UIHAlignment::Left, UIVAlignment::Top);
        //sprite->setPosition(100, 50);
        //Engine::ui()->add(sprite);
    }

    virtual void onUpdate() override
    {
    }
};

void Example_Sprite()
{
	App_Example_Sprite app;
	detail::ApplicationHelper::run(&app);
}




