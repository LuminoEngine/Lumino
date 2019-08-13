
#include <LuminoEditor/Plugin.hpp>
#include "StartupView.hpp"
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
    m_mainFrame->setBackgroundColor(ln::Color::Azure);
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
    addElement(m_mainLayout);

    auto documentTabs = ln::makeObject<ln::UITabBar>();
    documentTabs->setVerticalAlignment(ln::VAlignment::Top);
    //documentTabs->setBackgroundColor(ln::Color::Azure);
    //documentTabs->setHeight(30);
    //documentTabs->setVisibility(ln::UIVisibility::Collapsed);
    m_mainLayout->addChild(documentTabs);

    // test
    auto tab1 = ln::makeObject<ln::UITabItem>();
    auto text1 = ln::makeObject<ln::UITextBlock>();
    text1->setText(u"Scene-1");
    tab1->addElement(text1);
    documentTabs->addTab(tab1);

    auto tab2 = ln::makeObject<ln::UITabItem>();
    auto text2 = ln::makeObject<ln::UITextBlock>();
    text2->setText(u"Scene-2");
    tab2->addElement(text2);
    documentTabs->addTab(tab2);


    m_startupView = ln::makeObject<StartupView>();
    //m_startupView->setHeight(300);
    m_mainLayout->addChild(m_startupView);

    return true;
}

void DocumentManager::addDocument(Document* doc)
{
    if (m_documents.size() == 0) {
        m_mainLayout->removeChild(m_startupView);
    }

    m_documents.add(doc);
    addElement(doc->mainFrame());
}


//==============================================================================
// AssetEditorDocument

AssetEditorDocument::AssetEditorDocument()
{
}

ln::Result AssetEditorDocument::init(ln::AssetModel* asset, ln::AssetEditorViewModel* editorModel)
{
    if (!Document::init()) return false;
    m_asset = asset;
    m_editor = editorModel;
    return true;
}
