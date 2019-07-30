
#include "StartupView.hpp"
#include "DocumentManager.hpp"

//==============================================================================
// Document

Document::Document()
{
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

    //auto documentTabs = ln::makeObject<ln::UITabBar>();
    //documentTabs->setVerticalAlignment(ln::VAlignment::Top);
    //documentTabs->setBackgroundColor(ln::Color::Azure);
    //documentTabs->setHeight(30);
    //addElement(documentTabs);

    //// test
    //auto tab1 = ln::makeObject<ln::UITabItem>();
    //auto text1 = ln::makeObject<ln::UITextBlock>();
    //text1->setText(u"Scene-1");
    //tab1->addElement(text1);
    //documentTabs->addTab(tab1);

    //auto tab2 = ln::makeObject<ln::UITabItem>();
    //auto text2 = ln::makeObject<ln::UITextBlock>();
    //text2->setText(u"Scene-2");
    //tab2->addElement(text2);
    //documentTabs->addTab(tab2);


    m_startupView = ln::makeObject<StartupView>();
    //m_startupView->setHeight(300);
    addElement(m_startupView);

    return true;
}
