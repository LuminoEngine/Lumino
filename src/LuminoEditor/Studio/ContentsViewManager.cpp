
#include "MainWindow.h"
#include "External/QtAwesome/QtAwesome.h"
#include "ContentsViewManager.h"

ContentsViewManager::ContentsViewManager()
{
    m_sidear = new QFrame();
    m_sidear->setFixedWidth(50);
    m_sidear->setStyleSheet("background-color:gray;");

    {
        m_buttonContainer = new QVBoxLayout();
        m_buttonContainer->setAlignment(Qt::AlignTop);
        m_buttonContainer->setContentsMargins(0, 0, 0, 0);
        m_buttonContainer->setSpacing(0);
        //auto* button1 = new QPushButton();
        //button1->setFlat(true);
        //button1->setCheckable(true);
        //button1->setIcon(MainWindow::instance()->awesome()->icon("music"));
        ////connect(button1, &QPushButton::toggled, this, &ContentsViewPane::onContainerButtonCheckChanged);

        //vbox->addWidget(button1);
        //vbox->addWidget(new QPushButton("test2"));
        //vbox->addWidget(new QPushButton("test3"));
        m_sidear->setLayout(m_buttonContainer);
    }

    m_viewContainer = new QStackedWidget();
}

void ContentsViewManager::addContentsViewProvider(ContentsViewProvider* provider)
{
    LN_CHECK(provider);

    m_providers.add(provider);

    QVariantMap iconOptions;
    iconOptions.insert("color", QColor(255, 255, 255));
    iconOptions.insert("color-disabled", QColor(172, 172, 172));
    iconOptions.insert("color-active", QColor(255, 255, 255));
    iconOptions.insert("color-selected", QColor(255, 255, 255));

    auto* button1 = new QPushButton();
    button1->setFlat(true);
    button1->setCheckable(true);
    button1->setIcon(MainWindow::instance()->awesome()->icon(fa::music, iconOptions));
    button1->setFixedSize(50, 50);
    button1->setStyleSheet("padding: 3px; font-size: 24px;");
    button1->setContentsMargins(0, 0, 0, 0);
    button1->setIconSize(QSize(32, 32));
    m_buttonContainer->addWidget(button1);

    QWidget* view = provider->createView();
    m_viewContainer->addWidget(view);
}


//==============================================================================
// ContentsViewProvider

ContentsViewProvider::ContentsViewProvider()
{

}


