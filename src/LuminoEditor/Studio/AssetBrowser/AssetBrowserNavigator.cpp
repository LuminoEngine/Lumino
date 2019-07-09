
#include <Project.hpp>
#include "../External/QtAwesome/QtAwesome.h"
#include "../MainWindow.h"
#include "AssetBrowserNavigator.h"

//==============================================================================
// AssetBrowserTreeModel

QVariant AssetBrowserTreeModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DecorationRole) {
        QFileInfo info = QFileSystemModel::fileInfo(index);
        if (info.isFile()) {
            return MainWindow::instance()->awesome()->icon("file");
        }
        else {
            return MainWindow::instance()->awesome()->icon("folder");
        }
    }
    return QFileSystemModel::data(index, role);
}

//==============================================================================
// AssetBrowserTreeView

AssetBrowserTreeView::AssetBrowserTreeView(lna::Project* project, QWidget* parent)
    : QTreeView(parent)
    , m_project(project)
{
    m_model = new AssetBrowserTreeModel(this);
    QModelIndex rootModelIndex = m_model->setRootPath(LnToQt(m_project->assetsDir()));//"D:/Proj/TH-10/Assets");//

    //setHeaderHidden(true);
    //setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setTextElideMode(Qt::ElideNone);
    setModel(m_model);
    setRootIndex(rootModelIndex);
    //setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    //setFixedHeight(500);
    //setColumnWidth(0, INT_MAX);

    // Hide other than name
    for (int i = 1; i < m_model->columnCount(); ++i) {
        hideColumn(i);
    }

    // ダブルクリックで編集を開始しないようにする
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(onDoubleClicked(const QModelIndex &)));

    //connect(this, SIGNAL(doubleclicked(QModelIndex)), this, SLOT(OnDoubleclicked(QModelIndex)));
}

void AssetBrowserTreeView::setRootDir(QString dir)
{
    QModelIndex rootModelIndex = m_model->setRootPath(dir);
    setRootIndex(rootModelIndex);
}

void AssetBrowserTreeView::onDoubleClicked(const QModelIndex &index)
{
    QString filePath = m_model->fileInfo(index).absoluteFilePath();

    ;
    ln::GameAudio::playBGM(ln::String((char16_t*)filePath.data()));
}

//==============================================================================
// AssetBrowserContentsViewProvider

AssetBrowserContentsViewProvider::AssetBrowserContentsViewProvider(lna::Project* project, QWidget* parent)
    : ContentsViewProvider(parent)
    , m_treeView(nullptr)
{
	m_rootLayout = new QVBoxLayout(this);
	m_expander = new Expander("Test", this);
	m_treeView = new AssetBrowserTreeView(project, this);

	m_expander->setContent(m_treeView);

	m_rootLayout->addWidget(m_expander);
	setLayout(m_rootLayout);
}
