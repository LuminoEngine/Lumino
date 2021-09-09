
#include <LuminoPlatform/PlatformWindow.hpp>	// TODO: remove platformWindow
#include <LuminoPlatform/PlatformDialogs.hpp>
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

	auto layout = ln::makeObject<ln::UIBoxLayout>();
	layout->addChild(ln::UIText::create(_TT("Project name:")));

	m_projectName = ln::makeObject<ln::UITextField>();
	m_projectName->setWidth(200);
	m_projectName->setHeight(20);
	layout->addChild(m_projectName);

	layout->addChild(ln::UIText::create(_TT("Project folder:")));

	m_projectFolder = ln::makeObject<ln::UITextField>();
	m_projectFolder->setWidth(200);
	m_projectFolder->setHeight(20);
	layout->addChild(m_projectFolder);

	auto selectFolderButton = ln::makeObject<ln::UIButton>();
	selectFolderButton->setText(_TT("..."));
	selectFolderButton->connectOnClicked(ln::bind(this, &NewProjectDialog::handleSelectFolder));
	layout->addChild(selectFolderButton);

	auto createButton = ln::makeObject<ln::UIButton>();
	createButton->setText(_TT("Create"));
	createButton->connectOnClicked(ln::bind(this, &NewProjectDialog::handleCreate));
	layout->addChild(createButton);

	addElement(layout);

	updateDefaultProjectName();
}

void NewProjectDialog::updateDefaultProjectName()
{
	// TODO: unique
	m_projectName->setText(_TT("Project1"));
}

void NewProjectDialog::handleSelectFolder()
{
	auto dlg = ln::PlatformSelectFolderDialog::create();
	if (dlg->showDialog(EditorApplication::instance()->mainWindow()->platformWindow())) {	// TODO: remove platformWindow
		m_projectFolder->setText(dlg->getPath());
	}
}

void NewProjectDialog::handleCreate()
{
	EditorApplication::instance()->newProject(m_projectFolder->text(), m_projectName->text());
	close();
}
