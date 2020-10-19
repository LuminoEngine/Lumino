
#include <LuminoEngine.hpp>
using namespace ln;

class App_Sandbox_SkinnedMeshTest : public Application
{
public:
    Ref<SkinnedMesh> m_mesh;
    Ref<SkinnedMeshModel> m_model;
    Ref<AnimationState> m_idle;

    App_Sandbox_SkinnedMeshTest()
    {
    }

    virtual void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        Engine::camera()->addComponent(CameraOrbitControlComponent::create());
        Engine::renderView()->setBackgroundColor(Color::Gray);

        m_mesh = SkinnedMesh::load(u"C:/Proj/LN/Lumino/src/LuminoEngine/test/Assets/Mesh/SkinnedAxis1.glb");
        m_model = m_mesh->skinnedMeshComponent()->model();

        auto clip = AnimationClip::load(u"C:/Proj/LN/Lumino/src/LuminoEngine/test/Assets/Mesh/SkinnedAxis1.bvh");

        auto state = m_model->animationController()->addClip(clip);
        m_model->animationController()->play(state);

        for (auto& node : m_model->m_nodes) {
            std::cout << node->name() << std::endl;
            std::cout << node->initialLocalTransform().position().y << std::endl;
        }
    }

    virtual void onUpdate() override
    {
    }
};

void Sandbox_SkinnedMeshTest()
{
    App_Sandbox_SkinnedMeshTest app;
	detail::ApplicationHelper::run(&app);
}




