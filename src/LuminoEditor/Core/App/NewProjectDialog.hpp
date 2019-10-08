#pragma once

class NewProjectDialog : public ln::UIDialog
{
public:
	NewProjectDialog();


protected:

private:
	void handleSelectFolder(ln::UIEventArgs* e);
	void handleCreate(ln::UIEventArgs* e);

	Ref<ln::UITextField> m_projectName;
	Ref<ln::UITextField> m_projectFolder;
};

