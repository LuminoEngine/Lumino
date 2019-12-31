
#include "TilemapSceneEditorModel.hpp"
#include "TilemapSceneModePane.hpp"

namespace lna {

//==============================================================================
// TilemapSceneModePane

const float TilemapSceneModePane::DisplayTileSize = 32.0f;

void TilemapSceneModePane::init(TilemapSceneEditorModel* model)
{
    EditorPane::init();
	setHorizontalAlignment(ln::HAlignment::Left);
	setVerticalAlignment(ln::VAlignment::Top);

	m_model = model;
	setViewModel(m_model);

    m_material = ln::makeObject<ln::Material>();
	m_displayScale = ln::Vector2(1, 1);
}

void TilemapSceneModePane::setTileset(ln::Tileset* tileset)
{
    m_tileset = tileset;

    auto texture = m_tileset->material()->mainTexture();
    if (texture) {
        m_material->setMainTexture(texture);

		m_tileSize.width = texture->width() / 8;
        m_tileSize.height = m_tileSize.width;
		m_displayScale.x = DisplayTileSize / m_tileSize.width;
		m_displayScale.y = DisplayTileSize / m_tileSize.height;

        setWidth(m_displayScale.x * texture->width());
        setHeight(m_displayScale.y * texture->height());
    }
}

void TilemapSceneModePane::onSourcePropertyChanged(ln::UINotifyPropertyChangedEventArgs* e)
{
	// とりあえず再描画
	invalidateVisual();
}

void TilemapSceneModePane::onRender(ln::UIRenderingContext* context)
{
    if (m_material && m_material->mainTexture()) {
        context->drawImage(ln::Rect(0, 0, m_displayScale.x * m_material->mainTexture()->width(), m_displayScale.x * m_material->mainTexture()->height()), m_material);

		if (m_model->tilemapBrush()->source() == TilemapBrushSource::Tileset) {
			auto range = m_model->tilemapBrush()->range();
			context->drawBoxBorderLine(ln::Rect(range.x * DisplayTileSize, range.y * DisplayTileSize, range.width * DisplayTileSize, range.height * DisplayTileSize), 2, ln::Color::Red, true);
		}
    }
}

void TilemapSceneModePane::onRoutedEvent(ln::UIEventArgs* e)
{
	if (e->type() == ln::UIEvents::MouseDownEvent) {
		auto me = static_cast<ln::UIMouseEventArgs*>(e);
		auto pt = me->getPosition(this);
		m_model->tilemapBrush()->setNormalTiles(m_tileset, ln::RectI(pt.x / DisplayTileSize, pt.y / DisplayTileSize, 1, 1));
		m_model->notify();
		e->handled = true;
		return;
	}
}

} // namespace lna
