
#include "AudioAssetNavigator.h"

//==============================================================================
// AudioAssetTreeView

AudioAssetTreeView::AudioAssetTreeView(QWidget* parent)
    : QTreeView(parent)
{
    m_model = new AudioAssetTreeModel(this);
    //QModelIndex rootModelIndex = m_model->setRootPath("D:/Proj/LN/PrivateProjects/HC0/Assets/Audio");

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

void AudioAssetTreeView::setRootDir(QString dir)
{
    QModelIndex rootModelIndex = m_model->setRootPath(dir);
    setRootIndex(rootModelIndex);
}

void AudioAssetTreeView::onDoubleClicked(const QModelIndex &index)
{
    QString filePath = m_model->fileInfo(index).absoluteFilePath();

    ;
    ln::GameAudio::playBGM(ln::String((char16_t*)filePath.data()));
}

//==============================================================================
// AudioContentsViewProvider

AudioContentsViewProvider::AudioContentsViewProvider(QObject* parent)
    : ContentsViewProvider(parent)
    , m_treeView(nullptr)
{
}

QWidget* AudioContentsViewProvider::createView()
{
    LN_CHECK(!m_treeView);
    m_treeView = new AudioAssetTreeView();
    return m_treeView;
}
