#pragma once
#include "../ContentsViewManager.h"


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
    PrefabContentsViewProvider(QObject* parent);
    virtual QString icon() const override { return "fileimageo"; }
    virtual QWidget* createView() override;
	virtual void onChangeCurrentProjet(lna::Project* project) override {}

    PrefabAssetTreeView* view() const { return m_treeView; }

private:
    PrefabAssetTreeView* m_treeView;
};

