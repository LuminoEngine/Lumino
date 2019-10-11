#pragma once
#include "Common.h"

class ClothesEditor : public ln::AssetEditorViewModel
{
public:
    virtual void onOpened(ln::AssetModel* asset, ln::UIContainerElement* frame);
    virtual void onClosed();
};

class ClothesEditorPlugin : public ln::IDocumentEditorExtension
{
public:
    ClothesEditorPlugin();
    virtual const ln::Char* id() const { return u"8DEE452C-7AEE-40DC-8A7C-D92FE8EC8020"; }
    virtual const ln::Char* displayName() const { return u"ClothesEditorPlugin"; }
    virtual const ln::Char* typeKeyword() const { return u"TH10Clothes"; }
    virtual ln::Ref<ln::AssetEditorViewModel> createEditor() override;


private:
};

class TH10EditorExtension : public ln::IPluginModule
{
public:
    TH10EditorExtension();
    virtual int getEditorExtensionCount() { return 1; }
    virtual ln::IEditorExtension* getEditorExtension(int index) { return &m_clothesEditorPlugin; }

private:
    ClothesEditorPlugin m_clothesEditorPlugin;
};
