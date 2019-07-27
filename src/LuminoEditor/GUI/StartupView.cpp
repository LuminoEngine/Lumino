
#include "Application.hpp"
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
    m_newProjectButton->setCommand(EditorApplication::NewCommand);
    addElement(m_newProjectButton);

    m_openProjectButton = ln::makeObject<ln::UIButton>();
    m_openProjectButton->setText(u"Open project...");
    m_openProjectButton->setCommand(EditorApplication::OpenCommand);
    addElement(m_openProjectButton);

    return true;
}
