﻿
#include <LuminoEngine.hpp>
#include <LuminoEngine/Tilemap/Voxel.hpp>
using namespace ln;


class App_Experiment_SerializeLevel : public Application
{
    class TestLevel : public Level
    {

    };

    void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        Engine::camera()->addComponent(CameraOrbitControlComponent::create());

        if (0)
        {
            Engine::setActiveWorld(nullptr);

            auto level1 = makeObject<Level>();

            auto model1 = MeshModel::load(u"Models/Box1.gltf");
            //Assets::saveAsset(model1, u"Models/Box1");


            auto mesh1 = StaticMesh::create(model1);


            level1->addObject(mesh1);
            level1->acquireRenderParameters()->m_fogColor = Color::Gray;

            Assets::saveAsset(level1, u"Level1");
        }
        else
        {
            auto level1 = makeObject<TestLevel>();
            Assets::reloadAsset(u"Level1", level1);

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




