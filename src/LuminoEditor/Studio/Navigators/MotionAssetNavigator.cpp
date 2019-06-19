
#include "MotionAssetNavigator.h"

//==============================================================================
// MotionAssetTreeView

MotionAssetTreeView::MotionAssetTreeView(QWidget* parent)
    : QTreeView(parent)
{
    m_model = new MotionAssetTreeModel(this);
    //QModelIndex rootModelIndex = m_model->setRootPath("D:/Proj/LN/PrivateProjects/HC0/Assets/Motion");

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

void MotionAssetTreeView::setRootDir(QString dir)
{
    QModelIndex rootModelIndex = m_model->setRootPath(dir);
    setRootIndex(rootModelIndex);
}

void MotionAssetTreeView::onDoubleClicked(const QModelIndex &index)
{
    QString filePath = m_model->fileInfo(index).absoluteFilePath();
}

//==============================================================================
// MotionContentsViewProvider

MotionContentsViewProvider::MotionContentsViewProvider(QObject* parent)
    : ContentsViewProvider(parent)
    , m_treeView(nullptr)
{
}

QWidget* MotionContentsViewProvider::createView()
{
    LN_CHECK(!m_treeView);
    m_treeView = new MotionAssetTreeView();
    return m_treeView;
}
