#include <LuminoEngine.hpp>
using namespace ln;

class App : public Application {
public:
    App() {
        EngineSettings::setMainWindowResizable(true);
        EngineSettings::setDevelopmentToolsEnabled(true);
    }

    void onInit() override {
        Engine::renderView()->setGuideGridEnabled(true);
        Engine::mainCamera()->addComponent(CameraOrbitControlComponent::create());
    }

    void onUpdate() override {
    }
};

int main() {
    App app;
    detail::ApplicationHelper::run(&app);
}
