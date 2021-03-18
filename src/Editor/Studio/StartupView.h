#pragma once
#include "Document.h"

class StartupView : public QWidget
{
    Q_OBJECT
public:
	StartupView(QWidget* parent = nullptr);

private:
	QPushButton* m_newProjectButton;
	QPushButton* m_openProjectButton;
};
