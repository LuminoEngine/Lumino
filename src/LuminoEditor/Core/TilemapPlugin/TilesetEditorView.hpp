#pragma once
#include <LuminoEditor/Plugin.hpp>
#include "../App/NavigatorManager.hpp"
#include "../AssetEditor/AssetEditorModel.hpp"

namespace lna {
class TilesetView;

class AutoTilesetControl
	: public ln::UIControl
{
public:
	void setTileset(ln::Tileset* tileset);

protected:
	virtual void onRender(ln::UIRenderingContext* context) override;

private:
	Ref<ln::Tileset> m_tileset;
};

class TilesetControl
    : public ln::UIControl
{
public:
	TilesetControl();
    void setTileset(ln::Tileset* tileset);

protected:
    virtual void onRender(ln::UIRenderingContext* context) override;

private:
    Ref<ln::Tileset> m_tileset;
	float m_displayTileSize;
	float m_displayTileCountH;
	ln::Vector2 m_displayTileScale;
};

class TilesetFormControl
	: public ln::UIControl
{
public:
	TilesetFormControl(TilesetView* parent);

protected:

private:
};

class TilesetView
	: public ln::UIControl
{
public:
	TilesetView();

	void setTileset(ln::Tileset* tileset);

	//Ref<ln::UIVBoxLayout3> layout();

protected:

private:
	Ref<ln::UIVBoxLayout3> m_layout;
	Ref<TilesetFormControl> m_tilesetFormControl;
	Ref<AutoTilesetControl> m_autoTilesetControl;
	Ref<TilesetControl> m_tilesetControl;
	Ref<ln::Tileset> m_tileset;
};

} // namespace lna
