#include "EditorMain.h"

void ClothesEditor::onOpened(ln::AssetModel* asset, ln::UIContainerElement* frame)
{
}

void ClothesEditor::onClosed()
{
}

//==============================================================================
// ClothesEditorPlugin

ClothesEditorPlugin::ClothesEditorPlugin()
{

}

ln::Ref<ln::AssetEditorViewModel> ClothesEditorPlugin::createEditor()
{
    return ln::makeObject<ClothesEditor>();
}

//==============================================================================
// TH10EditorExtension

TH10EditorExtension::TH10EditorExtension()
{
}

LUMINO_MODULE(TH10EditorExtension);
