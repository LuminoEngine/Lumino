#pragma once
#include "../ContentsViewManager.h"


class MapAssetTreeModel : public QFileSystemModel
{
    Q_OBJECT
public:
    explicit MapAssetTreeModel(QObject* parent = nullptr)
        : QFileSystemModel(parent)
    {}
};

class MapAssetTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit MapAssetTreeView(QWidget* parent = nullptr);

    void setRootDir(QString dir);

public slots:
    void onDoubleClicked(const QModelIndex &index);

private:
    MapAssetTreeModel* m_model;
};



class MapContentsViewProvider : public ContentsViewProvider
{
public:
    MapContentsViewProvider(QObject* parent);
    virtual QString icon() const override { return "fileimageo"; }
    virtual QWidget* createView() override;
	virtual void onChangeCurrentProjet(lna::Project* project) override {}

    MapAssetTreeView* view() const { return m_treeView; }

private:
    MapAssetTreeView* m_treeView;
};

