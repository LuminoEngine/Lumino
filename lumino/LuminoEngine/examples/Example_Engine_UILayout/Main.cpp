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

        ln::UIControl* rootDockNode = Engine::mainUIView();
        rootDockNode->style()->flexDirection = UILayoutFlexDirection::Column;

		auto toolbar = makeObject<UIElement>();
        toolbar->setName(U"toolbar");
        toolbar->style()->flexDirection = UILayoutFlexDirection::Column;
        toolbar->style()->width = UIStyleValue::makePercent(100);
        toolbar->style()->height = UIStyleValue(100);
        toolbar->style()->flexShrink = 0.0f;    // 自動縮小を許可しない
        toolbar->setBackgroundColor(UIColors::red());
        rootDockNode->addChild(toolbar);

        auto navbarDockNode = makeObject<UIControl>();
        navbarDockNode->setName(U"main");
        navbarDockNode->style()->flexDirection = UILayoutFlexDirection::Row;
        navbarDockNode->style()->height = UIStyleValue::makePercent(100);
        navbarDockNode->setBackgroundColor(UIColors::blue());
        rootDockNode->addChild(navbarDockNode);

        auto navbar = makeObject<UIControl>();
        navbar->setName(U"navbar");
        navbar->style()->width = UIStyleValue(200);
        navbar->style()->height = UIStyleValue::makePercent(100);
        navbar->setBackgroundColor(UIColors::green());
        navbarDockNode->addChild(navbar);

        auto logDockNode = makeObject<UIControl>();
        logDockNode->setName(U"logDockNode");
        logDockNode->style()->flexDirection = UILayoutFlexDirection::Column;
        logDockNode->style()->width = UIStyleValue::makePercent(100);
        logDockNode->style()->height = UIStyleValue::makePercent(100);
        logDockNode->setBackgroundColor(UIColors::orange());
        navbarDockNode->addChild(logDockNode);

        auto propsDockNode = makeObject<UIControl>();
        propsDockNode->setName(U"propsDockNode");
        propsDockNode->style()->flexDirection = UILayoutFlexDirection::Row;
        propsDockNode->style()->width = UIStyleValue::makePercent(100);
        propsDockNode->style()->height = UIStyleValue::makePercent(100);
        propsDockNode->setBackgroundColor(UIColors::cyan());
        logDockNode->addChild(propsDockNode);
        
        auto log = makeObject<UIControl>();
        log->setName(U"log");
        log->style()->flexDirection = UILayoutFlexDirection::Column;
        log->style()->width = UIStyleValue::makePercent(100);
        log->style()->height = UIStyleValue(100);
        log->setBackgroundColor(UIColors::brown());
        logDockNode->addChild(log);
        
  //      
		//auto element1 = makeObject<UIElement>();
  //      element1->setName(U"Test");
  //      element1->setSize(100, 200);
  //      element1->setBackgroundColor(Color::Red);
  //      root->addElement(element1);

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
