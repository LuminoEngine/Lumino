#pragma once

namespace ln { class AssetEditor; }
class StartupView;
class ToolPanesArea;

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
    ln::Result init();

    const Ref<ln::UIContainerElement>& mainFrame() const { return m_mainFrame; }

protected:

private:
    Ref<ln::UIContainerElement> m_mainFrame;
};

class DocumentManager
    : public ln::UIControl
{
public:
	DocumentManager();
    ln::Result init();

    const Ref<ToolPanesArea>& modePanesArea() const { return m_modePanesArea; }
    const Ref<ToolPanesArea>& toolPanesArea() const { return m_toolPanesArea; }
    const Ref<ToolPanesArea>& inspectorPanesArea() const { return m_inspectorPanesArea; }

    void addDocument(Document* doc);
    void setActiveDocument(Document* doc);

protected:

private:
    void documentTabs_SelectionChanged(ln::UISelectionChangedEventArgs* e);

    Ref<ln::UIBoxLayout3> m_mainLayout;
    Ref<ln::UISwitchLayout> m_switchLayout;
    Ref<ln::UITabBar> m_documentTabs;
    Ref<StartupView> m_startupView;
    ln::List<Ref<Document>> m_documents;
    Document* m_activeDocument;

    Ref<ToolPanesArea> m_modePanesArea;
    Ref<ToolPanesArea> m_toolPanesArea;
    Ref<ToolPanesArea> m_inspectorPanesArea;
};

class AssetEditorDocument
    : public Document
{
public:
    AssetEditorDocument();
    ln::Result init(ln::AssetModel* asset, ln::AssetEditor* editorModel);

    const Ref<ln::AssetModel>& asset() const { return m_asset; }
    const Ref<ln::AssetEditor>& editor() const { return m_editor; }

protected:

private:
    Ref<ln::AssetModel> m_asset;
    Ref<ln::AssetEditor> m_editor;
};
