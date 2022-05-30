#include <LuminoEngine.hpp>
#include <LuminoEngine/Engine/ApplicationRunner.hpp>
using namespace ln;

class App : public Application {
public:
    static void configure() {
        EngineSettings::setMainWindowTitle(U"HelloApp Example");
        EngineSettings::setGraphicsDebugEnabled(true);
    }

    App() {
        ln::Engine::mainCamera()->setPosition(5, 5, 5);
        ln::Engine::mainCamera()->lookAt(0, 0, 0);
        ln::Engine::renderView()->setBackgroundColor(Color::Gray);

        
		auto element1 = makeObject<UIElement>();
        element1->setName(U"Test");
        element1->setSize(100, 200);
        element1->setBackgroundColor(Color::Red);
        Engine::mainUIView()->addElement(element1);

        //m_boxMesh->setShadingModel(ShadingModel::Unlit);
        //m_boxMesh->setColorScale(Color::Red);
    }

    void onUpdate() override {
    }

private:
};

LUMINO_APP(App);

int main() {
    StandaloneApplicationRunner runner;
    runner.run(LuminoConfigureApplication, LuminoCreateApplicationInstance);
}
