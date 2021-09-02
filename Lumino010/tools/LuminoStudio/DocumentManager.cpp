#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include "DocumentManager.h"
#include "LuminoWidget.h"

DocumentManager::DocumentManager(QObject* parent)
    : QObject(parent)
    , m_rootWidget(new QWidget())
    , m_tabWidget(new QTabWidget(m_rootWidget))
{
    QVBoxLayout* vertical = new QVBoxLayout(m_rootWidget);
    vertical->addWidget(m_tabWidget);
    vertical->setMargin(0);
    vertical->setSpacing(0);


    //QTextEdit* editor1 = new QTextEdit(tr("1つめのタブ"), m_tabWidget);
    QTextEdit* editor2 = new QTextEdit(tr("2つめのタブ"), m_tabWidget);
    m_tabWidget->addTab(new LuminoWidget(m_tabWidget), tr("TAB1"));
    m_tabWidget->addTab(editor2, tr("TAB2"));
    m_tabWidget->setTabsClosable(true);
}

DocumentManager::~DocumentManager()
{
    delete m_tabWidget;
}

#include "DocumentManager.moc"
