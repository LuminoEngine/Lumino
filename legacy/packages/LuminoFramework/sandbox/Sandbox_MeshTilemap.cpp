
#include <LuminoFramework.hpp>
#include <LuminoFramework/UI/Controls/UIComboBox.hpp>
#include <LuminoFramework/Scene/MeshVoxelmap/MeshVoxelset.hpp>
#include <LuminoFramework/Scene/MeshVoxelmap/MeshVoxelmapLayer.hpp>
#include <LuminoFramework/Scene/MeshVoxelmap/MeshVoxelmapModel.hpp>
#include <LuminoFramework/Scene/MeshVoxelmap/MeshVoxelmapComponent.hpp>
using namespace ln;

#include <Lumino.hpp>

#define TEST_DTL 0

#if TEST_DTL
#include <DTL.hpp>
#endif

class Sandbox_MeshTilemap_App : public Application
{
	Ref<SphereMesh> s;

	virtual void onInit() override
	{
		Engine::renderView()->setGuideGridEnabled(true);
		Engine::mainCamera()->addComponent(CameraOrbitControlComponent::create());

		//auto tex = Texture2D::load(u"autotile1");
		////auto sprite = Sprite::create(tex);

		//auto mesh = makeObject_deprecated<Mesh>(4, 6);
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

		//auto meshContainer = makeObject_deprecated<MeshContainer>();
		//meshContainer->setMesh(mesh);

		//auto node = makeObject_deprecated<MeshNode>();
		//node->setMeshContainerIndex(0);

		//auto material = Material::create(tex);

		//auto meshModel = makeObject_deprecated<MeshModel>();
		//meshModel->addMeshContainer(meshContainer);
		//meshModel->addNode(node);
		//meshModel->addMaterial(material);
		//meshModel->updateNodeTransforms();

		//auto staticMesh = Mesh::create();
		//auto meshComponent = staticMesh->staticMeshComponent();
		//meshComponent->setModel(meshModel);
		//
		//staticMesh->setShadingModel(ShadingModel::Unlit);
		////auto mesh = Mesh::create(u"D:/Tech/Graphics/glTF-Sample-Models/2.0/Box/glTF/Box.gltf");

		auto meshTilemapModel = makeObject_deprecated<MeshVoxelmapModel>();

		auto meshTileset = makeObject_deprecated<MeshVoxelset>();
		meshTilemapModel->setTileset(meshTileset);

		// TODO: test
		auto layer = makeObject_deprecated<MeshVoxelmapLayer>();
		layer->resize(20, 3, 20);

		if (0) {
			layer->putAutoTile(0, 0, 0, 0);
			layer->putAutoTile(1, 1, 1, 0);
			layer->putAutoTile(2, 1, 1, 0);
			layer->putAutoTile(3, 1, 1, 0);
			layer->putAutoTile(4, 1, 1, 0);
			layer->putAutoTile(5, 1, 1, 0);
			layer->putAutoTile(2, 1, 2, 0);
			layer->putAutoTile(3, 1, 2, 0);
			layer->putAutoTile(4, 1, 2, 0);
			layer->putAutoTile(2, 1, 3, 0);
			layer->putAutoTile(4, 1, 3, 0);
			layer->putAutoTile(3, 2, 2, 0);

			layer->putAutoTile(5, 0, 0, 0); layer->putAutoTile(6, 0, 0, 0); layer->putAutoTile(7, 0, 0, 0);
			layer->putAutoTile(5, 0, 1, 0); layer->putAutoTile(6, 0, 1, 0); layer->putAutoTile(7, 0, 1, 0);
			layer->putAutoTile(5, 1, 0, 0); layer->putAutoTile(6, 1, 0, 0); layer->putAutoTile(7, 1, 0, 0);
			layer->putAutoTile(5, 1, 1, 0); layer->putAutoTile(6, 1, 1, 0); layer->putAutoTile(7, 1, 1, 0);
		}
		else {
			for (int z = 0; z < layer->sizeZ(); z++)
				for (int x = 0; x < layer->sizeX(); x++)
					layer->putAutoTile(x, 0, z, 1);
		}

		meshTilemapModel->addLayer(layer);

		auto meshTilemapComponent = makeObject_deprecated<MeshVoxelmapComponent>();
		meshTilemapComponent->setModel(meshTilemapModel);
		auto meshTilemap = makeObject_deprecated<VisualObject>();
		meshTilemap->addComponent(meshTilemapComponent);
		meshTilemapComponent->setShadingModel(ShadingModel::Unlit);


		//auto sp1 = UISprite::create(Texture2D::load(u"autotile2"));
		//sp1->setAlignments(UIHAlignment::Left, UIVAlignment::Top);


#if TEST_DTL
		constexpr std::size_t matrix_size_x{ 20 };
		constexpr std::size_t matrix_size_y{ 20 };
		constexpr std::size_t dungeon_pixel_size{ 2 };
		std::array<std::bitset<matrix_size_x>, matrix_size_y> matrix{ {} };
		dtl::shape::SimpleVoronoiIsland<bool>(100, 0.5, 1, 0).draw(matrix);
		//dtl::utility::CellularAutomation<bool>().draw(matrix);
		for (std::size_t row{}; row < matrix.size(); ++row)
			for (std::size_t col{}; col < matrix[row].size(); ++col)
				if (matrix[row][col]) {
					//layer->putAutoTile(row, 1, col, 0);
					//layer->putAutoTile(row, 2, col, 0);
					layer->putAutoTile(row, 1, col, 3);
				}
#endif
		//{
		//	layer->putAutoTile(1, 0, 0, 3);
		//	layer->putAutoTile(0, 0, 1, 3);
		//	layer->putAutoTile(1, 0, 1, 3);
		//	layer->putAutoTile(2, 0, 1, 3);
		//}
	}

	void onUpdate() override
	{
	}
};

void Sandbox_MeshTilemap()
{
	Sandbox_MeshTilemap_App app;
	//EngineSettings::setGraphicsDebugEnabled(true);
 //   EngineSettings::setDebugToolEnabled(false);
	//EngineSettings::setMainWindowSize(1600, 800);
	EngineSettings::setDebugToolEnabled(true);
    detail::ApplicationHelper::run(&app);
}




