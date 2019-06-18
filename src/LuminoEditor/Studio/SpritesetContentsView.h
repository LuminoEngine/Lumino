#pragma once
#include "ContentsViewManager.h"


class SpritesetAssetTreeModel : public QFileSystemModel
{
    Q_OBJECT
public:
    explicit SpritesetAssetTreeModel(QObject* parent = nullptr)
        : QFileSystemModel(parent)
    {}
};

class SpritesetAssetTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit SpritesetAssetTreeView(QWidget* parent = nullptr);

    void setRootDir(QString dir);

public slots:
    void onDoubleClicked(const QModelIndex &index);

private:
    SpritesetAssetTreeModel* m_model;
};



class SpritesetContentsViewProvider : public ContentsViewProvider
{
public:
    SpritesetContentsViewProvider(QObject* parent);
    virtual QString icon() const override { return "fileimageo"; }
    virtual QWidget* createView() override;

    SpritesetAssetTreeView* view() const { return m_treeView; }

private:
    SpritesetAssetTreeView* m_treeView;
};

