
#define LN_MSVC_DISABLE_LIBRARY_LINK
#include <LuminoEngine.hpp>
#include <LuminoCore/Testing/TestHelper.hpp>
#include "../src/Engine/EngineManager.hpp"
#include "../src/Engine/EngineDomain.hpp"
using namespace ln;

Ref<Application> CreateApp_App_Tutorial_Sandbox();
Ref<Application> CreateApp_App_Sprite();

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
	//EngineSettings::setUITheme(u"Chocotelier");
    EngineSettings::setGraphicsDebugEnabled(true);
    EngineSettings::setDebugToolEnabled(true);
    //EngineSettings::setDevelopmentToolsEnabled(true);
	detail::EngineManager::s_settings.standaloneFpsControl = true;
	//detail::EngineDomain::engineManager()->settings().createMainLights = true;



    Engine::initialize();


    auto m_listbox1 = UIListBox::Builder()
        .width(200)
        .backgroundColor(Color::Gray)
        .onSubmit([&]() { /*handleListItemSubmit(m_listbox1->selectedItem()->dataAs<String>()); */})
        .buildInto(/*Engine::mainWindow()*/);
    m_listbox1->setAlignments(UIHAlignment::Left, UIVAlignment::Stretch);
    m_listbox1->setSubmitMode(UIListSubmitMode::Single);
    m_listbox1->addChild(UIListBoxItem::With()
        .text("Sprite")
        .onSubmit([]() { detail::EngineDomain::engineManager()->resetApp(CreateApp_App_Sprite()); })
        .build());
    m_listbox1->addChild(u"item2");
    m_listbox1->addChild(u"item3");
    m_listbox1->addChild(u"item4");

    Engine::renderView()->setBackgroundColor(Color::AliceBlue);

    {
        //auto app = CreateApp_App_Tutorial_Sandbox();
        //detail::EngineDomain::engineManager()->resetApp(app);

        while (Engine::update()) {

        }

        //detail::EngineDomain::engineManager()->resetApp(nullptr);
    }

    Engine::terminate();

	return 0;
}

