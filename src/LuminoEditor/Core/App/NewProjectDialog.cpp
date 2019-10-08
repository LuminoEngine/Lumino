
#include "NewProjectDialog.hpp"

//==============================================================================
// NewProjectDialog

NewProjectDialog::NewProjectDialog()
{
	setWidth(100);
	setHeight(200);
	setBackgroundColor(ln::Color::White);

	auto layout = ln::makeObject<ln::UIBoxLayout3>();
	layout->addChild(ln::UITextBlock::create(u"Project name:"));

	m_projectName = ln::makeObject<ln::UITextField>();
	m_projectName->setWidth(200);
	m_projectName->setHeight(20);
	layout->addChild(m_projectName);

	addElement(layout);
}
