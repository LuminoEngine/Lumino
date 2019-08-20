#pragma once

namespace lna {

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

private:
	Ref<TilemapBrush> m_tilemapBrush;
};

} // namespace lna 
