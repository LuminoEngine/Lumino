#include <LuminoEngine.hpp>
#include <LuminoEngine/Engine/ApplicationRunner.hpp>
using namespace ln;

class MainWindow : public ln::UIMainWindow {
	
};

class App : public ln::Application {
public:
    App() {
	
        m_boxMesh = BoxMesh::With().buildInto();

        ln::Engine::mainCamera()->setPosition(5, 5, 5);
        ln::Engine::mainCamera()->lookAt(0, 0, 0);
        ln::Engine::renderView()->setBackgroundColor(Color::Gray);

        //m_boxMesh->setShadingModel(ShadingModel::Unlit);
        //m_boxMesh->setColorScale(Color::Red);
    }

    void onUpdate() override {
    }

private:
    Ref<BoxMesh> m_boxMesh;
};

int main() {
    ln::Engine::initialize();
    ln::Engine::setupMainWindow(*makeObject<MainWindow>());
    ln::Engine::setupApplication(*makeObject<App>());
    ln::Engine::run();
    ln::Engine::terminate();
}
