
#include "MainWindow.h"
#include "External/QtAwesome/QtAwesome.h"
#include "ContentsViewManager.h"

ContentsViewManager::ContentsViewManager()
{
	m_sidebar = new QFrame();
	m_sidebar->setFixedWidth(50);
	m_sidebar->setStyleSheet("background-color:gray;");

    {
        m_buttonContainer = new QVBoxLayout();
        m_buttonContainer->setAlignment(Qt::AlignTop);
        m_buttonContainer->setContentsMargins(0, 0, 0, 0);
        m_buttonContainer->setSpacing(0);
        //auto* button1 = new QPushButton();
        //button1->setFlat(true);
        //button1->setCheckable(true);
        //button1->setIcon(MainWindow::instance()->awesome()->icon("music"));
        //connect(button1, &QPushButton::toggled, this, &ContentsViewPane::onContainerButtonCheckChanged);

        //vbox->addWidget(button1);
        //vbox->addWidget(new QPushButton("test2"));
        //vbox->addWidget(new QPushButton("test3"));
		m_sidebar->setLayout(m_buttonContainer);
    }

    m_viewContainer = new QStackedWidget();
}

void ContentsViewManager::setup(lna::Project* project, QString toolsetName)
{
    m_project = project;

	if (toolsetName == "ARPG-HC0") {

	}
	else {
		LN_NOTIMPLEMENTED();
	}
}

void ContentsViewManager::addContentsViewProvider(ContentsViewProvider* provider)
{
    LN_CHECK(provider);
    provider->m_manager = this;

    QVariantMap iconOptions;
    iconOptions.insert("color", QColor(255, 255, 255));
    iconOptions.insert("color-disabled", QColor(172, 172, 172));
    iconOptions.insert("color-active", QColor(255, 255, 255));
    iconOptions.insert("color-selected", QColor(255, 255, 255));

    auto* button1 = new QPushButton();
    button1->setFlat(true);
    button1->setCheckable(true);
    button1->setIcon(MainWindow::instance()->awesome()->icon(provider->icon(), iconOptions));
    button1->setFixedSize(50, 50);
    button1->setStyleSheet("padding: 3px; font-size: 24px;");
    button1->setContentsMargins(0, 0, 0, 0);
    button1->setIconSize(QSize(32, 32));
    connect(button1, &QPushButton::toggled, this, &ContentsViewManager::onContainerButtonCheckChanged);
    m_buttonContainer->addWidget(button1);
    button1->setProperty("_index", m_providers.size());

    m_viewContainer->addWidget(provider);

	ProviderInfo info;
	info.provider = provider;
	info.iconButton = button1;
    m_providers.append(info);
}

void ContentsViewManager::onContainerButtonCheckChanged(bool checked)
{
    if (checked) {
        auto* button = static_cast<QPushButton*>(sender());
        int index = button->property("_index").toInt();
        m_viewContainer->setCurrentIndex(index);

		// radio implement
		for (auto& info : m_providers) {
			if (info.iconButton != button) {
				info.iconButton->setChecked(false);
			}
		}
    }
}

//==============================================================================
// ContentsViewProvider

ContentsViewProvider::ContentsViewProvider(QWidget* parent = nullptr)
    : QFrame(parent)
{
}


