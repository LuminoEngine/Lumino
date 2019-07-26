
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

    m_startupView = ln::makeObject<StartupView>();
    addElement(m_startupView);

    return true;
}
