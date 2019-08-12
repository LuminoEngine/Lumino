#pragma once

namespace ln { class AssetEditorViewModel; }
class StartupView;

// Tab-Document だけではなく、Inspector や AssetBrowerToolPane など、複数の View から参照・編集されるデータを扱うクラス。
// 様々な View の中央に位置して各 View とメッセージをやり取りしたりする。
// また、一般的な MVC とおそらく異なるのは、View の生成も担当すること。
// 一般的には DocumentManager が Document の型情報を元に View を作るのが良いのだろうけれど、
// ActiveDocument が切り替わったときに、TabView だけではなく ToolPane, InspectorPane の内容も Document に合わせて変更したり、
// Scene 内の WorldObject の型に合わせて Insector の内容を変えたりと、全てを DocumentManager で賄うのが大変。
class Document
	: public ln::Object
{
public:
	Document();


protected:

private:
};

class DocumentManager
    : public ln::UIControl
{
public:
	DocumentManager();
    ln::Result init();

    void addDocument(Document* doc);

protected:

private:
    Ref<StartupView> m_startupView;
    ln::List<Ref<Document>> m_documents;
};

class AssetEditorDocument
    : public Document
{
public:
    AssetEditorDocument();
    ln::Result init(ln::AssetModel* asset, ln::AssetEditorViewModel* editorModel);

protected:

private:
    Ref<ln::AssetModel> m_asset;
    Ref<ln::AssetEditorViewModel> m_editor;
};
