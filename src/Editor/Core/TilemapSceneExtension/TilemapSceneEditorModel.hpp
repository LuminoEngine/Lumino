#pragma once

namespace lna {
class EditorContext;

enum class TilemapBrushSource
{
	Tileset,
	Tilemap,
};

class TilemapBrush
	: public ln::Object
{
public:
	///** TilemapBrushSource */
	TilemapBrushSource source() const { return m_source; }
	//void setSource(TilemapBrushSource value) { m_source = value; notify("source"); }

	void setNormalTiles(ln::Tileset* tileset, const ln::RectI& range);

	const ln::RectI& range() const { return m_range; }
    int width() const { return m_range.width; }
    int height() const { return m_range.height; }
    const std::vector<int>& tiles() const { return m_tiles; }

private:
	TilemapBrushSource m_source = TilemapBrushSource::Tileset;
	ln::RectI m_range = ln::RectI(0, 0, 1, 1);
	std::vector<int> m_tiles = { 0 };
};

class TilemapSceneEditorModel
    : public ln::UIViewModel
{
public:
	TilemapSceneEditorModel();
	const Ref<TilemapBrush>& tilemapBrush() const { return m_tilemapBrush; }

	static ln::Result createNewTilemapSceneAsset(lna::EditorContext* context, const ln::Path& filePath);

private:
	Ref<TilemapBrush> m_tilemapBrush;
};

} // namespace lna 
