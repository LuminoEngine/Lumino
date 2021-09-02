#pragma once
#include "../ContentsViewManager.h"
#include "../Widgets/Expander.h"


class PrefabAssetTreeModel : public QFileSystemModel
{
    Q_OBJECT
public:
    explicit PrefabAssetTreeModel(QObject* parent = nullptr)
        : QFileSystemModel(parent)
    {}
};

class PrefabAssetTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit PrefabAssetTreeView(QWidget* parent = nullptr);

    void setRootDir(QString dir);

public slots:
    void onDoubleClicked(const QModelIndex &index);

private:
    PrefabAssetTreeModel* m_model;
};



class PrefabContentsViewProvider : public ContentsViewProvider
{
public:
    PrefabContentsViewProvider(QWidget* parent);
    virtual QString icon() const override { return "fileimageo"; }
	virtual void onChangeCurrentProjet(lna::Project* project) override {}

    PrefabAssetTreeView* view() const { return m_treeView; }

private:
	QVBoxLayout* m_rootLayout;
	Expander* m_expander;
    PrefabAssetTreeView* m_treeView;
};

