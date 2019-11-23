
#include "ActionManager.h"
#include "StartupView.h"

//==============================================================================
// StartupView

StartupView::StartupView(QWidget* parent)
    : QWidget(parent)
{
    auto layout1 = new QVBoxLayout();
	layout1->setAlignment(Qt::AlignTop);
	layout1->setMargin(100);
	{
		auto label1 = new QLabel(this);
		label1->setText(tr("Start"));
		label1->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
		layout1->addWidget(label1);

		m_newProjectButton = new QPushButton(this);
		m_newProjectButton->setText(tr("New project..."));
		layout1->addWidget(m_newProjectButton);

		m_openProjectButton = new QPushButton(this);
		m_openProjectButton->setText(tr("Open project..."));
		layout1->addWidget(m_openProjectButton);

	}
    setLayout(layout1);

	connect(m_newProjectButton, &QPushButton::clicked, this, [] { ActionManager::instance()->triggerAction("NewProject"); });
	connect(m_openProjectButton, &QPushButton::clicked, this, [] { ActionManager::instance()->triggerAction("OpenProject"); });
}
