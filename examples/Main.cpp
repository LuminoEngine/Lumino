
#define LN_MSVC_DISABLE_LIBRARY_LINK
#include <LuminoEngine.hpp>
#include <LuminoCore/Testing/TestHelper.hpp>
#include "../src/Engine/src/Engine/EngineManager.hpp"
#include "../src/Engine/src/Engine/EngineDomain.hpp"
using namespace ln;

Ref<Application> CreateApp_App_Tutorial_Sandbox();
Ref<Application> CreateApp_App_Sprite();
Ref<Application> CreateApp_App_ShapeEntity();

struct ExampleEntry
{
    String name;
    std::function<Ref<Application>()> app;
};

List<ExampleEntry> s_examples = {
    { u"Sprite", CreateApp_App_Sprite },
    { u"ShapeEntity", CreateApp_App_ShapeEntity },
};

class Navigator
{
public:
    Navigator()
    {
        m_list = UIListBox::Builder()
            .width(200)
            .backgroundColor(Color::Gray)
            .buildInto(Engine::mainWindow());
        m_list->setAlignments(UIHAlignment::Left, UIVAlignment::Stretch);
        m_list->setSubmitMode(UIListSubmitMode::Single);

        for (const auto& e : s_examples) {
            m_list->addChild(UIListBoxItem::With()
                .text(e.name)
                .onSubmit([&, this]() { startApp(e.app()); })
                .build());
        }

        m_button = UIButton::With()
            .text(u"<<")
            .width(40)
            .alignment(UIHAlignment::Left, UIVAlignment::Top)
            .buildInto(Engine::mainWindow());
        m_button->connectOnClicked([this]() { showList(); });

        m_alpha.setEasingFunction(EasingFunctions::easeOutExpo);
        m_alpha.start(1.0f, 0.5);
    }

    void update()
    {
        m_alpha.advanceTime(0.016f);
        m_list->setOpacity(m_alpha.value());

        m_list->setPosition(m_alpha.value() * 220 - 220, 0.0f);

        float a = 1.0f - m_alpha.value();
        m_button->setPosition(a * 100 - 100, 0.0f);
        m_button->setOpacity(a);
    }

    void startApp(Ref<Application> app)
    {
        detail::EngineDomain::engineManager()->resetApp(app);
        m_alpha.start(0.0f, 0.5);
    }

    void showList()
    {
        m_alpha.start(1.0f, 0.5);
    }

private:
    Ref<UIListBox> m_list;
    Ref<UIButton> m_button;
    EasingValue<float> m_alpha;
};

int main(int argc, char** argv)
{
	setlocale(LC_ALL, "");

#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	Logger::addStdErrAdapter();
    Logger::setLevel(LogLevel::Debug);
	EngineSettings::setEngineFeatures(EngineFeature::Experimental);// EngineFeature::Public);// 
	EngineSettings::setGraphicsAPI(GraphicsAPI::Vulkan);//GraphicsAPI::DirectX12);//(GraphicsAPI::OpenGL);//
    EngineSettings::setPriorityGPUName(u"Microsoft Basic Render Driver");
	EngineSettings::addAssetDirectory(LN_LOCALFILE("Assets"));
	EngineSettings::setUITheme(u"Chocotelier");
    EngineSettings::setGraphicsDebugEnabled(true);
    EngineSettings::setDebugToolEnabled(true);
    //EngineSettings::setDevelopmentToolsEnabled(true);
	detail::EngineManager::s_settings.standaloneFpsControl = true;
	//detail::EngineDomain::engineManager()->settings().createMainLights = true;



    Engine::initialize();

    Navigator navigator;

    {
        //auto app = CreateApp_App_Tutorial_Sandbox();
        //detail::EngineDomain::engineManager()->resetApp(app);

        while (Engine::update()) {
            navigator.update();
        }

        //detail::EngineDomain::engineManager()->resetApp(nullptr);
    }

    Engine::terminate();

	return 0;
}

