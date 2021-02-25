
#include <filesystem>
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/Controls/UIComboBox.hpp>
using namespace ln;

class App_Example_glTFSampleViewer : public Application
{
    Ref<UIListBox> m_listbox1;
    Ref<StaticMesh> m_mesh;

    void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        Engine::renderView()->setGizmoEnabled(true);
        Engine::mainCamera()->addComponent(CameraOrbitControlComponent::create());
        //Engine::renderView()->setBackgroundColor(Color::Gray);
        //Scene::setAntialiasEnabled(false);

        namespace fs = std::filesystem;

        //m_listbox1 = UIListBox::create();
        //m_listbox1->setBackgroundColor(Color::Gray);
        //m_listbox1->setWidth(200);
        m_listbox1 = UIListBox::Builder()
            .width(200)
            .backgroundColor(Color::Gray)
            .onSubmit([&]() { handleListItemSubmit(m_listbox1->selectedItem()->dataAs<String>()); })
            .build();
        m_listbox1->setAlignments(UIHAlignment::Left, UIVAlignment::Stretch);
        m_listbox1->setSubmitMode(UIListSubmitMode::Single);
        UI::add(m_listbox1);


        std::filesystem::path dir = LN_GLTF_SAMPLE_MODELS_DIR "/2.0";
        for (const auto& x : fs::recursive_directory_iterator(dir)) {
            const auto glbdir = x.path() / "glTF-Binary";
            if (fs::exists(glbdir)) {
                for (const auto& y : fs::directory_iterator(glbdir)) {
                    std::cout << y.path() << std::endl;

                    m_listbox1->addItem(y.path().filename().u16string().c_str(), String(y.path().u16string().c_str()));

                }
            }
        }
    }

    void onUpdate() override
    {
    }

    void handleListItemSubmit(const String& path)
    {
        if (m_mesh) {
            m_mesh->removeFromParent();
        }


        auto settings = makeObject<MeshImportSettings>();
        settings->setApplyBoneTransformationsEnabled(false);
        m_mesh = StaticMesh::load(path, settings);
        m_mesh->setBlendMode(BlendMode::Alpha);
        m_mesh->addInto();

        std::cout << path << std::endl;
    }
};

void Example_glTFSampleViewer()
{
    App_Example_glTFSampleViewer app;
	detail::ApplicationHelper::run(&app);
}




