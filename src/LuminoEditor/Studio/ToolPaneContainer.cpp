
#include "ToolPaneContainer.h"

//==============================================================================
// ToolPaneContainer

ToolPaneContainer::ToolPaneContainer(QWidget* parent)
    : QFrame(parent)
{
	auto* layout1 = new QVBoxLayout();

	m_tabs = new QTabWidget();
	layout1->addWidget(m_tabs);

	setLayout(layout1);
}

void ToolPaneContainer::addStandalonePane(QFrame* pane)
{
	m_tabs->addTab(pane, "Test");
}
