
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/UIComboBox.hpp>
#include <LuminoEngine/Scene/MeshTilemap/MeshTilemapModel.hpp>
#include <LuminoEngine/Scene/MeshTilemap/MeshTilemapComponent.hpp>
using namespace ln;

#include <Lumino.hpp>


class App : public Application
{
	Ref<SphereMesh> s;

	virtual void onInit() override
	{
		Engine::renderView()->setGuideGridEnabled(true);
		Engine::camera()->addComponent(CameraOrbitControlComponent::create());

		//auto tex = Texture2D::load(u"autotile1");
		////auto sprite = Sprite::create(tex);

		//auto mesh = makeObject<Mesh>(4, 6);
		//mesh->setVertex(0, Vertex{ Vector3(0, 0.5, 0), Vector3::UnitZ, Vector2(0, 0), Color::White });
		//mesh->setVertex(1, Vertex{ Vector3(0.5, 0.5, 0), Vector3::UnitZ, Vector2(1, 0), Color::White });
		//mesh->setVertex(2, Vertex{ Vector3(0, 0, 0), Vector3::UnitZ, Vector2(0, 1), Color::White });
		//mesh->setVertex(3, Vertex{ Vector3(0.5, 0, 0), Vector3::UnitZ, Vector2(1, 1), Color::White });
		//mesh->setIndex(0, 0);
		//mesh->setIndex(1, 1);
		//mesh->setIndex(2, 2);
		//mesh->setIndex(3, 2);
		//mesh->setIndex(4, 1);
		//mesh->setIndex(5, 3);
		//mesh->addSection(0, 2, 0, PrimitiveTopology::TriangleList);

		//auto meshContainer = makeObject<MeshContainer>();
		//meshContainer->setMesh(mesh);

		//auto node = makeObject<MeshNode>();
		//node->setMeshContainerIndex(0);

		//auto material = Material::create(tex);

		//auto meshModel = makeObject<StaticMeshModel>();
		//meshModel->addMeshContainer(meshContainer);
		//meshModel->addNode(node);
		//meshModel->addMaterial(material);
		//meshModel->updateNodeTransforms();

		//auto staticMesh = StaticMesh::create();
		//auto meshComponent = staticMesh->staticMeshComponent();
		//meshComponent->setModel(meshModel);
		//
		//staticMesh->setShadingModel(ShadingModel::Unlit);
		////auto mesh = StaticMesh::create(u"D:/Tech/Graphics/glTF-Sample-Models/2.0/Box/glTF/Box.gltf");

		auto meshTilemapModel = makeObject<MeshTilemapModel>();

		auto meshTilemapComponent = makeObject<MeshTilemapComponent>();
		meshTilemapComponent->setModel(meshTilemapModel);
		auto meshTilemap = makeObject<VisualObject>();
		meshTilemap->addComponent(meshTilemapComponent);
	}

	void onUpdate() override
	{
	}
};

void Tutorial_Sandbox()
{
    App app;
	//EngineSettings::setGraphicsDebugEnabled(true);
 //   EngineSettings::setDebugToolEnabled(false);
	//EngineSettings::setMainWindowSize(1600, 800);
    detail::ApplicationHelper::run(&app);
}




