
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/Controls/UIComboBox.hpp>
#include <LuminoEngine/PostEffect/SSRPostEffect.hpp>
using namespace ln;

class App_Sandbox_EmptyApp : public Application
{
    void onInit() override
    {
        auto texture = Texture2D::load(u"picture1.jpg");
        //auto sprite = Sprite::With(texture).buildInto();
        auto box = BoxMesh::With()
            .size(1, 1, 1)
            .buildInto();
    }
};

void Sandbox_EmptyApp()
{
    App_Sandbox_EmptyApp app;
	detail::ApplicationHelper::run(&app);
}




