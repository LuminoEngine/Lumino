
#include <LuminoEditor/Plugin.hpp>
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
    m_mainFrame->setBackgroundColor(ln::Color(ln::Random::randFloat(), ln::Random::randFloat(), ln::Random::randFloat(), 1));
    //m_mainFrame->getGridLayoutInfo()->layoutWeight = 1; // fill in box layout
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

    m_mainLayout = ln::makeObject<ln::UIBoxLayout3>();
    //addElement(m_mainLayout);

    m_documentTabs = ln::makeObject<ln::UITabBar>();
    m_documentTabs->setVerticalAlignment(ln::VAlignment::Top);
    m_documentTabs->connectOnSelectionChanged(ln::bind(this, &DocumentManager::documentTabs_SelectionChanged));
    //documentTabs->setBackgroundColor(ln::Color::Azure);
    //documentTabs->setHeight(30);
    //documentTabs->setVisibility(ln::UIVisibility::Collapsed);
    m_mainLayout->addChild(m_documentTabs);

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

void DocumentManager::addDocument(Document* doc)
{
    if (m_documents.size() == 0) {
        removeElement(m_startupView);
        addElement(m_mainLayout);
    }

    m_documents.add(doc);
    m_switchLayout->addChild(doc->mainFrame());

    auto tab = ln::makeObject<ln::UITabItem>();
    auto text = ln::makeObject<ln::UITextBlock>();
    text->setText(u"Scene");
    tab->addElement(text);
    tab->setData(ln::makeVariant(doc));
    m_documentTabs->addTab(tab);
    m_documentTabs->setSelectedTab(tab);
}

void DocumentManager::setActiveDocument(Document* doc)
{
    if (doc) {
        m_activeDocument = doc;
        m_switchLayout->setActive(doc->mainFrame());

        if (auto assetEditorDoc = dynamic_cast<AssetEditorDocument*>(doc)) {
            for (auto& pane : assetEditorDoc->editor()->getEditorPanes(ln::EditorPaneKind::Mode)) {
                m_modePanesArea->addPane(pane);
            }
            for (auto& pane : assetEditorDoc->editor()->getEditorPanes(ln::EditorPaneKind::Inspector)) {
                m_inspectorPanesArea->addPane(pane);
            }
            for (auto& pane : assetEditorDoc->editor()->getEditorPanes(ln::EditorPaneKind::Tool)) {
                m_toolPanesArea->addPane(pane);
            }
        }
    }
    else {
        LN_NOTIMPLEMENTED();
    }
}

void DocumentManager::documentTabs_SelectionChanged(ln::UISelectionChangedEventArgs* e)
{
    auto tab = m_documentTabs->selectedTab();
    auto doc = tab->data()->getObject<Document>();
    setActiveDocument(doc);
}


//==============================================================================
// AssetEditorDocument

AssetEditorDocument::AssetEditorDocument()
{
}

ln::Result AssetEditorDocument::init(ln::AssetModel* asset, ln::AssetEditor* editorModel)
{
    if (!Document::init()) return false;
    m_asset = asset;
    m_editor = editorModel;
    return true;
}
