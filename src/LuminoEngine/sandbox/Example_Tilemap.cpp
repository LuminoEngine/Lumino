
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/UIComboBox.hpp>
using namespace ln;

class App_Example_Tilemap : public Application
{

    virtual void onInit() override
    {
		Engine::mainCamera()->addComponent(makeObject<CameraOrbitControlComponent>());


		auto tilesetTexture = ln::Texture2D::load(u"C:/Proj/LN/PrivateProjects/HC0/Assets/Tilesets/Tileset-1.png");
		auto tilesetMaterial = ln::makeObject<ln::Material>();
		tilesetMaterial->setMainTexture(tilesetTexture);

		auto tileset = ln::makeObject<ln::Tileset>();
		tileset->reset(tilesetMaterial, 16, 16);


		auto tilemap = ln::makeObject<ln::Tilemap>();
		tilemap->setShadingModel(ln::ShadingModel::UnLighting);
		//scene->addObject(tilemap);

		auto layer = ln::makeObject<ln::TilemapLayer>();
		layer->resize(60, 30);
		layer->setTileSize(ln::Size(1, 1));
		layer->setTileId(0, 0, 1);
		layer->setTileId(1, 1, 1);
		for (auto x = 0; x < layer->getWidth(); x++) {
			layer->setTileId(x, layer->getHeight() - 1, 1);
		}
		auto tilemapModel = ln::makeObject<ln::TilemapModel>();
		tilemapModel->setTileset(tileset);
		tilemapModel->addLayer(layer);
		tilemap->setTilemapModel(tilemapModel);
		Engine::world()->add(tilemap);
    }

    virtual void onUpdate() override
    {
    }
};

void Example_Tilemap()
{
	App_Example_Tilemap app;
	detail::ApplicationHelper::init(&app);
	detail::ApplicationHelper::run(&app);
}




