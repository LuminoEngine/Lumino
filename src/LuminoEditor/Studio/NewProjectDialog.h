#pragma once
#include "Document.h"

class NewProjectDialog : public QDialog
{
    Q_OBJECT
public:
	NewProjectDialog(QWidget* parent = nullptr);

private:
	QLineEdit* m_projectName;
	QDialogButtonBox* m_buttons;
};
