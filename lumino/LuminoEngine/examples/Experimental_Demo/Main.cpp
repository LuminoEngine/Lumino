#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/Controls/UITreeBox.hpp>
#include <LuminoEngine/UI/ImGuiIntegration.hpp>
using namespace ln;

class WorldViewPane : public ImGuiDockPane {
public:
private:
};

class MainWindow : public ln::UIMainWindow {
};

class Level1 : public ln::Level {

    void onStart() override {
        printf("");
    }
    void onUpdate() override {
        printf("");
    }
};

class App : public ln::Application {
public:
    App() {

        m_boxMesh = BoxMesh::With().buildInto();

        ln::Engine::mainCamera()->setPosition(5, 5, 5);
        ln::Engine::mainCamera()->lookAt(0, 0, 0);
        ln::Engine::renderView()->setBackgroundColor(Color::Gray);

        // m_boxMesh->setShadingModel(ShadingModel::Unlit);
        // m_boxMesh->setColorScale(Color::Red);

        UIFrameWindow* mainWindow = Engine::mainWindow();
        mainWindow->setImGuiLayerEnabled(true);

        ImGuiDockManager* dockManager = mainWindow->dockManager();
        m_worldViewPane = *makeObject<WorldViewPane>();
        //m_worldViewPane->setInitialPlacement(ImGuiDockPlacement::DebugView);
        dockManager->addDockPane(m_worldViewPane);
		
		Scene::gotoLevel(*makeObject<Level1>());
    }

    void onUpdate() override {
    }

private:
    Ref<BoxMesh> m_boxMesh;
    Ref<WorldViewPane> m_worldViewPane;
};

int main() {
    ln::Engine::initialize();
    ln::Engine::setupMainWindow(*makeObject<MainWindow>());
    ln::Engine::setupApplication(*makeObject<App>());
    ln::Engine::run();
    ln::Engine::terminate();
}
