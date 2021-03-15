#pragma once
#include <LuminoEditor/Plugin.hpp>
#include "../App/NavigatorManager.hpp"
#include "../AssetEditor/AssetEditorModel.hpp"

namespace lna {
class TilesetView;


class DialogHostButton : public ln::UIButton
{
public:
	DialogHostButton(ln::UIDialog* dialog);

protected:
	virtual void onClick(ln::UIEventArgs* e) override;

private:
	Ref<ln::UIDialog> m_dialog;
};

class TextureAssetSelectDialog : public ln::UIDialog
{
public:
	TextureAssetSelectDialog();

protected:

private:
};



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
	Ref<ln::UIHBoxLayout3> m_layout1;
	Ref<ln::UIVBoxLayout3> m_layout2;
	Ref<TilesetFormControl> m_tilesetFormControl;
	Ref<AutoTilesetControl> m_autoTilesetControl;
	Ref<TilesetControl> m_tilesetControl;
	Ref<ln::Tileset> m_tileset;
};

} // namespace lna
