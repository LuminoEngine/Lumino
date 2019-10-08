#pragma once

class NewProjectDialog : public ln::UIDialog
{
public:
	NewProjectDialog();


protected:

private:
	Ref<ln::UITextField> m_projectName;
};

