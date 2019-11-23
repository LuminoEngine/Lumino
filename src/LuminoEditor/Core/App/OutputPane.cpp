
#include "Application.hpp"
#include "OutputPane.hpp"

OutputPane::OutputPane()
{
}

void OutputPane::init()
{
    EditorPane::init();

    setBackgroundColor(ln::Color::White);


    // TODO: test
    //{

    //    setLayoutPanel(ln::makeObject<ln::UIVBoxLayout>());

    //    auto m_newProjectButton = ln::makeObject<ln::UIButton>();
    //    m_newProjectButton->setWidth(200);
    //    m_newProjectButton->setText(u"New asset...");
    //    m_newProjectButton->setCommand(EditorApplication::NewCommand);
    //    addElement(m_newProjectButton);

    //    auto  m_openProjectButton = ln::makeObject<ln::UIButton>();
    //    m_openProjectButton->setWidth(200);
    //    m_openProjectButton->setText(u"Import asset...");
    //    m_openProjectButton->setCommand(EditorApplication::OpenCommand);
    //    addElement(m_openProjectButton);
    //}
}
