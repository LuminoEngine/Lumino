
#include <LuminoEngine/Platform/PlatformWindow.hpp>	// TODO: remove platformWindow
#include "Application.hpp"
#include "MainWindow.hpp"	// TODO: remove platformWindow
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

	layout->addChild(ln::UITextBlock::create(u"Project folder:"));

	m_projectFolder = ln::makeObject<ln::UITextField>();
	m_projectFolder->setWidth(200);
	m_projectFolder->setHeight(20);
	layout->addChild(m_projectFolder);

	auto selectFolderButton = ln::makeObject<ln::UIButton>();
	selectFolderButton->setText(u"...");
	selectFolderButton->connectOnClicked(ln::bind(this, &NewProjectDialog::handleSelectFolder));
	layout->addChild(selectFolderButton);

	auto createButton = ln::makeObject<ln::UIButton>();
	createButton->setText(u"Create");
	createButton->connectOnClicked(ln::bind(this, &NewProjectDialog::handleCreate));
	layout->addChild(createButton);

	addElement(layout);
}

void NewProjectDialog::handleSelectFolder(ln::UIEventArgs* e)
{
	auto dlg = ln::PlatformSelectFolderDialog::create();
	if (dlg->showDialog(EditorApplication::instance()->mainWindow()->platformWindow())) {	// TODO: remove platformWindow
		m_projectFolder->setText(dlg->getPath());
	}
}

void NewProjectDialog::handleCreate(ln::UIEventArgs* e)
{
	auto text = m_projectFolder->text();
	std::cout << text;
}
