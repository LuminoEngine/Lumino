#include <LuminoEngine.hpp>
using namespace ln;

class App : public Application {
public:
    static void configure() {
        EngineSettings::setMainWindowTitle(U"HelloApp Example");
    }

    App() {

    }

    void onUpdate() override {
    }
};

LUMINO_APP(App);

int main() {
    ::LuminoConfigureApplication();
    auto* app = ::LuminoCreateApplicationInstance();
    AppIntegration::run(app);
}
