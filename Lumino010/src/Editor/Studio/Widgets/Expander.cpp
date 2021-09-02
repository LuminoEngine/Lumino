
#include "Expander.h"

Expander::Expander(QString title, QWidget* parent)
	: QWidget(parent)
	, m_rootLayout(nullptr)
	, m_headerToolLayout(nullptr)
	, m_toggleButton(nullptr)
	, m_contentArea(nullptr)
	, m_contentLayout(nullptr)
	, m_content(nullptr)
{
	setStyleSheet("QWidget {background-color: gray;}");

	m_rootLayout = new QGridLayout(this);
	m_headerToolLayout = new QHBoxLayout(this);
	m_toggleButton = new QToolButton(this);
	m_contentArea = new QFrame(this);
	m_contentLayout = new QVBoxLayout(this);

	m_headerToolLayout->setAlignment(Qt::AlignTop);

	m_toggleButton->setStyleSheet("QToolButton {border: none;}");
	m_toggleButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	m_toggleButton->setArrowType(Qt::ArrowType::RightArrow);
	m_toggleButton->setCheckable(true);
	m_toggleButton->setChecked(false);

	// collapsed
	//m_contentArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	//m_contentArea->setMaximumHeight(0);
	//m_contentArea->setMinimumHeight(0);
	m_contentArea->setLayout(m_contentLayout);

	m_rootLayout->setVerticalSpacing(0);
	m_rootLayout->setContentsMargins(0, 0, 0, 0);
	m_rootLayout->addWidget(m_toggleButton, 0, 0, 1, 1, Qt::AlignLeft);
	m_rootLayout->addLayout(m_headerToolLayout, 0, 2, 1, 1, Qt::AlignRight);
	m_rootLayout->addWidget(m_contentArea, 1, 0, 1, 3);
	setLayout(m_rootLayout);

    QObject::connect(m_toggleButton, &QToolButton::toggled, [this](const bool checked)
    {
		m_toggleButton->setArrowType(checked ? Qt::ArrowType::DownArrow : Qt::ArrowType::RightArrow);
		m_contentArea->setMinimumHeight(m_contentHeight);
		m_contentArea->setMaximumHeight(m_contentHeight);
    });
}

void Expander::addToolWidget(QWidget* widget)
{
	m_headerToolLayout->addWidget(widget);
}

void Expander::setContent(QWidget* content)
{
	assert(!m_content);

	//auto hh = content->minimumHeight();
	//auto hhg = content->maximumHeight();

	m_content = content;
	m_contentLayout->addWidget(m_content);
	m_contentHeight = m_contentArea->sizeHint().height();
}
