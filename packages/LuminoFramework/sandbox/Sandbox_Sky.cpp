
#include <LuminoFramework.hpp>
#include <LuminoEngine/Mesh/MeshProcessing.hpp>
#include <LuminoFramework/PostEffect/SSRPostEffect.hpp>
#include <LuminoFramework/PostEffect/SSAOPostEffect.hpp>
#include <LuminoFramework/PostEffect/TonemapPostEffect.hpp>
#include <LuminoFramework/PostEffect/DepthOfFieldPostEffect.hpp>
#include <LuminoFramework/PostEffect/FXAAPostEffect.hpp>
using namespace ln;

class App_Sandbox_Sky : public Application
{
    void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        Engine::mainCamera()->addComponent(CameraOrbitControlComponent::create());
        Engine::renderView()->setBackgroundColor(Color::Black);
        Engine::renderView()->setClearMode(SceneClearMode::SkyDome);
    }

    void onUpdate() override
    {
    }
};

void Sandbox_Sky()
{
    App_Sandbox_Sky app;
	detail::ApplicationHelper::run(&app);
}




