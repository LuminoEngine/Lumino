
#include "PrefabAssetNavigator.h"

//==============================================================================
// PrefabAssetTreeView

PrefabAssetTreeView::PrefabAssetTreeView(QWidget* parent)
    : QTreeView(parent)
{
    m_model = new PrefabAssetTreeModel(this);
    //QModelIndex rootModelIndex = m_model->setRootPath("D:/Proj/LN/PrivateProjects/HC0/Assets/Prefab");

    //setHeaderHidden(true);
    //setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setTextElideMode(Qt::ElideNone);
    setModel(m_model);
    //setRootIndex(rootModelIndex);
    //setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    //setFixedHeight(500);
    //setColumnWidth(0, INT_MAX);

    // ダブルクリックで編集を開始しないようにする
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(onDoubleClicked(const QModelIndex &)));

    //connect(this, SIGNAL(doubleclicked(QModelIndex)), this, SLOT(OnDoubleclicked(QModelIndex)));
}

void PrefabAssetTreeView::setRootDir(QString dir)
{
    QModelIndex rootModelIndex = m_model->setRootPath(dir);
    setRootIndex(rootModelIndex);
}

void PrefabAssetTreeView::onDoubleClicked(const QModelIndex &index)
{
    QString filePath = m_model->fileInfo(index).absoluteFilePath();
}

//==============================================================================
// PrefabContentsViewProvider

PrefabContentsViewProvider::PrefabContentsViewProvider(QObject* parent)
    : ContentsViewProvider(parent)
    , m_treeView(nullptr)
{
}

QWidget* PrefabContentsViewProvider::createView()
{
    LN_CHECK(!m_treeView);
    m_treeView = new PrefabAssetTreeView();
    return m_treeView;
}
