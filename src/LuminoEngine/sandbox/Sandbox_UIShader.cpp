
#include <LuminoEngine.hpp>
using namespace ln;

class App_Sandbox_UIShader : public Application
{
    void onInit() override
    {
        auto shader1 = Shader::load(u"C:/Proj/LN/Lumino/src/LuminoEngine/sandbox/Assets/Shader/UIShaderTest.fx");
        auto texture1 = Texture2D::load(u"C:/Proj/LN/PrivateProjects/HC4/assets/rect1969-5-3.png");
        auto sprite1 = UISprite::create(texture1);
        //sprite1->setShader(shader1);


        UI::add(sprite1);
    }

    void onUpdate() override
    {
    }
};

void Sandbox_UIShader()
{
    App_Sandbox_UIShader app;
	detail::ApplicationHelper::run(&app);
}




