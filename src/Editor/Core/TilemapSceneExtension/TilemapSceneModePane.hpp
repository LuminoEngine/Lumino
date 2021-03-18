#pragma once
#include "../App/ToolPanesArea.hpp"

namespace lna {
class TilemapSceneEditorModel;

class TilemapSceneModePane
    : public ln::EditorPane
{
public:
	static const int DisplayTilesX = 8;	// 横に 8 個を 1 行とするのは View 側の都合。Model 側としては、元テクスチャはそういうフォーマットかもしれないが、Tileset は単に id 受け取って tile 返すだけで、横の数は気にしない
	static const float DisplayTileSize;

    void init(TilemapSceneEditorModel* model);
    void setTileset(ln::Tileset* tileset);

protected:
	virtual void onSourcePropertyChanged(ln::UINotifyPropertyChangedEventArgs* e) override;
    virtual void onRender(ln::UIRenderingContext* context) override;
	virtual void onRoutedEvent(ln::UIEventArgs* e) override;

private:
	TilemapSceneEditorModel* m_model;
    Ref<ln::Tileset> m_tileset;
    Ref<ln::Material> m_material;
	ln::Size m_tileSize;
	ln::Vector2 m_displayTileScale;
};

} // namespace lna
