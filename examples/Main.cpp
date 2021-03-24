
#define LN_MSVC_DISABLE_LIBRARY_LINK
#include <LuminoEngine.hpp>
#include <LuminoCore/Testing/TestHelper.hpp>
#include <LuminoEngine/UI/ImGuiIntegration.hpp>
#include <LuminoEngine/Editor/Editor.hpp>
#include "../src/Engine/src/Engine/EngineManager.hpp"
#include "../src/Engine/src/Engine/EngineDomain.hpp"
using namespace ln;

Example CreateApp_App_Tutorial_Sandbox();
Example CreateApp_HelloExample();
Example CreateApp_ObjectClassBasicExample();
Example CreateApp_App_Sprite();
Example CreateApp_App_ShapeEntity();



struct ExampleEntry
{
    std::string name;
    std::function<Example()> app;
};

struct ExampleGroup
{
    std::string name;
    std::vector<ExampleEntry> items;
};

std::vector<ExampleGroup> s_examples = {
    {
        "Basic",
        {
            { "Hello", CreateApp_HelloExample },
            { "ObjectClass", CreateApp_ObjectClassBasicExample },
        }
    },
    {
        "Features",
        {
            { "Sprite", CreateApp_App_Sprite },
            { "ShapeEntity", CreateApp_App_ShapeEntity },
        }
    },
};

String g_appName;

#if 0
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
                .onSubmit([&, this]() { startApp(e.app().app); })
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
#endif


class ExampleListPane
    : public ImGuiDockPane
{
public:

protected:
    void onGui() override
    {
        const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
        const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

        static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;
        char textId[64];

        if (ImGui::BeginTable("3ways", 3, flags)) {
            // The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
            ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
            ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 18.0f);
            ImGui::TableHeadersRow();


            for (int iGroup = 0; iGroup < s_examples.size(); iGroup++) {
                const auto& group = s_examples[iGroup];
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                bool open = ImGui::TreeNodeEx(group.name.c_str(), ImGuiTreeNodeFlags_SpanFullWidth);
                ImGui::TableNextColumn();
                ImGui::TextDisabled("--");
                ImGui::TableNextColumn();
                ImGui::TextUnformatted("test");
                if (open) {
                    for (int iItem = 0; iItem < group.items.size(); iItem++) {
                        const auto& item = group.items[iItem];
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::TreeNodeEx(item.name.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
                        ImGui::TableNextColumn();
                        //ImGui::Text("%d", 10);
                        sprintf(textId, "C++##%s", item.name.c_str());
                        if (ImGui::Button(textId)) {
                            printf("on %s\n", item.name.c_str());
                            Process proc;
                            proc.setUseShellExecute(true);
                            proc.setProgram(g_appName);
                            proc.setArguments({ String::fromStdString(item.name) });
                            proc.start();
                        }
                        ImGui::TableNextColumn();
                        ImGui::TextUnformatted("test2");
                    }
                    ImGui::TreePop();
                }



            }


            //// Simple storage to output a dummy file-system.
            //struct MyTreeNode
            //{
            //    const char* Name;
            //    const char* Type;
            //    int             Size;
            //    int             ChildIdx;
            //    int             ChildCount;
            //    static void DisplayNode(const MyTreeNode* node, const MyTreeNode* all_nodes)
            //    {
            //        ImGui::TableNextRow();
            //        ImGui::TableNextColumn();
            //        const bool is_folder = (node->ChildCount > 0);
            //        if (is_folder)
            //        {
            //            bool open = ImGui::TreeNodeEx(node->Name, ImGuiTreeNodeFlags_SpanFullWidth);
            //            ImGui::TableNextColumn();
            //            ImGui::TextDisabled("--");
            //            ImGui::TableNextColumn();
            //            ImGui::TextUnformatted(node->Type);
            //            if (open)
            //            {
            //                for (int child_n = 0; child_n < node->ChildCount; child_n++)
            //                    DisplayNode(&all_nodes[node->ChildIdx + child_n], all_nodes);
            //                ImGui::TreePop();
            //            }
            //        }
            //        else
            //        {
            //            ImGui::TreeNodeEx(node->Name, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
            //            ImGui::TableNextColumn();
            //            ImGui::Text("%d", node->Size);
            //            ImGui::TableNextColumn();
            //            ImGui::TextUnformatted(node->Type);
            //        }
            //    }
            //};
            //static const MyTreeNode nodes[] =
            //{
            //    { "Root",                         "Folder",       -1,       1, 3    }, // 0
            //    { "Music",                        "Folder",       -1,       4, 2    }, // 1
            //    { "Textures",                     "Folder",       -1,       6, 3    }, // 2
            //    { "desktop.ini",                  "System file",  1024,    -1,-1    }, // 3
            //    { "File1_a.wav",                  "Audio file",   123000,  -1,-1    }, // 4
            //    { "File1_b.wav",                  "Audio file",   456000,  -1,-1    }, // 5
            //    { "Image001.png",                 "Image file",   203128,  -1,-1    }, // 6
            //    { "Copy of Image001.png",         "Image file",   203256,  -1,-1    }, // 7
            //    { "Copy of Image001 (Final2).png","Image file",   203512,  -1,-1    }, // 8
            //};

            //MyTreeNode::DisplayNode(&nodes[0], nodes);

            ImGui::EndTable();
        }
    }

private:
};

int main(int argc, char** argv)
{
	setlocale(LC_ALL, "");
    g_appName = String::fromCString(argv[0]);

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



    if (argc == 2) {
        const std::string exampleName = argv[1];

        Ref<Application> app;
        for (const auto& group : s_examples) {
            for (const auto& item : group.items) {
                if (item.name == exampleName) {
                    app = item.app().app();
                }
            }
        }
        
        if (app) {
            detail::ApplicationHelper::run(app);
        }
        else {
            return 1;
        }
    }
    else {
        EngineSettings::setDebugToolEnabled(true);
        EngineSettings::setDevelopmentToolsEnabled(true);
        detail::EngineManager::s_settings.standaloneFpsControl = true;
        //detail::EngineDomain::engineManager()->settings().createMainLights = true;



        Engine::initialize();

        Editor::closeAllPanes();

        auto pane = makeObject<ExampleListPane>();
        pane->setInitialPlacement(ImGuiDockPlacement::MainView);
        Editor::addPane(pane);

        //Navigator navigator;

        {
            //auto app = CreateApp_App_Tutorial_Sandbox();
            //detail::EngineDomain::engineManager()->resetApp(app);

            while (Engine::update()) {
                //navigator.update();
            }

            //detail::EngineDomain::engineManager()->resetApp(nullptr);
        }

        Engine::terminate();

        return 0;
    }


}

