#pragma once

namespace lna { class AssetEditorModel; }
class StartupView;
class ToolPanesArea;
class DocumentManager;
class AssetEditorDocument;

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

	/** Doucment を Close しようとする。reject された場合は false を返す */
	//bool close();

protected:
	virtual bool onClosing();

private:
    Ref<ln::UIContainerElement> m_mainFrame;

	friend class DocumentManager;
};

class DocumentTab
	: public ln::UITabItem
{
public:
	DocumentTab(Document* document);
	const Ref<Document>& document() const { return m_document; }

private:
	Ref<Document> m_document;
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

    AssetEditorDocument* activeDocument() const;

	// Model controls
    void addDocument(Document* doc);
	void removeDocument(Document* doc);
    void setActiveDocument(Document* doc);



	/** 各 Tab に Close メッセージ送信後、Reject されることなくすべて Close できたら true を返す。 */
	bool closeAllTabs();

protected:

private:
    void documentTabBar_SelectionChanged(ln::UISelectionChangedEventArgs* e);

    Ref<ln::UIBoxLayout> m_mainLayout;
    Ref<ln::UISwitchLayout> m_switchLayout;
    Ref<ln::UITabBar> m_documentTabBar;
	ln::List<Ref<DocumentTab>> m_documentTabs;

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
    ln::Result init(ln::AssetModel* asset, lna::AssetEditorModel* editorModel);

    const Ref<ln::AssetModel>& asset() const { return m_asset; }
    const Ref<lna::AssetEditorModel>& editor() const { return m_editor; }

protected:

private:
    Ref<ln::AssetModel> m_asset;
    Ref<lna::AssetEditorModel> m_editor;
};


