
#include <filesystem>
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/Controls/UIComboBox.hpp>
using namespace ln;

class App_Example_glTFSampleViewer : public Application
{
    Ref<UIListBox> m_listbox1;

    void onInit() override
    {
        namespace fs = std::filesystem;

        m_listbox1 = UIListBox::create();
        m_listbox1->setBackgroundColor(Color::Gray);
        m_listbox1->setWidth(200);
        m_listbox1->setAlignments(UIHAlignment::Left, UIVAlignment::Stretch);
        UI::add(m_listbox1);


        std::filesystem::path dir = LN_GLTF_SAMPLE_MODELS_DIR "/2.0";
        for (const auto& x : fs::recursive_directory_iterator(dir)) {
            const auto glbdir = x / "glTF-Binary";
            if (fs::exists(glbdir)) {
                for (const auto& y : fs::directory_iterator(glbdir)) {
                    std::cout << y.path() << std::endl;

                    m_listbox1->addChild(y.path().u16string().c_str());
                }
            }
        }

    }

    void onUpdate() override
    {
    }
};

void Example_glTFSampleViewer()
{
    App_Example_glTFSampleViewer app;
	detail::ApplicationHelper::run(&app);
}




