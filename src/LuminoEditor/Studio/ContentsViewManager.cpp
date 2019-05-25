
#include "ContentsViewManager.h"

ContentsViewManager::ContentsViewManager()
{
    m_sidear = new QFrame();
    m_sidear->setFixedWidth(50);
    m_sidear->setStyleSheet("background-color:gray;");


    {
        auto* vbox = new QVBoxLayout();
        vbox->setAlignment(Qt::AlignTop);
        auto* button1 = new QPushButton();
        button1->setFlat(true);
        button1->setCheckable(true);
        //button1->setIcon(awesome->icon(fa::cog));
        //connect(button1, &QPushButton::toggled, this, &ContentsViewPane::onContainerButtonCheckChanged);

        vbox->addWidget(button1);
        vbox->addWidget(new QPushButton("test2"));
        vbox->addWidget(new QPushButton("test3"));
        m_sidear->setLayout(vbox);
    }
}

void ContentsViewManager::addContentsViewProvider(ContentsViewProvider* value)
{
    m_providers.add(value);
}
