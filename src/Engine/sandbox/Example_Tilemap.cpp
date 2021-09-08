
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/Controls/UIComboBox.hpp>
using namespace ln;

class App_Example_Tilemap : public Application
{

    virtual void onInit() override
    {
		Engine::mainCamera()->addComponent(makeObject<CameraOrbitControlComponent>());


		auto tilesetTexture = ln::Texture2D::load(_TT("C:/Proj/LN/PrivateProjects/HC0/Assets/Tilesets/Tileset-1.png"));
		auto tilesetMaterial = ln::makeObject<ln::Material>();
		tilesetMaterial->setMainTexture(tilesetTexture);

		auto tileset = ln::makeObject<ln::Tileset>();
		tileset->reset(tilesetMaterial, 16, 16);

		auto autotileset = ln::makeObject<ln::AutoTileset>();
		auto autotilesetTexture = ln::Texture2D::load(_TT("C:/Proj/LN/PrivateProjects/HC0/Assets/Tilesets/AutoTileTest-1.png"));
		auto autotilesetMaterial = ln::Material::create(autotilesetTexture);
		autotileset->material = autotilesetMaterial;
		tileset->addAutoTileset(autotileset);

		auto tilemap = ln::makeObject<ln::Tilemap>();
		tilemap->setShadingModel(ln::ShadingModel::Unlit);
		tilemap->setBlendMode(ln::BlendMode::Alpha);	// TODO: AlphaTest でカバーしたい

		auto layer = ln::makeObject<ln::TilemapLayer>();
		layer->resize(60, 30);
		layer->setTileSize(ln::Size(1, 1));
		//for (auto x = 0; x < layer->getWidth(); x++) {
		//	layer->setTileId(x, layer->getHeight() - 1, 1);
		//}

		//layer->setTileId(0, layer->getHeight() - 1, 16384);
		//layer->setTileId(1, layer->getHeight() - 1, 16385);
		//layer->setTileId(2, layer->getHeight() - 1, 16386);
		//layer->setTileId(3, layer->getHeight() - 1, 16387);
		layer->putAutoTile(0, layer->getHeight() - 1, 0);
		layer->putAutoTile(1, layer->getHeight() - 1, 0);
		layer->putAutoTile(2, layer->getHeight() - 1, 0);
		layer->putAutoTile(3, layer->getHeight() - 1, 0);
		layer->putAutoTile(4, layer->getHeight() - 1, 0);
		layer->putAutoTile(1, layer->getHeight() - 2, 0);
		//layer->putAutoTileDirect(2, layer->getHeight() - 3, 0, 96);
		layer->putAutoTileSlope(2, layer->getHeight() - 2, 0);
		layer->putAutoTileSlope(3, layer->getHeight() - 2, 0);
		layer->putAutoTile(1, layer->getHeight() - 3, 0);

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
	detail::ApplicationHelper::run(&app);
}




