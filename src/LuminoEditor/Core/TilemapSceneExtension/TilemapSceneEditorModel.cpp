
#include <Workspace.hpp>
#include <Project.hpp>
#include <AssetDatabase.hpp>
#include <PluginManager.hpp>
#include "TilemapSceneEditorModel.hpp"
#include "TilemapSceneModePane.hpp"

namespace lna {

//==============================================================================
// TilemapBrush

void TilemapBrush::setNormalTiles(ln::Tileset* tileset, const ln::RectI& range)
{
	if (LN_REQUIRE(tileset)) return;

	int beginX = range.x;
	int beginY = range.y;
	int endX = range.x + range.width;
	int endY = range.y + range.height;

	m_tiles.clear();
	for (int y = beginY; y < endY; y++) {
		for (int x = beginX; x < endX; x++) {
			int id = y * TilemapSceneModePane::DisplayTilesX + x;
			m_tiles.push_back(id);
		}
	}

	m_source = TilemapBrushSource::Tileset;
	m_range.x = beginX;
	m_range.y = beginY;
	m_range.width = endX - beginX;
	m_range.height = endY - beginY;
}

//==============================================================================
// TilemapSceneEditorModel

TilemapSceneEditorModel::TilemapSceneEditorModel()
	: m_tilemapBrush(ln::makeObject<TilemapBrush>())
{
}

} // namespace lna

