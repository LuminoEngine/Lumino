
#include "StartupView.hpp"

StartupView::StartupView()
{
}

ln::Result StartupView::init()
{
    UIControl::init();
    setLayoutPanel(ln::makeObject<ln::UIVBoxLayout>());

    m_newProjectButton = ln::makeObject<ln::UIButton>();
    m_newProjectButton->setText(u"New project...");
    addElement(m_newProjectButton);

    m_openProjectButton = ln::makeObject<ln::UIButton>();
    m_openProjectButton->setText(u"Open project...");
    addElement(m_openProjectButton);

    return true;
}
