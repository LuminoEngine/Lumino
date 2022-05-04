
#include <filesystem>
#include <LuminoEngine.hpp>
#include <LuminoGraphics/Mesh/MeshModeEntity.hpp>
#include <LuminoEngine/UI/Controls/UIComboBox.hpp>
#include <LuminoEngine/UI/ImGuiIntegration.hpp>
#include <LuminoEngine/Editor/Editor.hpp>
using namespace ln;
namespace fs = std::filesystem;

class FileListPane : public ImGuiDockPane
{
public:
    List<Path> m_files;
    int m_selectedIndex = -1;
    Ref<Delegate<void(Path)>> selected;

    FileListPane()
    {
        setInitialPlacement(ImGuiDockPlacement::InnerLeft);
    
        std::filesystem::path dir = LN_GLTF_SAMPLE_MODELS_DIR "/2.0";
        for (const auto& x : fs::recursive_directory_iterator(dir)) {
            const auto glbdir = x.path() / "glTF-Binary";
            if (fs::exists(glbdir)) {
                for (const auto& y : fs::directory_iterator(glbdir)) {
                    std::cout << y.path() << std::endl;

                    m_files.add(String(y.path().u32string().c_str()));
                }
            }
        }
    }

    void onGui() override
    {
        for (int i = 0; i < m_files.size(); i++) {
            if (ImGui::Selectable(m_files[i].fileName().str().toStdString().c_str(), i == m_selectedIndex)) {
                m_selectedIndex = i;
                selected->call(m_files[i]);
            }
        }
    }
};


class MeshInfoPane : public ImGuiDockPane
{
public:
    MeshInfoPane()
        : m_meshInstance(nullptr)
    {
        setInitialPlacement(ImGuiDockPlacement::Right);
    }

    void setup(detail::MeshModelInstance* meshInstance)
    {
        m_meshInstance = meshInstance;
    }

    void onGui() override
    {
        if (m_meshInstance) {
            MeshModel* model = m_meshInstance->model();


            const auto& morphs = m_meshInstance->morphs();
            for (int iMorph = 0; iMorph < morphs.size(); iMorph++) {
                ImGui::Text("Morph[%d]", iMorph);
                const auto& morph = morphs[iMorph];
                const auto& meshContainer = model->meshContainers()[morph->meshContainerIndex()];
                const auto& meshPrimitive = meshContainer->meshPrimitive();
                for (int iMorphTarget = 0; iMorphTarget < meshPrimitive->morphTargetCount(); iMorphTarget++) {

                    char label[32];
                    sprintf(label, "Target[%d]", iMorphTarget);
                    ImGui::Text("Target[%d]", iMorphTarget);
                    float v = morph->weight(iMorphTarget);
                    ImGui::SliderFloat(label, &v, 0.0f, 1.0f, "%.3f");
                    morph->setWeight(iMorphTarget, v);
                }
            }
        }
    }

private:
    detail::MeshModelInstance* m_meshInstance;
};

class App_Example_glTFSampleViewer : public Application
{
public:
    App_Example_glTFSampleViewer()
    {
        EngineSettings::setMainWindowResizable(true);
        EngineSettings::setDevelopmentToolsEnabled(true);
    }

    Ref<FileListPane> m_pane;
    Ref<MeshInfoPane> m_meshInfoPane;
    Ref<SkinnedMesh> m_mesh;

    void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        //Engine::renderView()->setGizmoEnabled(true);
        Engine::mainCamera()->addComponent(CameraOrbitControlComponent::create());
        //Engine::renderView()->setBackgroundColor(Color::Gray);
        //Scene::setAntialiasEnabled(false);


        m_pane = makeObject<FileListPane>();
        m_pane->selected = makeDelegate(this, &App_Example_glTFSampleViewer::handleListItemSubmit);
        Editor::addPane(m_pane);
        Editor::editorViewportToolPane()->close();

        m_meshInfoPane = makeObject<MeshInfoPane>();
        Editor::addPane(m_meshInfoPane);

        //m_listbox1 = UIListBox::Builder()
        //    .width(200)
        //    .backgroundColor(Color::Gray)
        //    .onSubmit([&]() { handleListItemSubmit(m_listbox1->selectedItem()->dataAs<String>()); })
        //    .build();
        //m_listbox1->setAlignments(UIHAlignment::Left, UIVAlignment::Stretch);
        //m_listbox1->setSubmitMode(UIListSubmitMode::Single);
        //UI::add(m_listbox1);


    }

    void onUpdate() override
    {
    }

    void handleListItemSubmit(Path path)
    {
        if (m_mesh) {
            m_mesh->removeFromParent();
        }


        auto settings = makeObject<MeshImportSettings>();
        settings->setApplyBoneTransformationsEnabled(false);
        //m_mesh = StaticMesh::load(path, settings);
        m_mesh = SkinnedMesh::load(path);
        m_mesh->setBlendMode(BlendMode::Alpha);
        m_mesh->addInto();
        //m_mesh->setScale(10);


        const auto model = m_mesh->meshComponent()->model();
        const auto aabb = model->boundingBox();
        float modelRadius = Vector3::distance(aabb.max, aabb.min) * .5f;
        modelRadius = std::max(modelRadius, 0.5f);  // モデルが小さすぎるときに接近して near clip またがないように
        //const Vector3 eye = (aabb.min + aabb.max) * .5f + Vector3(modelRadius, modelRadius, -modelRadius);
        const Vector3 eye = (aabb.min + aabb.max) * .5f + Vector3(0, 0, modelRadius * 2.0f);
        Engine::mainCamera()->setPosition(eye);
        Engine::mainCamera()->lookAt(Vector3::Zero);


        m_meshInfoPane->setup(m_mesh->meshComponent()->modelInstance());
    }
};

void Example_glTFSampleViewer()
{
    App_Example_glTFSampleViewer app;
	detail::ApplicationHelper::run(&app);
}




