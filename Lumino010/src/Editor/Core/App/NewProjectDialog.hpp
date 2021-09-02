#pragma once

class NewProjectDialog : public ln::UIDialog
{
public:
	NewProjectDialog();


protected:

private:
	void updateDefaultProjectName();
	void handleSelectFolder();
	void handleCreate();

	Ref<ln::UITextField> m_projectName;
	Ref<ln::UITextField> m_projectFolder;
};

