
#include <LuminoFramework.hpp>
#include <LuminoFramework/Tilemap/Voxel.hpp>
using namespace ln;


class App_Experiment_SerializeLevel : public Application
{
    class TestLevel : public Level
    {

    };

    void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        Engine::mainCamera()->addComponent(CameraOrbitControlComponent::create());

        if (0)
        {
            Engine::setActiveWorld(nullptr);

            auto level1 = makeObject_deprecated<Level>();

            auto model1 = MeshModel::load(_TT("Models/Box1.gltf"));
            //Assets::saveAsset(model1, _TT("Models/Box1"));


            auto mesh1 = StaticMesh::create(model1);


            level1->addObject(mesh1);
            level1->acquireRenderParameters()->m_fogColor = Color::Gray;

            Assets::saveAsset(level1, _TT("Level1"));
        }
        else
        {
            auto level1 = makeObject_deprecated<TestLevel>();
            Assets::reloadAsset(_TT("Level1"), level1);

            Scene::gotoLevel(level1);
        }
	}

    void onUpdate() override
    {
    }
};

void Experiment_SerializeLevel()
{
	App_Experiment_SerializeLevel app;
	detail::ApplicationHelper::run(&app);
}




