#pragma once
#include <LuminoEditor/Plugin.hpp>

class TilesetList
	: public ln::UIControl
{
public:
	void init();

private:
    void addButton_onClick(ln::UIEventArgs* e);
    void listView_onItemClick(ln::UIClickEventArgs* e);

    Ref<ln::UIFileSystemCollectionModel> m_model;
    Ref<ln::UIListView> m_listview;
    ln::Path m_assetRootDir;
};


class TilesetNavigatorExtension
    : public ln::Object
    , public ln::IAssetNavigatorExtension
{
public:
protected:
    virtual const ln::Char* id() const { return u"7BDAD7F8-E8A1-4B3A-9883-3700A6315A2A"; }
    virtual const ln::Char* displayName() const { return u"TilesetNavigatorExtension"; }
    virtual void onAttached() override;
    virtual void onDetached() override {}
    virtual ln::NavigationMenuItem* getNavigationMenuItem() override;
	virtual ln::UIElement* getNavigationPane() override;

private:
    Ref<ln::NavigationMenuItem> m_item;
	Ref<ln::UIElement> m_tilesetList;
};


class TilesetImporter
    : public ln::AssetImporter
{
protected:
    virtual void onGetSupportedExtensions(ln::List<ln::String>* outExtensions) {}
    virtual Ref<ln::AssetModel> onImport(const ln::Path& sourceFilePath) override;
};

class TilesetImporterExtension : public ln::IAssetImporterEditorExtension
{
public:
    virtual int matchFilePath(const ln::Path& filePath) override;
    virtual Ref<ln::AssetImporter> createImporter(const ln::Char* assetSourceFilePath) override;
};


class TilesetEditor : public ln::AssetEditor
{
public:
    virtual void onOpened(ln::AssetModel* asset, ln::UIContainerElement* frame) override;
    virtual void onClosed() override;
};

class TilesetEditorExtension
    : public ln::Object
    , public ln::IAssetEditorExtension
{
public:
    TilesetEditorExtension();
    virtual const ln::Char* id() const { return u"E27FD90C-AE45-45B4-BF53-B68FD144B611"; }
    virtual const ln::Char* displayName() const { return u"TilesetEditorExtension"; }
    virtual const ln::Char* typeKeyword() const { return u"Tileset"; }
    virtual ln::Ref<ln::AssetEditor> createEditor() override;

private:
};