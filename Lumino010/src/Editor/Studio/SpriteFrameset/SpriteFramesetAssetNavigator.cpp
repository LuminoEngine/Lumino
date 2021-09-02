
#include <Project.hpp>
#include "SpriteFramesetAssetNavigator.h"

//==============================================================================
// SpritesetAssetTreeView

SpritesetAssetTreeView::SpritesetAssetTreeView(QWidget* parent)
    : QTreeView(parent)
{
    m_model = new SpritesetAssetTreeModel(this);
    //QModelIndex rootModelIndex = m_model->setRootPath("D:/Proj/LN/PrivateProjects/HC0/Assets/Spriteset");


    //setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setTextElideMode(Qt::ElideNone);
    setModel(m_model);
    //setRootIndex(rootModelIndex);
    //setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    //setFixedHeight(500);
    //setColumnWidth(0, INT_MAX);


	setHeaderHidden(true);
	setColumnHidden(1, true);     // Hide Size
	setColumnHidden(2, true);     // Hide Type
	setColumnHidden(3, true);     // Hide Date

    // ダブルクリックで編集を開始しないようにする
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(onDoubleClicked(const QModelIndex &)));

    //connect(this, SIGNAL(doubleclicked(QModelIndex)), this, SLOT(OnDoubleclicked(QModelIndex)));
}

void SpritesetAssetTreeView::setRootDir(QString dir)
{
    QModelIndex rootModelIndex = m_model->setRootPath(dir);
    setRootIndex(rootModelIndex);
}

void SpritesetAssetTreeView::onDoubleClicked(const QModelIndex& index)
{
    QString filePath = m_model->fileInfo(index).absoluteFilePath();
}

//==============================================================================
// SpritesetContentsViewProvider

SpritesetContentsViewProvider::SpritesetContentsViewProvider(lna::Project* project, QWidget* parent)
    : ContentsViewProvider(parent)
    , m_treeView(nullptr)
{
	m_rootLayout = new QVBoxLayout(this);
	m_expander = new Expander("Test", this);
	m_treeView = new SpritesetAssetTreeView(this);
	m_newFileButton = new QPushButton("A");
	m_newFolderButton = new QPushButton("B");

	m_expander->setContent(m_treeView);
	m_expander->addToolWidget(m_newFileButton);
	m_expander->addToolWidget(m_newFolderButton);

	m_rootLayout->addWidget(m_expander);
	setLayout(m_rootLayout);


	auto assetDir = ln::Path(project->assetsDir(), u"Spriteset");
	ln::FileSystem::createDirectory(assetDir);
	m_treeView->setRootDir(LnToQt(assetDir));
}
