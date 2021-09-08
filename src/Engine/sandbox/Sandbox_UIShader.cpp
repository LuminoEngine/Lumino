
#include <LuminoEngine.hpp>
using namespace ln;

class App_Sandbox_UIShader : public Application
{
    Ref<Shader> m_shader;

    void onInit() override
    {
        Engine::renderView()->setBackgroundColor(Color::Gray);

        m_shader = Shader::load(_TT("C:/Proj/LN/Lumino/src/LuminoEngine/sandbox/Assets/Shader/UIShaderTest.fx"));
        auto texture1 = Texture2D::load(_TT("C:/Proj/LN/PrivateProjects/HC4/assets/Graphics/title1.png"));
        auto mask1 = Texture2D::load(_TT("C:/Proj/LN/PrivateProjects/HC4/assets/Graphics/title1-mask.png"));
        auto sprite1 = UISprite::create(texture1);
        sprite1->setBlendMode(BlendMode::Alpha);
        sprite1->setShader(m_shader);

        m_shader->findParameter(_TT("_MaskTexture"))->setTexture(mask1);

        UI::add(sprite1);
    }

    void onUpdate() override
    {
        float ti = Math::clamp(Engine::time() * 0.25 - 1.0, 0.0, 1.0);
        m_shader->findParameter(_TT("_Factor"))->setFloat(ti);


        const float vague = 0.2;
        float t = ti;
        float range = 1.0 + vague;
        t *= range;
        t -= (vague / 2.0);

        std::cout << ti <<  ": " << t << std::endl;
    }
};

void Sandbox_UIShader()
{
    App_Sandbox_UIShader app;
	detail::ApplicationHelper::run(&app);
}




