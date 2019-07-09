#pragma once
#include "../ContentsViewManager.h"
#include "../Widgets/Expander.h"


class AssetBrowserTreeModel : public QFileSystemModel
{
    Q_OBJECT
public:
    explicit AssetBrowserTreeModel(QObject* parent = nullptr)
        : QFileSystemModel(parent)
    {}

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

class AssetBrowserTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit AssetBrowserTreeView(lna::Project* project, QWidget* parent = nullptr);

    void setRootDir(QString dir);

public slots:
    void onDoubleClicked(const QModelIndex &index);

private:
    lna::Project* m_project;
    AssetBrowserTreeModel* m_model;
};



class AssetBrowserContentsViewProvider : public ContentsViewProvider
{
public:
    AssetBrowserContentsViewProvider(lna::Project* project, QWidget* parent);
    virtual QString icon() const override { return "file"; }
	virtual void onChangeCurrentProjet(lna::Project* project) override {}

    AssetBrowserTreeView* view() const { return m_treeView; }

private:
	QVBoxLayout* m_rootLayout;
	Expander* m_expander;
    AssetBrowserTreeView* m_treeView;
};

