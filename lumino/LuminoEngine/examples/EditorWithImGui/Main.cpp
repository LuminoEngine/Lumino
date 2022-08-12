#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/ImGuiIntegration.hpp>
#include <LuminoEngine/Engine/ApplicationRunner.hpp>
using namespace ln;

class TestDockPane : public ImGuiDockPane {
public:
    TestDockPane() {
        setInitialPlacement(ImGuiDockPlacement::Left);
    }

    void onGui() override {
        ImGui::Text("TestDockPane."); 
    }

private:
};

class TestMenuBar : public ImGuiMenuBar {
    void onImGui() override {
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New")) {
                    printf("");
				}
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
    }
};

class MainWindow : public UIMainWindow {
public:
    void onInit() override {
        setImGuiLayerEnabled(true);

        m_menuBar = makeObject_deprecated<TestMenuBar>();
        dockManager()->addMenuBar(m_menuBar);
		
        m_pane = makeObject_deprecated<TestDockPane>();
        dockManager()->addDockPane(m_pane);
    }

    //void onImGui() override {
    //    {
    //        static float f = 0.0f;
    //        static int counter = 0;

    //        ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

    //        ImGui::Text("This is some useful text.");          // Display some text (you can use a format strings too)
    //        //ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
    //        //ImGui::Checkbox("Another Window", &show_another_window);

    //        //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    //        //ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

    //        if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
    //            counter++;
    //        ImGui::SameLine();
    //        ImGui::Text("counter = %d", counter);

    //        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    //        ImGui::End();
    //    }
    //}

private:
    Ref<TestMenuBar> m_menuBar;
    Ref<TestDockPane> m_pane;
};

class App : public Application {
public:
    //void onSetup(ApplicationSetupSettings* settings) override {
    //    settings->setMainWindow(makeObject_deprecated<MainWindow>(false));
    //}

    void onInit() override {
    }

    void onUpdate() override {
    }
};

LUMINO_APP(App);

int main() {
    StandaloneApplicationRunner runner;
    runner.run(LuminoConfigureApplication, LuminoCreateApplicationInstance);
}
