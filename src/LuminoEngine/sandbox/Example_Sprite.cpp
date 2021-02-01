
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/Controls/UIComboBox.hpp>
using namespace ln;

class App_Example_Sprite : public Application
{
    virtual void onInit() override
    {
		auto texture = Texture2D::load(u"picture1.jpg");
		auto sprite = Sprite::create(texture);
        sprite->setSize(2, 1);
        sprite->addInto();
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




