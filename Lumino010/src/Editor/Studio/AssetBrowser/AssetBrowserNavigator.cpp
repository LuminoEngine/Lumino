
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
    if (role == Qt::BackgroundColorRole) {

        QFileInfo info = QFileSystemModel::fileInfo(index);
        if (QFileInfo::exists(info.filePath() + QString((const QChar*)ln::AssetModel::AssetFileExtension.c_str()))) {

        }
        else {
            return QColor(Qt::gray);
        }
    }
    return QFileSystemModel::data(index, role);
}

Qt::ItemFlags AssetBrowserTreeModel::flags(const QModelIndex &index) const
{
    auto flags = QFileSystemModel::flags(index);

    //QFileInfo info = QFileSystemModel::fileInfo(index);
    //auto s = info.suffix();
    //if (info.suffix() == "lnasset") {
    //    flags &= ~Qt::ItemIsEnabled;
    //    // ### TODO you shouldn't be able to set this as the current item, task 119433
    //    return flags;
    //}

    return flags;
}

//==============================================================================
// AssetBrowserTreeView

AssetBrowserTreeView::AssetBrowserTreeView(lna::Project* project, QWidget* parent)
    : QTreeView(parent)
    , m_project(project)
{
    m_model = new AssetBrowserTreeModel(this);

    m_modelProxy = new AssetBrowserTreeModelProxy(this);
    m_modelProxy->setSourceModel(m_model);


    //setHeaderHidden(true);
    //setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setTextElideMode(Qt::ElideNone);
    setModel(m_modelProxy);


    QModelIndex rootModelIndex = m_model->setRootPath(LnToQt(m_project->assetsDir()));//"D:/Proj/TH-10/Assets");//
    setRootIndex(m_modelProxy->mapFromSource(rootModelIndex));
    //setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    //setFixedHeight(500);
    //setColumnWidth(0, INT_MAX);


    //QStringList sDriveFilters;
    //sDriveFilters << "*.lnasset";
    //m_model->setNameFilters(sDriveFilters);
    //m_model->setNameFilterDisables(true);

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
    auto sourceIndex = m_modelProxy->mapToSource(index);
    QString filePath = m_model->fileInfo(sourceIndex).absoluteFilePath();
    //MainWindow::instance()->importFile(filePath);
    MainWindow::instance()->openFile(filePath);
    //;
    //ln::GameAudio::playBGM(ln::String((char16_t*)filePath.data()));
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
