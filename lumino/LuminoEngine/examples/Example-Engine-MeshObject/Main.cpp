#include <LuminoEngine.hpp>
using namespace ln;

class App : public Application {
public:
    static void configure() {
        EngineSettings::setMainWindowTitle(U"HelloApp Example");
    }

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

LUMINO_APP(App);

int main() {
    ::LuminoConfigureApplication();
    auto* app = ::LuminoCreateApplicationInstance();
    AppIntegration::run(app);
}
