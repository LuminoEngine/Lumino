
#include <LuminoEditor/Plugin.hpp>
#include "AssetEditor/AssetEditorModel.hpp"
#include "StartupView.hpp"
#include "ToolPanesArea.hpp"
#include "DocumentManager.hpp"

//==============================================================================
// Document

Document::Document()
{
}

ln::Result Document::init()
{
    Object::init();
    m_mainFrame = ln::makeObject<ln::UIContainerElement>();

    // SaveCommand などは m_mainFrame にアタッチしたい。
    // m_mainFrame が focus を持てるようにしておかないと、もし Document 内に Focusable な UIElement が１つも無いと、
    // コマンドを実行できなくなる。
    m_mainFrame->setFocusable(true);

    //m_mainFrame->getGridLayoutInfo()->layoutWeight = 1; // fill in box layout
    return true;
}

//bool Document::close()
//{
//	return onClosing();
//}

bool Document::onClosing()
{
	return true;
}


//==============================================================================
// DocumentManager

DocumentManager::DocumentManager()
{
}

ln::Result DocumentManager::init()
{
    UIControl::init();
    //setLayoutPanel(ln::makeObject<ln::UIVBoxLayout>());

    m_mainLayout = ln::makeObject<ln::UIBoxLayout>();
    //addElement(m_mainLayout);

    m_documentTabBar = ln::makeObject<ln::UITabBar>();
    m_documentTabBar->setVAlignment(ln::UIVAlignment::Top);
    m_documentTabBar->connectOnSelectionChanged(ln::bind(this, &DocumentManager::documentTabBar_SelectionChanged));
    m_documentTabBar->setBackgroundColor(ln::Color::Azure);
    //m_documentTabBar->setHeight(30);
    //m_documentTabBar->setVisibility(ln::UIVisibility::Collapsed);
	m_documentTabBar->getGridLayoutInfo()->layoutWeight = 0;	// 'auto'
    m_mainLayout->addChild(m_documentTabBar);

    m_switchLayout = ln::makeObject<ln::UISwitchLayout>();
    m_switchLayout->setActiveIndex(-1);
    m_switchLayout->getGridLayoutInfo()->layoutWeight = 1;  // fill in box layout
    m_mainLayout->addChild(m_switchLayout);

    m_startupView = ln::makeObject<StartupView>();
    addElement(m_startupView);
    //m_startupView->setHeight(300);
    //m_mainLayout->addChild(m_startupView);

    m_modePanesArea = ln::makeObject<ToolPanesArea>();
    m_toolPanesArea = ln::makeObject<ToolPanesArea>();
    m_inspectorPanesArea = ln::makeObject<ToolPanesArea>();

    return true;
}

AssetEditorDocument* DocumentManager::activeDocument() const
{
    auto d = dynamic_cast<AssetEditorDocument*>(m_activeDocument);
    assert(d);  // TODO: もう AssetEditorDocument しか作らない
    return d;
}

void DocumentManager::addDocument(Document* doc)
{
    if (m_documents.size() == 0) {
        removeElement(m_startupView);
        addElement(m_mainLayout);
    }

    m_documents.add(doc);
    m_switchLayout->addChild(doc->mainFrame());

	auto tab = ln::makeObject2<DocumentTab>(doc);
	m_documentTabs.add(tab);

    m_documentTabBar->addTab(tab);
    m_documentTabBar->setSelectedTab(tab);
}

void DocumentManager::removeDocument(Document* doc)
{
	for (auto& tab : m_documentTabs) {
		if (tab->document() == doc) {
			m_documentTabs.remove(tab);
			m_switchLayout->removeChild(doc->mainFrame());
			m_documentTabBar->removeTab(tab);
			break;
		}
	}
}

void DocumentManager::setActiveDocument(Document* doc)
{
    if (doc) {
        m_activeDocument = doc;
        m_switchLayout->setActive(doc->mainFrame());

        if (auto assetEditorDoc = dynamic_cast<AssetEditorDocument*>(doc)) {
            auto list = assetEditorDoc->editor()->getEditorPanes(lna::EditorPaneKind::Mode);
            if (list) {
                for (auto& pane : list) {
                    m_modePanesArea->addPane(pane);
                }
            }
            list = assetEditorDoc->editor()->getEditorPanes(lna::EditorPaneKind::Inspector);
            if (list) {
                for (auto& pane : list) {
                    m_inspectorPanesArea->addPane(pane);
                }
            }
            list = assetEditorDoc->editor()->getEditorPanes(lna::EditorPaneKind::Tool);
            if (list) {
                for (auto& pane : list) {
                    m_toolPanesArea->addPane(pane);
                }
            }
        }

        m_activeDocument->mainFrame()->focus();
    }
    else {
        LN_NOTIMPLEMENTED();
    }
}

bool DocumentManager::closeAllTabs()
{
	auto closeList = m_documentTabs;
	for (auto& tab : closeList) {
		if (tab->document()->onClosing()) {
			removeDocument(tab->document());
		}
		else {
			return false;
		}
	}
	return true;
}

void DocumentManager::documentTabBar_SelectionChanged(ln::UISelectionChangedEventArgs* e)
{
    auto tab = m_documentTabBar->selectedTab();
    auto doc = tab->data()->getObject<Document>();
    setActiveDocument(doc);
}


//==============================================================================
// AssetEditorDocument

AssetEditorDocument::AssetEditorDocument()
{
}

ln::Result AssetEditorDocument::init(ln::AssetModel* asset, lna::AssetEditorModel* editorModel)
{
    if (!Document::init()) return false;
    m_asset = asset;
    m_editor = editorModel;

    m_editor->onOpened(m_asset, mainFrame());
    return true;
}

//==============================================================================
// DocumentTab

DocumentTab::DocumentTab(Document* document)
	: m_document(document)
{
	//auto tab = ln::makeObject<ln::UITabItem>();
	auto text = ln::makeObject<ln::UIText>();
	text->setText(_TT("Tab"));  // TODO: AssetEditor から、Document 経由で設定したい。プロパティ変更を scribe する必要があるかも。
	/*tab->*/addElement(text);
	/*tab->*/setData(ln::makeVariant(document));
}
