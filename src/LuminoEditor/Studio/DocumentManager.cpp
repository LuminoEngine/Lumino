
#include "DocumentManager.h"
#include "LuminoWidget.h"
#include "StartupView.h"
#include "SceneEditorDocumentView.h"

DocumentManager::DocumentManager(QWidget* parent)
    : QObject(parent)
    , m_rootWidget(new QWidget(parent))
	//, m_rootLayout(new QStackedLayout(m_rootWidget))
    //, m_tabWidget(new QTabWidget(m_rootWidget))
    , m_tabbar(new QTabBar(m_rootWidget))
	, m_startupView(new StartupView(m_rootWidget))
{

	//auto* b = new QPushButton("test", m_rootWidget);
	//auto s = b->sizePolicy();

	//auto* t = new QPlainTextEdit(m_rootWidget);
	//auto s = t->sizePolicy();

	//QVBoxLayout *vertical = new QVBoxLayout(m_rootWidget);
	//QGridLayout* layout = new QGridLayout();
	//layout->addWidget(new TilemapLandscapeWorldEditorView());
	////layout->addWidget(t);
	////layout->addWidget(b);
	//layout->setMargin(0);
	//layout->setSpacing(0);
	//layout->setColumnStretch(0, 300);
	//layout->setRowStretch(0, 300);


	//win->setStyleSheet("background-color: green");
	//win->setFixedHeight(100);
	//win->setFixedWidth(200);
	QVBoxLayout *layout = new QVBoxLayout();
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);
	m_rootWidget->setLayout(layout);

	//TilemapLandscapeWorldEditorView *simpleWidget = new TilemapLandscapeWorldEditorView();
	//layout->addWidget(simpleWidget);


	//m_rootLayout->addWidget(new TilemapLandscapeWorldEditorView(m_rootWidget));
	//m_rootWidget->setLayout(m_rootLayout);

    layout->addWidget(m_tabbar);

    //m_tabWidget = new QTabWidget();
	m_viewStack = new QStackedLayout();
	m_viewStack->addWidget(m_startupView);
    layout->addLayout(m_viewStack);

#if 1
    //QVBoxLayout* vertical = new QVBoxLayout(m_rootWidget);
    //vertical->addWidget(m_tabWidget);
    //vertical->setMargin(0);
    //vertical->setSpacing(0);


 //   //QTextEdit* editor1 = new QTextEdit(tr("1つめのタブ"), m_tabWidget);
 //   QTextEdit* editor2 = new QTextEdit(tr("2つめのタブ"), m_tabWidget);
	////m_tabWidget->addTab(new TilemapLandscapeWorldEditorView(m_tabWidget), tr("TAB0"));
 //   m_tabWidget->addTab(new LuminoWidget(m_tabWidget), tr("TAB1"));
 //   m_tabWidget->addTab(editor2, tr("TAB2"));
    //m_tabWidget->setTabsClosable(true);
#endif

}

DocumentManager::~DocumentManager()
{
    //delete m_tabWidget;
}

void DocumentManager::addDocument(EditorDocument* doc)
{
    QWidget* view = nullptr;
    if (auto ddoc = dynamic_cast<SceneEditorDocument*>(doc)) {
        view = new SceneEditorView();
        //m_tabWidget->addTab(view, tr("tab"));
        m_documents.append(ddoc);
        ddoc->setParent(this);
    }
    else if (auto ddoc = dynamic_cast<AssetEditorDocument*>(doc)) {
        view = new AssetEditorView(ddoc);
        //m_tabWidget->addTab(view, tr("tab"));
        m_documents.append(ddoc);
        ddoc->setParent(this);
    }
    else if (auto ddoc = dynamic_cast<EditorDocument*>(doc)) {
        view = new EditorView();
        //m_tabWidget->addTab(view, tr("tab"));
        m_documents.append(ddoc);
        ddoc->setParent(this);
    }
    else {
        LN_UNREACHABLE();
    }

    //int i = m_viewStack->count();
    if (m_tabbar->count() == 0) {
        m_viewStack->removeWidget(m_startupView);
    }

    m_viewStack->addWidget(view);
    int index = m_tabbar->addTab("Tab");
    m_viewStack->setCurrentIndex(index);
}
